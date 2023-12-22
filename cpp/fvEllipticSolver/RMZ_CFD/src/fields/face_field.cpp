//
//  face_field.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#include "face_field.hpp"

face_field::face_field(string field_name):
mesh_ptr_(nullptr),
name_(field_name)
{}

face_field::face_field(mesh* discrete_mesh,string field_name):
mesh_ptr_(discrete_mesh),
name_(field_name)
{
    field_values_list_=vector_of_doubles(mesh_ptr_->n_faces(),0.0);
}

face_field::face_field(mesh* discrete_mesh,face_field::vector_of_doubles values,string field_name):
mesh_ptr_(discrete_mesh),
name_(field_name),
field_values_list_(values)
{}

face_field::~face_field()
{}

face_field::vector_of_doubles face_field::field_values(){
    return field_values_list_;
}


face_field::vector_of_doubles face_field::field_values_prev(){
    return field_values_list_prev_;
}

face_field::vector_of_doubles& face_field::field_values_ref(){
    return field_values_list_;
}

face_field::vector_of_doubles* face_field::field_values_pointer(){
    return &field_values_list_;
}

void face_field::store_old_time(){
    field_values_list_prev_=field_values_list_;
}

mesh* face_field::discretization_mesh_ptr(){
    return mesh_ptr_;
}

face_field::string face_field::field_name(){
    return name_;
}
