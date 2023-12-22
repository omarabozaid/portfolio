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
#include <time.h>

#include "vtk_writer.hpp"
#include "gmsh_reader.hpp"
#include "stl_reader.hpp"
#include "conforming_finite_volume_mesh.hpp"
#include "node_field.hpp"
#include "point.hpp"
#include "vector_operators_overloaded.hpp"
#include "kd_tree.h"


int main(int argc, const char * argv[]){
    
    clock_t tStart = clock();

    //---------------------------------------------------------------------------//
    // BEGIN 
    // Read gmsh grid
    std::string file_name = argv[1];
    gmsh_reader read_gmsh_file(file_name);
    auto& grid = read_gmsh_file.computational_grid();
    //---------------------------------------------------------------------------//
    node_field sdf(&grid,"phi");
    auto& sdf_vals_ref = sdf.nodal_values_ref();
    
    //---------------------------------------------------------------------------//
    // READ STL FILE
    file_name = argv[2];
    auto stl = stl_reader(file_name);
    auto points_of_triangls = stl.points_list();
    auto normals_of_triangles = stl.normals_list();
    std::cout<<"start kd tree "<<std::endl;
    auto& nodes_list = grid.nodes_list_ref();
    kd_tree<point> search_tree(points_of_triangls,3);
    for(auto& vertex:nodes_list){
        auto pt = vertex.pt();
        auto nearest_idx = search_tree.knnSearch(pt.coordinates(),2)[0];
        auto nearest_point = points_of_triangls[nearest_idx];
        auto ray = (pt-nearest_point).coordinates();
        auto normal = normals_of_triangles[nearest_idx];
        sdf_vals_ref[vertex.index()]=magnitude(ray);
        if(dot(normal, ray)<0){
            sdf_vals_ref[vertex.index()]*=-1;
        }
    }
    std::cout<<"end kd tree "<<std::endl;
    
    //---------------------------------------------------------------------------//
    // Dump fields
    double dir = 1;
    std::vector<cell_field*> cell_field_ptrs;
    std::vector<node_field*> node_field_ptrs;
    node_field_ptrs.push_back(&sdf);
    vtk_writer vtk_output_case(&grid,cell_field_ptrs,node_field_ptrs,dir);
    
    //---------------------------------------------------------------------------//
    // PRINT CONSUMED TIME
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}
