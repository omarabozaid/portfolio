//
//  node_cell_interpolation.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 28/08/2022.
//

#ifndef node_cell_interpolation_hpp
#define node_cell_interpolation_hpp


#include <vector>
#include <string>

#include "mesh.hpp"
#include "cell_field.hpp"
#include "node_field.hpp"

class node_cell_interpolation{
public:
    
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    
    node_cell_interpolation(mesh* mesh_ptr);
    ~node_cell_interpolation(){};
    vector_of_doubles interpolate(node_field* scalar_field);
    void interpolate(node_field* scalar_field,cell_field* scalar_cell_field);
private:
    mesh* mesh_ptr_;
    vector_of_vector_of_doubles weights_;

};
#endif /* node_cell_interpolation_hpp */
