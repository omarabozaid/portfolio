//
//  robin_BC.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 01/07/2022.
//

#include "robin_BC.hpp"
#include "cell_field.hpp"

robin_BC::robin_BC(
                   mesh* grid,
                   vector_of_pointers_to_faces boundary_faces,
                   vector_of_doubles alpha_dirichlet,
                   vector_of_doubles ref_values,
                   vector_of_vector_of_doubles ref_gradients,
                   map_pointer_of_face_to_int* boundary_face_to_boundary_field_map,
                   cell_field* field_ptr
         ):
grid(grid),
boundary_faces(boundary_faces),
alpha_dirichlet(alpha_dirichlet),
ref_values(ref_values),
ref_gradients(ref_gradients),
field_ptr_(field_ptr),
boundary_face_to_boundary_field_map(boundary_face_to_boundary_field_map)
{   
    compute_laplacian_matrices();
    compute_grad_matrices();
}

robin_BC::robin_BC(mesh* grid,
         vector_of_pointers_to_faces boundary_faces,
         string boundary_file_name,
         map_pointer_of_face_to_int* boundary_face_to_boundary_field_map,
         cell_field* field_ptr,
         bool interpolate
         ):
grid(grid),
boundary_faces(boundary_faces),
field_ptr_(field_ptr),
boundary_face_to_boundary_field_map(boundary_face_to_boundary_field_map)
{
    
    alpha_dirichlet.resize(boundary_faces.size(),1);
    ref_values.resize(boundary_faces.size(),1);
    ref_gradients.resize(boundary_faces.size(),std::vector<double>(3,0));
    table_reader xyz_reader(boundary_file_name,8,false);
    auto data = xyz_reader.table();
    vector_of_points surf_mesh_face_centers((data.size()));
    vector_of_doubles surf_mesh_alpha_dirichlet_values(data.size());
    vector_of_doubles surf_mesh_dirichlet_values(data.size());
    vector_of_vector_of_doubles surf_mesh_grad_values(data.size());
    
    int surf_mesh_face_i = 0;
    for(auto line:data){
        surf_mesh_face_centers[surf_mesh_face_i] = point(std::vector<double>({line[0],line[1],line[2]}));
        surf_mesh_alpha_dirichlet_values[surf_mesh_face_i] = line[3];
        surf_mesh_dirichlet_values[surf_mesh_face_i] = line[4];
        surf_mesh_grad_values[surf_mesh_face_i] = std::vector<double>({line[5],line[6],line[7]});
        surf_mesh_face_i+=1;
    }
    
    std::cout<<"start kd tree "<<std::endl;
    kd_tree<point> search_tree(surf_mesh_face_centers,3);
    int face_i = 0;
    for(auto f:boundary_faces){
        //int nearest_idx = search_tree.knnSearch(f->face_center().coordinates(), 2)[0];
        int nearest_idx = search_tree.knnSearch(f->face_center(), 2)[0];
        alpha_dirichlet[face_i] = surf_mesh_alpha_dirichlet_values[nearest_idx];
        ref_values[face_i] = surf_mesh_dirichlet_values[nearest_idx];
        ref_gradients[face_i] = surf_mesh_grad_values[nearest_idx];
        face_i+=1;
    }
    std::cout<<"end kd tree "<<std::endl;
    
    compute_laplacian_matrices();
    compute_grad_matrices();
}


robin_BC::sp_mat robin_BC::laplacian_A_matrix(){
    return laplacian_A_;
}

robin_BC::eigen_vector robin_BC::laplacian_B_vector(){
    return laplacian_B_;
}

robin_BC::vector_of_sp_mat robin_BC::gradient_A_matrix(){
    return gradient_A_;
}

robin_BC::vector_of_eigen_vector robin_BC::gradient_B_vector(){
    
    return gradient_B_;
}

void robin_BC::update_boundary_values(){
    auto& cell_values = field_ptr_->field_values_ref();
    auto& boundary_values = field_ptr_->field_boundary_values_ref() ;
    boundary_values.resize(boundary_faces.size());
    int counter = 0 ;
    for(auto& f:boundary_faces){
        auto x_o=f->owner_neighbour()[0]->cell_center().coordinates();
        auto x_f=f->face_center().coordinates();
        auto n_o_f=x_f-x_o;
        auto o_value=cell_values[f->owner_neighbour()[0]->index()];
        auto& grad = ref_gradients[counter];
        auto extrapolated_value = dot(grad,n_o_f)+o_value;
        boundary_values[counter]= alpha_dirichlet[counter]*ref_values[counter] + (1-alpha_dirichlet[counter])*extrapolated_value;
        counter+=1;
    }
}

