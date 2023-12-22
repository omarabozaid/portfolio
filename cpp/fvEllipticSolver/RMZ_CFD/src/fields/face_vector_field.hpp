//
//  face_vector_field.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#ifndef face_vector_field_hpp
#define face_vector_field_hpp

#include <iostream>
#include <string>
#include <vector>

#include "face_field.hpp"

class face_vector_field{
public:
    
    typedef std::string string;
    typedef std::vector<face_field*> vector_of_pointers_to_cell_fields;
    typedef std::vector<face_field> vector_of_cell_fields;
    
    face_vector_field();
    face_vector_field(string field_name="field_");
    face_vector_field(vector_of_cell_fields components,string field_name="field_");
    ~face_vector_field();
    
    mesh* discretization_mesh_ptr();
    void store_old_time();
    string field_name();
    vector_of_cell_fields components();
    vector_of_cell_fields& components_ref();
    vector_of_cell_fields* components_ptr();
    
    face_field operator&(face_vector_field field);

private:

    string name_;
    vector_of_cell_fields components_;
};
#endif /* face_vector_field_hpp */
