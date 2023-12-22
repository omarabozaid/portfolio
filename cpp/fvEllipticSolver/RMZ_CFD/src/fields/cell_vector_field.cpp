//
//  cell_vector_field.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#include "cell_vector_field.hpp"

cell_vector_field::cell_vector_field():
name_("empty_field")
{
    components_.push_back(cell_field("field_x"));
    components_.push_back(cell_field("field_y"));
    components_.push_back(cell_field("field_z"));

}


cell_vector_field::cell_vector_field(string field_name):
name_(field_name)
{
    components_.push_back(cell_field(field_name+"_x"));
    components_.push_back(cell_field(field_name+"_y"));
    components_.push_back(cell_field(field_name+"_z"));
}


cell_vector_field::cell_vector_field(vector_of_cell_fields components,string field_name):
name_(field_name),
components_(components)
{}


cell_vector_field::~cell_vector_field()
{}

mesh* cell_vector_field::discretization_mesh_ptr(){
    return components_[0].discretization_mesh_ptr();
}

void cell_vector_field::store_old_time(){
    for(auto& cmp:components_){
        cmp.store_old_time();
    }
}


cell_vector_field::string cell_vector_field::field_name(){
    return name_;
}

cell_vector_field::vector_of_cell_fields cell_vector_field::components(){
    return components_;
}

cell_vector_field::vector_of_cell_fields& cell_vector_field::components_ref(){
    return components_;
}

cell_vector_field::vector_of_cell_fields* cell_vector_field::components_ptr(){
    return &components_;
}

cell_field cell_vector_field::operator&(cell_vector_field field){
    string name=field.field_name()+"_dot_"+name_;
    cell_field xx(name);
    std::vector<double>& xx_values_ref=xx.field_values_ref();
    std::vector<std::vector<double>> v1({field.components()[0].field_values_ref(),field.components()[1].field_values_ref(),field.components()[2].field_values_ref()});
    std::vector<std::vector<double>> v2({components_[0].field_values_ref(),components_[1].field_values_ref(),components_[2].field_values_ref()});
    xx_values_ref=v1&v2;
    return xx;
}
