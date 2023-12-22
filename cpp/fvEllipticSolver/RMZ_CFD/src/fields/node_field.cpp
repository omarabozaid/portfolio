//
//  node_field.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 15/08/2022.
//

#include "node_field.hpp"

node_field::node_field(mesh* mesh_ptr, string name):
mesh_ptr_(mesh_ptr),
field_name_(name),
nodal_values_(mesh_ptr->n_vertices(),0.0)
{}

node_field::node_field(cell_field& field):
mesh_ptr_(field.discretization_mesh_ptr()),
field_name_("nodal_"+field.field_name()),
nodal_values_(field.discretization_mesh_ptr()->n_vertices(),0.0)
{}

node_field::~node_field()
{}

node_field::vector_of_double node_field::nodal_values(){
    return nodal_values_;
}

node_field::vector_of_double& node_field::nodal_values_ref(){
    return nodal_values_;
}

node_field::string node_field::field_name(){
    return field_name_;
}
