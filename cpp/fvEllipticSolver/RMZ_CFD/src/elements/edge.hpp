//
//  edge.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 08/05/2022.
//

#ifndef edge_hpp
#define edge_hpp

#include <vector>
#include <iostream>
#include "point.hpp"
#include "node.hpp"
#include"vector_operators_overloaded.hpp"

class face;

class edge{
public:
    edge(
        node& node_start, 
        node& node_end,
        std::vector<face*> shared_faces,
        int index=-1, 
        bool is_boundary_edge = false, 
        double length=0.0
        );
    
    edge(
        node& node_start, 
        node& node_end,
        int index=-1, 
        bool is_boundary_edge = false, 
        double length=0.0
        );

    ~edge(){};
    
    void reset_index(int new_index);
    std::vector<face*> faces_stencil();
    void append(face* neighbouring_face);
    void set_faces_stencil(std::vector<face*> faces_list);
    void set_is_boundary_edge(bool is_boundary);
    int index();
    bool is_boundary_edge();
    double length();
    node* start_node_ptr();
    node* end_node_ptr();
    node& start_node_ref();
    node& end_node_ref();
    bool is_degenerate_edge();
        
private:
    int idx_;
    bool is_boundary_edge_;
    std::vector<node*> nodes_ptrs_;
    std::vector<face*> shared_faces_ptrs_;
    double length_;
    bool is_degenerate_edge_;
    void compute_length();
    void check_degeneracy();    
};


#endif /* node_hpp */
