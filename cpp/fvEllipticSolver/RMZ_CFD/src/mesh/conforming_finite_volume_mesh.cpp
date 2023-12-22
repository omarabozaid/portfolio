//
//  conforming_finite_volume_mesh.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 18/07/2022.
//

#include "conforming_finite_volume_mesh.hpp"

conforming_finite_volume_mesh::conforming_finite_volume_mesh(
             vector_of_cells& cells_list,
             vector_of_faces& faces_list,
             vector_of_pointers_to_faces single_cell_faces_list,
             vector_of_nodes& nodes_list,
             vector_of_points& points_list,
             vector_of_pointers_to_nodes& single_boundary_nodes_list,
             physical_label_faces_ptrs& physical_label_physical_faces_ptrs,
             physical_name_faces_ptrs& physical_name_physical_faces_ptrs,
             label_label_map physical_label_physical_type,
             label_string_map physical_label_physical_name,
             int n_boundary_nodes,
             int n_boundary_cells,
             int n_boundary_faces,
             int n_triangles,
             int n_quads,
             int n_edges,
             int n_tetras,
             int n_pyramids,
             int n_prisms,
             int n_hexas
          ):
          cells_list_(cells_list),
          faces_list_(faces_list),
          single_cell_faces_list_(single_cell_faces_list),
          nodes_list_(nodes_list),
          points_list_(points_list),
          single_boundary_nodes_list_(single_boundary_nodes_list),
          physical_label_physical_faces_ptrs_(physical_label_physical_faces_ptrs),
          physical_name_physical_faces_ptrs_(physical_name_physical_faces_ptrs),
          physical_label_physical_type_(physical_label_physical_type),
          physical_label_physical_name_(physical_label_physical_name),
          n_faces_(int(faces_list.size())),
          n_cells_(int(cells_list.size())),
          n_vertices_(int(nodes_list.size())),
          n_boundary_nodes_(n_boundary_nodes),
          n_boundary_cells_(n_boundary_cells),
          n_boundary_faces_(n_boundary_faces),
          n_triangles_(n_triangles),
          n_quads_(n_quads),
          n_edges_(n_edges),
          n_tetras_(n_tetras),
          n_pyramids_(n_pyramids),
          n_prisms_(n_prisms),
          n_hexas_(n_hexas)
{
};

conforming_finite_volume_mesh::~conforming_finite_volume_mesh(){}

void conforming_finite_volume_mesh::handle_zero_volume_cells(double vol_tol){
   for(auto& c:cells_list_){
      if(c.vol()<=vol_tol){
         //std::cout<<"capping one cell whose index is "<<c.index()<<std::endl;
         c.set_vol(vol_tol);
         /*
         auto c_faces_ptrs_ = c.neighbouring_faces();
         for(auto f_ptr_:c_faces_ptrs_){
            f_ptr_->set_area(0.0);
         }
         */
      }
   }
}