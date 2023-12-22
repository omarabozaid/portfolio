//
//  cell_field.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#ifndef cell_field_hpp
#define cell_field_hpp


#include <iostream>
#include <string>
#include <vector>
#include "mesh.hpp"


class robin_BC;

class cell_field{
public:
    
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    
    cell_field(string field_name="field_",robin_BC* field_boundary=NULL);
    cell_field(mesh* discrete_mesh,string field_name="field_",robin_BC* field_boundary=NULL);
    cell_field(mesh* discrete_mesh,vector_of_doubles values,string field_name="field_",robin_BC* field_boundary=NULL);
    ~cell_field();
    
    vector_of_doubles field_values();
    vector_of_doubles field_boundary_values();
    vector_of_doubles field_values_prev();
    vector_of_doubles field_boundary_values_prev();
    vector_of_doubles& field_values_ref();
    vector_of_doubles& field_boundary_values_ref();
    vector_of_doubles* field_values_pointer();
    vector_of_doubles* field_boundary_values_pointer();
    mesh* discretization_mesh_ptr();
    string field_name();
    robin_BC* bc();
    void update_boundary_values();
    void set_bc(robin_BC* bc);
    void store_old_time();

private:

    string name_;
    mesh* mesh_ptr_;
    vector_of_doubles field_values_list_;
    vector_of_doubles field_boundary_values_;
    vector_of_doubles field_values_list_prev_;
    vector_of_doubles field_boundary_values_prev_;
    robin_BC* field_boundary;
    
};
#endif /* cell_field_hpp */
