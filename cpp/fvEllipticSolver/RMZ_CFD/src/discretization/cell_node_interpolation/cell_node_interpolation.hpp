//
//  cell_node_interpolation.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 13/08/2022.
//

#ifndef cell_node_interpolation_hpp
#define cell_node_interpolation_hpp


#include <vector>
#include <iostream>

#include "Sparse"
#include "mesh.hpp"
#include "node.hpp"
#include "face.hpp"
#include "cell.hpp"
#include "robin_BC.hpp"
#include "cell_field.hpp"
#include "node_field.hpp"


class cell_node_interpolation{
public:
    typedef std::vector<double> vector_of_double;
    typedef std::vector<bool> vector_of_bool;
    typedef std::vector<std::vector<double>> vector_of_vector_of_double;
    typedef std::vector<std::vector<int>> vector_of_vector_of_cells_indices;
    typedef std::vector<std::vector<face*>> vector_of_vector_of_faces_ptrs;
    typedef std::vector<node*> vector_of_nodes_ptrs;
    node_field interpolate(cell_field& u);
    void interpolate(cell_field& u, node_field& scalar_field);
    cell_node_interpolation(mesh& grid);
    ~cell_node_interpolation();
    
private:
    void compute_cell_weights();
    void compute_face_weights();
    void correct_boundary_nodes(cell_field& u, node_field& u_nodal);
    vector_of_vector_of_double cell_weights;
    vector_of_vector_of_double face_nodes_distances;
    vector_of_double nodes_faces_d_sum;
    mesh& grid;
};

#endif /* cell_node_interpolation_hpp */
