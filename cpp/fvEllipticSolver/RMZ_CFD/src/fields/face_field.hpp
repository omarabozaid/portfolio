//
//  face_field.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#ifndef face_field_hpp
#define face_field_hpp

#include <iostream>
#include <string>
#include <vector>
#include "../mesh/mesh.hpp"

class face_field{
public:
    
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    
    face_field(string field_name="field_");
    face_field(mesh* discrete_mesh,string field_name="field_");
    face_field(mesh* discrete_mesh,vector_of_doubles values,string field_name="field_");
    ~face_field();
    
    vector_of_doubles field_values();
    vector_of_doubles field_values_prev();
    vector_of_doubles& field_values_ref();
    vector_of_doubles* field_values_pointer();
    mesh* discretization_mesh_ptr();
    void store_old_time();
    string field_name();

private:

    string name_;
    mesh* mesh_ptr_;
    vector_of_doubles field_values_list_;
    vector_of_doubles field_values_list_prev_;
    
};
#endif /* face_field_hpp */
