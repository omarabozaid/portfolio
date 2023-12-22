//
//  node_field.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 15/08/2022.
//

#ifndef node_field_hpp
#define node_field_hpp

#include <vector>
#include <string>

#include "../mesh/mesh.hpp"
#include "cell_field.hpp"

class node_field{
public:
    typedef std::vector<double> vector_of_double;
    typedef std::string string;
    
    vector_of_double nodal_values();
    vector_of_double& nodal_values_ref();
    string field_name();
    mesh* mesh_ptr(){return mesh_ptr_;};
    node_field(mesh* mesh_ptr,string name);
    node_field(cell_field& field);
    ~node_field();
    
private:
    mesh* mesh_ptr_;
    vector_of_double nodal_values_;
    string field_name_;
};

#endif /* node_field_hpp */
