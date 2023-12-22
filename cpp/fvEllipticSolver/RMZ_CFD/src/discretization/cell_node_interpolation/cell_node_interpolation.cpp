//
//  cell_node_interpolation.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 13/08/2022.
//

#include "cell_node_interpolation.hpp"

cell_node_interpolation::cell_node_interpolation(mesh& grid):
grid(grid)
{
    compute_cell_weights();
    compute_face_weights();
}

cell_node_interpolation::~cell_node_interpolation()
{}

void cell_node_interpolation::compute_cell_weights(){
    cell_weights.resize(grid.n_vertices());
    auto& nodes = grid.nodes_list_ref();
    for(auto& n:nodes){
        auto node_xyz = n.pt().coordinates();
        auto stencil = n.cells_stencil();
        int counter = 0;
        auto& weights = cell_weights[n.index()];
        weights = vector_of_double(stencil.size(),0.0);
        double d_sum = 0.0;
        for(auto& c:stencil){
            auto cell_xyz = c->cell_center().coordinates();
            auto vec_cell_node = node_xyz-cell_xyz;
            auto distance = magnitude(vec_cell_node);
            weights[counter]=distance;
            d_sum += distance;
            counter+=1;
        }
        for(auto& weight:weights){
            weight/=d_sum;
        }
    }
}

void cell_node_interpolation::compute_face_weights(){
    nodes_faces_d_sum.resize(grid.n_vertices(),0.0);
    auto boundary_faces = grid.boundary_faces_list();
    face_nodes_distances.resize(boundary_faces.size());
    int counter = 0;
    for(auto& f:boundary_faces){
        auto f_xyz = f->face_center().coordinates();
        auto& stencil_distances = face_nodes_distances[counter];
        auto stencil = f->face_nodes();
        stencil_distances = vector_of_double(stencil.size(),0.0);
        int inner_counter = 0;
        for(auto& n:stencil){
            auto n_xyz = n->pt().coordinates();
            auto v = n_xyz-f_xyz;
            auto d = magnitude(v);
            auto n_idx = n->index();
            nodes_faces_d_sum[n_idx]+=d;
            stencil_distances[inner_counter] = d ;
            inner_counter+=1;
        }
        counter+=1;
    }
}

node_field cell_node_interpolation::interpolate(cell_field& u){
    node_field u_nodal(u);
    auto& nodal_values = u_nodal.nodal_values_ref();
    auto& field_cell_values = u.field_values_ref();
    auto& nodes = grid.nodes_list_ref();
    for(auto& n:nodes){
        auto node_idx = n.index();
        auto& weights = cell_weights[node_idx];
        auto stencil = n.cells_stencil();
        for(int i=0; i<weights.size(); i++){
            auto cell_idx = stencil[i]->index();
            nodal_values[node_idx]+=weights[i]*field_cell_values[cell_idx];
        }
    }
    correct_boundary_nodes(u, u_nodal);
    return u_nodal;
}

void cell_node_interpolation::interpolate(cell_field& u, node_field& scalar_field){
    auto& nodal_values = scalar_field.nodal_values_ref();
    auto& field_cell_values = u.field_values_ref();
    auto& nodes = grid.nodes_list_ref();
    for(auto& n:nodes){
        auto node_idx = n.index();
        auto& weights = cell_weights[node_idx];
        auto stencil = n.cells_stencil();
        nodal_values[node_idx] = 0.0 ;
        for(int i=0; i<weights.size(); i++){
            auto cell_idx = stencil[i]->index();
            nodal_values[node_idx]+=weights[i]*field_cell_values[cell_idx];
        }
    }
    correct_boundary_nodes(u, scalar_field);
}


void cell_node_interpolation::correct_boundary_nodes(cell_field& u, node_field& u_nodal){
    auto& nodal_values_ref_ = u_nodal.nodal_values_ref();
    auto& face_values_ref_ = u.field_boundary_values_ref();
    auto boundary_faces = grid.boundary_faces_list();
    auto boundary_nodes = grid.boundary_nodes_list();
    for(auto& n:boundary_nodes){
        nodal_values_ref_[n->index()]=0.0;
    }
    int counter = 0;
    for(auto& f:boundary_faces){
        auto stencil = f->face_nodes();
        auto& stencil_distances = face_nodes_distances[counter];
        int inner_counter = 0 ;
        for(auto& n:stencil){
            auto node_index_ = n->index();
            auto weight = stencil_distances[inner_counter]/nodes_faces_d_sum[node_index_];
            nodal_values_ref_[node_index_]+=face_values_ref_[counter]*weight;
            inner_counter+=1;
        }
        counter+=1;
    }
}
