//
//  node.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 08/05/2022.
//

#include "node.hpp"
#include "cell.hpp"
#include "face.hpp"


node::node(point& pt,int index, bool is_boundary_node_):
pt_(pt),
idx_(index),
is_boundary_node_(is_boundary_node_)
{};

node::node(point& pt, std::vector<node*> neighbouring_nodes_,int index, bool is_boundary_node_):
pt_(pt),
neighbouring_nodes_(neighbouring_nodes_),
idx_(index),
is_boundary_node_(is_boundary_node_)
{};


node::node(point& pt, std::vector<node*> neighbouring_nodes_,std::vector<cell*> neighbour_cells_,int index, bool is_boundary_node_):
pt_(pt),
neighbouring_nodes_(neighbouring_nodes_),
neighbouring_cells_(neighbour_cells_),
idx_(index),
is_boundary_node_(is_boundary_node_)
{};

node::node(const node& another_node):
pt_(another_node.pt_),
neighbouring_nodes_(another_node.neighbouring_nodes_),
neighbouring_cells_(another_node.neighbouring_cells_),
idx_(another_node.idx_)
{
};

node::~node()
{};

void node::append_neighbour_node(node* neighbour_node){
    neighbouring_nodes_.push_back(neighbour_node);
};

void node::append_neighbour_cell(cell* neighbour_cell){
    neighbouring_cells_.push_back(neighbour_cell);
};

void node::reset_index(int new_index){
    idx_=new_index;
};

std::vector<node*> node::nodes_stencil(){
    return neighbouring_nodes_;
};

std::vector<cell*> node::cells_stencil(){
    return neighbouring_cells_;
};

std::ostream& operator<<(std::ostream &os, node& nd){
    os<<"Node index "<<nd.idx_<<", Position -> "<<nd.pt_<<"\n";
    return os;
};

point node::pt(){
    return pt_;
}

void node::operator*=(double lambda){
    pt_=pt_*lambda;
};

void node::operator/=(double lambda){
    this->operator*=(double(1.0/lambda));
};

void node::operator+=(node another_node){
    pt_+=another_node.pt_;
};

void node::operator-=(node another_node){
    pt_-=another_node.pt_;
};

int node::index(){
    return idx_;
}


std::vector<face*> node::faces_stencil(){
    return neighbouring_faces_;
}

void node::set_nodes_stencil(std::vector<node*> nodes_stencil){
    neighbouring_nodes_= nodes_stencil;
}

void node::set_cells_stencil(std::vector<cell*> cells_stencil){
    neighbouring_cells_ = cells_stencil;
}

void node::set_faces_stencil(std::vector<face*> faces_stencil){
    neighbouring_faces_ = faces_stencil;
}

void node::set_is_boundary_node(bool is_boundary){
    is_boundary_node_ = is_boundary;
}

bool node::is_boundary_node(){
    return is_boundary_node_;
}

void node::append(node* neighbouring_node){
    neighbouring_nodes_.push_back(neighbouring_node);
}

void node::append(face* neighbouring_face){
    neighbouring_faces_.push_back(neighbouring_face);
}

void node::append(cell* neighbouring_cell){
    neighbouring_cells_.push_back(neighbouring_cell);
}
