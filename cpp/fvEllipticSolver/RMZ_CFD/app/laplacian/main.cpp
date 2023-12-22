//
//  main.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 05/07/2022.
//

#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "vtk_writer.hpp"
#include "gmsh_reader.hpp"
#include "conforming_finite_volume_mesh.hpp"
#include "cell_field.hpp"
#include "laplacian.hpp"
#include "cell_node_interpolation.hpp"
#include "node_cell_interpolation.hpp"
#include "robin_BC.hpp"

#include "Core"
#include "Sparse"
#include "SparseLU"
#include "LU"

//---------------------------------------------------------------------------//
// Define functions
void smooth_field(
                  conforming_finite_volume_mesh* mesh,
                  node_field* node_scalar_field,
                  cell_field* cell_scalar_field,
                  int n_smoothing_iterations,
                  cell_node_interpolation* cell_values_node_values_ptr,
                  node_cell_interpolation* node_values_cell_values
                  );

void read_solution_parameters(
    std::string parameters_file_name,
    std::string& mesh_file_name,
    int& n_smoothing_iterations
){
    std::fstream in_file;
    in_file.open(parameters_file_name);
    in_file>>mesh_file_name;
    in_file>>n_smoothing_iterations;
    in_file.close();
}


int main(int argc, const char * argv[]) {

    clock_t tStart = clock();

    std::string parameters_file_name = argv[1];
    std::string mesh_file_name;
    int n_smoothing_iterations;

    read_solution_parameters(parameters_file_name, mesh_file_name, n_smoothing_iterations);


    //---------------------------------------------------------------------------//
    // BEGIN //
    // Read tetrahedral gmsh grid
    typedef conforming_finite_volume_mesh conf_fv_mesh;
    gmsh_reader read_gmsh_file(mesh_file_name);
    conf_fv_mesh& grid = read_gmsh_file.computational_grid();
    grid.handle_zero_volume_cells(1e-9);
    
    //---------------------------------------------------------------------------//
    // Numerical operators
    std::cout<<"calling eigen for laplacian operator "<<std::endl;
    typedef Eigen::SparseMatrix<double,Eigen::RowMajor> sp_mat;
    typedef Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> bicgstab;
    typedef Eigen::VectorXd eigen_vector;
    laplacian laplacian_object;
    cell_node_interpolation cell_values_node_values(grid);
    node_cell_interpolation node_values_cell_values(&grid);
    sp_mat laplacian_operator = laplacian_object.laplacian_operator(&grid);
    
    //---------------------------------------------------------------------------//
    cell_field u(&grid,"eta");
    auto& u_vals_ref = u.field_values_ref();
     
    //---------------------------------------------------------------------------//
    // Grab boundary conditions "Box, Ship"


    auto boundary_faces = grid.boundary_faces_list();
    std::map<int,int> face_index_face_array_index_map;
    int counter = 0;
    for(auto face_ptr:boundary_faces){
        face_index_face_array_index_map.insert(std::make_pair(face_ptr->index(),counter));
        counter+=1;
    }
    auto n_boundary_faces = grid.n_boundary_faces();
    std::vector<double> dummy_zero(3,0) ;
    std::vector<double> dirichlet_eta_filter(n_boundary_faces,-1);
    std::vector<double> dirichlet_eta_values(n_boundary_faces,-1);
    std::vector<std::vector<double>> gradient_eta_values(n_boundary_faces,dummy_zero);
    std::vector<std::string> phys_searchable_names({"\"box\"", "\"ship\"", "\"fluid\""});
    auto phys_patches = grid.physical_name_physical_faces_ptrs();
    
    for(auto& pair:phys_patches){
        auto phys_name = pair.first;
        if(phys_name == phys_searchable_names[0]){
            auto vector_of_ptrs_phys_faces = pair.second;
            for(auto& f:vector_of_ptrs_phys_faces){
                auto index = face_index_face_array_index_map.find(f->index())->second;
                dirichlet_eta_filter[index]=1;
                dirichlet_eta_values[index]=1;
            }
        }
        else if(phys_name == phys_searchable_names[1]){
            auto vector_of_ptrs_phys_faces = pair.second;
            for(auto& f:vector_of_ptrs_phys_faces){
                auto index = face_index_face_array_index_map.find(f->index())->second;
                dirichlet_eta_filter[index]=1;
                dirichlet_eta_values[index]=0;
            }
        }
        else if (phys_name == phys_searchable_names[2]){
            continue;
        }
        else{
            std::cout<<"unpredicted patch name: "<<phys_name<<std::endl;
        }
    }
    
    robin_BC u_bc(
                 &grid,
                 boundary_faces,
                 dirichlet_eta_filter,
                 dirichlet_eta_values,
                 gradient_eta_values,
                 nullptr,
                 &u
                 );
    u.set_bc(&u_bc);
    std::cout<<"n° faces in robin-boundary-condition class is: "<<boundary_faces.size()<<std::endl;
    
    //---------------------------------------------------------------------------//
    sp_mat u_laplacian_A = u_bc.laplacian_A_matrix();
    eigen_vector u_laplacian_B = u_bc.laplacian_B_vector();
    bicgstab  BCGST_u;

    BCGST_u.compute(laplacian_operator+u_laplacian_A);
    std::cout<<"solving u field "<<std::endl;
    BCGST_u.setTolerance(1e-16);
    BCGST_u.setMaxIterations(2000);
    eigen_vector x_u = BCGST_u.solve(-u_laplacian_B);
    std::cout << "#iterations:     " << BCGST_u.iterations() << std::endl;
    std::cout << "estimated error: " << BCGST_u.error()      << std::endl;

    std::cout<<"casting u field "<<std::endl;

    u_vals_ref=std::vector<double>(x_u.data(), x_u.data() + x_u.rows() * x_u.cols());
    std::cout<<"correcting bc "<<std::endl;
    u.update_boundary_values();
    std::cout<<"interpolating "<<std::endl;
    auto u_nodal = cell_values_node_values.interpolate(u);
    
    //---------------------------------------------------------------------------//
    // Smoothing
    std::cout<<"smoothing the laplacian field "<<std::endl;
    node_field u_smooth(&grid,"eta_smoothed");
    u_smooth.nodal_values_ref() = u_nodal.nodal_values();
    smooth_field(&grid,&u_smooth,&u,n_smoothing_iterations,&cell_values_node_values,&node_values_cell_values);
    
    //---------------------------------------------------------------------------//
    // Dump fields
    std::cout<<"writing "<<std::endl;
    double dir = 1;
    std::vector<cell_field*> cell_field_ptrs;
    cell_field_ptrs.push_back(&u);
    std::vector<node_field*> node_field_ptrs;
    node_field_ptrs.push_back(&u_nodal);
    node_field_ptrs.push_back(&u_smooth);
    vtk_writer vtk_output_case(&grid,cell_field_ptrs,node_field_ptrs,dir);

    //---------------------------------------------------------------------------//
    // PRINT CONSUMED TIME
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    return 0;
}

void smooth_field(
                  conforming_finite_volume_mesh* mesh,
                  node_field* node_scalar_field,
                  cell_field* cell_scalar_field,
                  int n_smoothing_iterations,
                  cell_node_interpolation* cell_values_node_values_ptr,
                  node_cell_interpolation* node_values_cell_values
                  ){
    for(int i=0; i<n_smoothing_iterations; i++){
        std::cout<<"smoothing iteration "<<i<<std::endl;
        node_values_cell_values->interpolate(node_scalar_field,cell_scalar_field);
        cell_values_node_values_ptr->interpolate(*cell_scalar_field,*node_scalar_field);
    }
}
