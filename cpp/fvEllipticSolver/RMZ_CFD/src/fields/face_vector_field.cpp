//
//  face_vector_field.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#include "face_vector_field.hpp"


face_vector_field::face_vector_field():
name_("empty_field")
{
    components_.push_back(face_field("field_x"));
    components_.push_back(face_field("field_y"));
    components_.push_back(face_field("field_z"));

}


face_vector_field::face_vector_field(string field_name):
name_(field_name)
{
    components_.push_back(face_field(field_name+"_x"));
    components_.push_back(face_field(field_name+"_y"));
    components_.push_back(face_field(field_name+"_z"));
}


face_vector_field::face_vector_field(vector_of_cell_fields components,string field_name):
name_(field_name),
components_(components)
{}


face_vector_field::~face_vector_field()
{}

mesh* face_vector_field::discretization_mesh_ptr(){
    return components_[0].discretization_mesh_ptr();
}

void face_vector_field::store_old_time(){
    for(auto& cmp:components_){
        cmp.store_old_time();
    }
}


face_vector_field::string face_vector_field::field_name(){
    return name_;
}

face_vector_field::vector_of_cell_fields face_vector_field::components(){
    return components_;
}

face_vector_field::vector_of_cell_fields& face_vector_field::components_ref(){
    return components_;
}

face_vector_field::vector_of_cell_fields* face_vector_field::components_ptr(){
    return &components_;
}

face_field face_vector_field::operator&(face_vector_field field){
    string name=field.field_name()+"_dot_"+name_;
    face_field xx(name);
    std::vector<double>& xx_values_ref=xx.field_values_ref();
    std::vector<std::vector<double>> v1({field.components()[0].field_values_ref(),field.components()[1].field_values_ref(),field.components()[2].field_values_ref()});
    std::vector<std::vector<double>> v2({components_[0].field_values_ref(),components_[1].field_values_ref(),components_[2].field_values_ref()});
    xx_values_ref=v1&v2;
    return xx;
}
