//
//  cell_model.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 15/07/2022.
//

#ifndef cell_model_hpp
#define cell_model_hpp

#include <vector>
#include <algorithm>
#include <iostream>

class cell_model{
public:

    typedef std::vector<size_t> label_list;
    typedef std::vector<std::vector<size_t>> list_of_label_list;
    cell_model();
    ~cell_model();

    list_of_label_list faces_as_nodes_indices(label_list nodes_indices);
    list_of_label_list faces_as_nodes_indices_ordered_ascending(label_list nodes_indices);
    list_of_label_list tetrahedralize(label_list nodes_indices);
    list_of_label_list edges_as_nodes_indices(label_list nodes_indices);
    list_of_label_list edges_as_nodes_indices_ordered_ascending(label_list nodes_indices);

    list_of_label_list face_edges_as_nodes_indices_ordered_ascending(label_list nodes_indices);
    list_of_label_list face_edges_as_nodes_indices(label_list nodes_indices);

};
#endif /* cell_model_hpp */