robin_BC::vector_of_pointers_to_faces robin_BC::boundary_faces_list(){
    return boundary_faces;
}

void robin_BC::compute_grad_matrices(){
    int n_cells=grid->n_cells();
    sp_mat Ax(n_cells,n_cells);
    Ax*=0;
    sp_mat Ay(n_cells,n_cells);
    Ay*=0;
    sp_mat Az(n_cells,n_cells);
    Az*=0;
    int counter=0;
    for(auto f:boundary_faces){
        if(f->is_boundary_face()){
            auto x_o=f->owner_neighbour()[0]->cell_center().coordinates();
            auto x_f=f->face_center().coordinates();
            auto n_o_f=x_f-x_o;
            n_o_f=normalize(n_o_f);
            int oIdx=f->owner_neighbour()[0]->index();
            auto f_area=f->area();
            auto o_vol=f->owner_neighbour()[0]->vol();
            auto o_weight_x=(1-alpha_dirichlet[counter])*f_area*(n_o_f[0]/o_vol);
            auto o_weight_y=(1-alpha_dirichlet[counter])*f_area*(n_o_f[1]/o_vol);
            auto o_weight_z=(1-alpha_dirichlet[counter])*f_area*(n_o_f[2]/o_vol);
            Ax.coeffRef(oIdx,oIdx)+=o_weight_x;
            Ay.coeffRef(oIdx,oIdx)+=o_weight_y;
            Az.coeffRef(oIdx,oIdx)+=o_weight_z;
        }
        counter+=1;
    }
    Ax.makeCompressed();
    Ay.makeCompressed();
    Az.makeCompressed();
    gradient_A_ = vector_of_sp_mat({Ax,Ay,Az});
    counter=0;
    eigen_vector bx(n_cells);
    eigen_vector by(n_cells);
    eigen_vector bz(n_cells);
    bx*=0;
    by*=0;
    bz*=0;
    for(auto f:boundary_faces){
        if(f->is_boundary_face()){
            auto x_o=f->owner_neighbour()[0]->cell_center().coordinates();
            auto x_f=f->face_center().coordinates();
            auto n_o_f=x_f-x_o;
            auto extrapolated_value =dot(ref_gradients[counter],n_o_f);
            auto boundary_value = alpha_dirichlet[counter]*ref_values[counter] + (1-alpha_dirichlet[counter])*extrapolated_value;
            n_o_f=normalize(n_o_f);
            int oIdx=f->owner_neighbour()[0]->index();
            auto f_area=f->area();
            auto o_vol=f->owner_neighbour()[0]->vol();
            auto o_weight_x=boundary_value*f_area*(n_o_f[0]/o_vol);
            auto o_weight_y=boundary_value*f_area*(n_o_f[1]/o_vol);
            auto o_weight_z=boundary_value*f_area*(n_o_f[2]/o_vol);
            bx(oIdx)+=o_weight_x;
            by(oIdx)+=o_weight_y;
            bz(oIdx)+=o_weight_z;
        }
        counter+=1;
    }
    gradient_B_ = vector_of_eigen_vector({bx,by,bz});
}

void robin_BC::compute_laplacian_matrices(){
    auto n_cells=grid->n_cells();
    laplacian_A_ = sp_mat(n_cells,n_cells);
    laplacian_A_.reserve(Eigen::VectorXi::Constant(grid->n_cells(),10));
    laplacian_A_*=0;
    int counter=0;
    for(auto f:boundary_faces){
        if(f->is_boundary_face()){
            auto owner_neighbor = f->owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto o_vol=owner_neighbor[0]->vol();
            auto d=f->distances()[0];
            laplacian_A_.coeffRef(oIdx,oIdx)-=alpha_dirichlet[counter]*(f->area()/(o_vol*d));
            counter+=1;
        }
    }
    laplacian_A_.makeCompressed();
    laplacian_B_ =eigen_vector(n_cells);
    laplacian_B_*=0;
    counter=0;
    for(auto f:boundary_faces){
        if(f->is_boundary_face()){
            auto owner_neighbor = f->owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto o_vol=owner_neighbor[0]->vol();
            auto n=f->face_center().coordinates()-owner_neighbor[0]->cell_center().coordinates();
            n=normalize(n);
            auto d=f->distances()[0];
            laplacian_B_(oIdx)+=(f->area()/(o_vol*d))*(
                                           alpha_dirichlet[counter]*ref_values[counter]+
                                           dot(ref_gradients[counter],n)*d*(1-alpha_dirichlet[counter])
                                           );
            counter+=1;
        }
    }
}


void robin_BC::set_field_ptr(cell_field* field_ptr){
    field_ptr_=field_ptr;
}
