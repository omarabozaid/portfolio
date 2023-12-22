//
//  node.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 08/05/2022.
//

#ifndef node_hpp
#define node_hpp

#include <vector>
#include <iostream>
#include "point.hpp"

class cell;
class face;

class node{
public:
    node(point& pt, int index=-1, bool is_boundary_node_ = false);
    node(point& pt, std::vector<node*> neighbour_nodes_,int index=-1, bool is_boundary_node_ = false);
    node(point& pt, std::vector<node*> neighbour_nodes_,std::vector<cell*> neighbour_cells_,int index=-1, bool is_boundary_node_ = false);
    node(const node& another_node);
    ~node();
    
    void operator*=(double lambda);
    void operator/=(double lambda);
    void operator+=(node another_node);
    void operator-=(node another_node);
    friend node operator*(double lambda,node another_node);
    void append_neighbour_cell(cell* neighbour_cell);
    void append_neighbour_node(node* neighbour_node);
    void reset_index(int new_index);
    friend std::ostream& operator<<(std::ostream &os, node& nd);
    std::vector<node*> nodes_stencil();
    std::vector<cell*> cells_stencil();
    std::vector<face*> faces_stencil();
    void append(node* neighbouring_node);
    void append(face* neighbouring_face);
    void append(cell* neighbouring_cell);
    void set_nodes_stencil(std::vector<node*>);
    void set_cells_stencil(std::vector<cell*>);
    void set_faces_stencil(std::vector<face*>);
    void set_is_boundary_node(bool is_boundary);
    int index();
    bool is_boundary_node();
    point pt();
        
private:
    int idx_;
    point& pt_;
    bool is_boundary_node_;
    std::vector<node*> neighbouring_nodes_;
    std::vector<cell*> neighbouring_cells_;
    std::vector<face*> neighbouring_faces_;
};


#endif /* node_hpp */
