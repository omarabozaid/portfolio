#include "edge.hpp"

edge::edge(
    node& node_start, 
    node& node_end,
    std::vector<face*> shared_faces,
    int index, 
    bool is_boundary_edge, 
    double length
    ):
    idx_(index),
    nodes_ptrs_({&node_start,&node_end}),
    shared_faces_ptrs_(shared_faces),
    length_(length),
    is_boundary_edge_(is_boundary_edge)
    {
        if(length_ == 0){
            compute_length();
        }
        check_degeneracy();
    }

edge::edge(
    node& node_start, 
    node& node_end,
    int index, 
    bool is_boundary_edge, 
    double length
    ):
    idx_(index),
    nodes_ptrs_({&node_start,&node_end}),
    length_(length),
    is_boundary_edge_(is_boundary_edge)
    {
        if(length_ == 0){
            compute_length();
        }
        check_degeneracy();
    }

void edge::reset_index(int new_index){
    idx_=new_index;
}

int edge::index(){
    return idx_;
}

std::vector<face*> edge::faces_stencil(){
    return shared_faces_ptrs_;
}

void edge::append(face* neighbouring_face){
    shared_faces_ptrs_.push_back(neighbouring_face);
}

void edge::set_faces_stencil(std::vector<face*> faces_list){
    shared_faces_ptrs_ = faces_list;
}

void edge::set_is_boundary_edge(bool is_boundary){
    is_boundary_edge_ = is_boundary;
}

bool edge::is_boundary_edge(){
    return is_boundary_edge_;
}

double edge::length(){
    return length_;
}

node* edge::start_node_ptr(){
    return nodes_ptrs_[0];
}

node* edge::end_node_ptr(){
    return nodes_ptrs_[1];
}
node& edge::start_node_ref(){
    return *nodes_ptrs_[0];
}

node& edge::end_node_ref(){
    return *nodes_ptrs_[1];
}

void edge::compute_length(){
    auto pt1 = nodes_ptrs_[0]->pt().coordinates();
    auto pt2 = nodes_ptrs_[1]->pt().coordinates();
    auto e = pt2-pt1;
    length_ = magnitude(e);
}

void edge::check_degeneracy(){
    if(length_<1e-5){
        is_degenerate_edge_ = true;
    }
}

bool edge::is_degenerate_edge(){
    return is_degenerate_edge_;
}