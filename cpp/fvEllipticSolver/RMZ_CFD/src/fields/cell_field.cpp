//
//  cell_field.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#include "cell_field.hpp"
#include "robin_BC.hpp"

cell_field::cell_field(string field_name,robin_BC* field_boundary):
mesh_ptr_(nullptr),
name_(field_name),
field_boundary(field_boundary)
{
    if(field_boundary != nullptr){
        field_boundary->set_field_ptr(this);
        field_boundary_values_=vector_of_doubles(field_boundary->boundary_faces_list().size(),0.0);
        update_boundary_values();
    }
    else if(mesh_ptr_ != nullptr){
        field_boundary_values_=vector_of_doubles(mesh_ptr_->n_boundary_faces(),0.0);
    }
}

cell_field::cell_field(mesh* discrete_mesh,string field_name, robin_BC* field_boundary):
mesh_ptr_(discrete_mesh),
name_(field_name),
field_boundary(field_boundary)
{
    field_values_list_=vector_of_doubles(mesh_ptr_->n_cells(),0.0);
    if(field_boundary != nullptr){
        field_boundary->set_field_ptr(this);
        field_boundary_values_=vector_of_doubles(field_boundary->boundary_faces_list().size(),0.0);
    }
}

cell_field::cell_field(mesh* discrete_mesh,cell_field::vector_of_doubles values,string field_name, robin_BC* field_boundary):
mesh_ptr_(discrete_mesh),
name_(field_name),
field_values_list_(values),
field_boundary(field_boundary)
{
    if(field_boundary != nullptr){
        field_boundary->set_field_ptr(this);
        field_boundary_values_=vector_of_doubles(field_boundary->boundary_faces_list().size(),0.0);
    }
}

cell_field::~cell_field()
{}

cell_field::vector_of_doubles cell_field::field_values(){
    return field_values_list_;
}

cell_field::vector_of_doubles cell_field::field_boundary_values(){
    return field_boundary_values_;
}

cell_field::vector_of_doubles cell_field::field_values_prev(){
    return field_values_list_prev_;
}

cell_field::vector_of_doubles cell_field::field_boundary_values_prev(){
    return field_boundary_values_prev_;
}

cell_field::vector_of_doubles& cell_field::field_values_ref(){
    return field_values_list_;
}

cell_field::vector_of_doubles& cell_field::field_boundary_values_ref(){
    return field_boundary_values_;
}

cell_field::vector_of_doubles* cell_field::field_values_pointer(){
    return &field_values_list_;
}

cell_field::vector_of_doubles* cell_field::field_boundary_values_pointer(){
    return &field_boundary_values_;
}

void cell_field::store_old_time(){
    field_values_list_prev_=field_values_list_;
    field_boundary_values_prev_=field_boundary_values_;
}

mesh* cell_field::discretization_mesh_ptr(){
    return mesh_ptr_;
}

cell_field::string cell_field::field_name(){
    return name_;
}

void cell_field::set_bc(robin_BC* bc){
    field_boundary=bc;
}

robin_BC* cell_field::bc(){
    return field_boundary;
}

void cell_field::update_boundary_values(){
    if(field_boundary != nullptr){
        field_boundary->update_boundary_values();
    }
    else{
        std::cout<<"no boundary condition available for this field "<<std::endl;
    }
}
