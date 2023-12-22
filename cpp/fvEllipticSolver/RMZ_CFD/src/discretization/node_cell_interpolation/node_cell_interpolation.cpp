//
//  node_cell_interpolation.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 28/08/2022.
//

#include "node_cell_interpolation.hpp"

node_cell_interpolation::node_cell_interpolation(mesh* mesh_ptr):
mesh_ptr_(mesh_ptr)
{
    auto& cells_list = mesh_ptr_->cells_list_ref();
    for(auto& cell:cells_list){
        auto n_nodes = cell.cell_nodes().size();
        vector_of_doubles w(n_nodes,0.0);
        double dist_ = 0.0;
        auto x_cell = cell.cell_center().coordinates();
        int counter = 0;
        for(auto& vertex:cell.cell_nodes()){
            auto x_vertex = vertex->pt().coordinates();
            auto x_v_c = x_cell-x_vertex;
            auto d_v_c = magnitude(x_v_c);
            w[counter]+=d_v_c;
            dist_+=d_v_c;
            counter+=1;
        }
        for(auto& weight:w){
            weight/=dist_;
        }
        weights_.push_back(w);
    }
}

node_cell_interpolation::vector_of_doubles node_cell_interpolation::interpolate(node_field *scalar_field){
    auto n_cells = mesh_ptr_->n_cells();
    auto& nodal_values = scalar_field->nodal_values_ref();
    vector_of_doubles interpolated_values(n_cells,0);
    auto& cells_list = mesh_ptr_->cells_list_ref();
    for(auto& cell:cells_list){
        auto cell_idx = cell.index();
        int counter = 0;
        for(auto& vertex:cell.cell_nodes()){
            interpolated_values[cell_idx]+= nodal_values[vertex->index()]*weights_[cell_idx][counter];
            counter+=1;
        }
    }
    return interpolated_values;
}

void node_cell_interpolation::interpolate(node_field *scalar_field,cell_field* scalar_cell_field){
    auto& nodal_values = scalar_field->nodal_values_ref();
    auto& interpolated_values = scalar_cell_field->field_values_ref();
    auto& cells_list = mesh_ptr_->cells_list_ref();
    for(auto& cell:cells_list){
        auto cell_idx = cell.index();
        int counter = 0;
        interpolated_values[cell_idx] = 0.0 ;
        for(auto& vertex:cell.cell_nodes()){
            interpolated_values[cell_idx]+= (nodal_values[vertex->index()]*weights_[cell_idx][counter]);
            counter+=1;
        }
    }
}
