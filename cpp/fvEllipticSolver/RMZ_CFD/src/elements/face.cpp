//
//  face.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 19/06/2022.
//

#include "face.hpp"
#include "cell.hpp"

#include<iostream>

face::face(const face& f):
index_(f.index_),
is_boundary_face_(f.is_boundary_face_),
is_internal_face_(f.is_internal_face_),
area_(f.area_),
d_owner_neighbour_(f.d_owner_neighbour_),
normal_area_(f.normal_area_),
face_center_(f.face_center_),
normal_(f.normal_),
face_nodes_(f.face_nodes_),
owner_neighbour_(f.owner_neighbour_),
distances_(f.distances_)
{}

face::face(point& center, int index, bool is_boundary_face,face::vector_of_doubles distances, double area):
face_center_(center),
index_(index),
is_boundary_face_(is_boundary_face),
is_internal_face_(!is_boundary_face),
distances_(distances),
area_(area)
{
}

face::face(point& center, face::vector_of_pointers_to_nodes face_nodes,int index, bool is_boundary_face,face::vector_of_doubles distances, double area):
face_center_(center),
face_nodes_(face_nodes),
index_(index),
is_boundary_face_(is_boundary_face),
is_internal_face_(!is_boundary_face),
distances_(distances),
area_(area)
{
}

face::face(point& center, face::vector_of_pointers_to_nodes face_nodes,face::vector_of_doubles normal, int index, bool is_boundary_face,face::vector_of_doubles weights, double area):
face_center_(center),
face_nodes_(face_nodes),
normal_(normal),
index_(index),
is_boundary_face_(is_boundary_face),
is_internal_face_(!is_boundary_face),
distances_(weights),
area_(area)
{
}

face::face(point& center, face::vector_of_pointers_to_nodes face_nodes,vector_of_pointers_to_cells owner_neighbour,int index, bool is_boundary_face,face::vector_of_doubles distances, double area):
face_center_(center),
face_nodes_(face_nodes),
owner_neighbour_(owner_neighbour),
index_(index),
is_boundary_face_(is_boundary_face),
is_internal_face_(!is_boundary_face),
distances_(distances),
area_(area)
{
}

face::face(point& center, vector_of_pointers_to_nodes face_nodes, vector_of_doubles normal, vector_of_pointers_to_cells owner_neighbour, int index, bool is_boundary_face,face::vector_of_doubles distances, double area):
face_center_(center),
face_nodes_(face_nodes),
owner_neighbour_(owner_neighbour),
normal_(normal),
index_(index),
is_boundary_face_(is_boundary_face),
is_internal_face_(!is_boundary_face),
distances_(distances),
area_(area)
{
}

face::~face()
{}

void face::append(node* face_node){
    face_nodes_.push_back(face_node);
}

void face::append(cell* shared_cell){
    owner_neighbour_.push_back(shared_cell);
}

void face::reset_index(int index){
    index_=index;
}

bool face::is_boundary_face(){
    return is_boundary_face_;
}

bool face::is_internal_face(){
    return is_internal_face_;
}

const point& face::face_center(){
    return face_center_;
}

face::vector_of_doubles face::normal(){
    return normal_;
}

int face::index(){
    return index_;
}

face::vector_of_pointers_to_cells face::owner_neighbour(){
    return owner_neighbour_;
}

double face::area(){
    return area_;
}

face::vector_of_doubles face::normal_area(){
    return normal_area_;
}

double& face::area_ref(){
    return area_;
}


void face::compute_distances(){
    face::vector_of_doubles x_face=face_center_.coordinates();
    face::vector_of_doubles x_cell=owner_neighbour_[0]->cell_center().coordinates();
    face::vector_of_doubles d_face_cell=x_face-x_cell;
    distances_[0]=magnitude(d_face_cell);
    if(!is_boundary_face_){
        x_cell=owner_neighbour_[1]->cell_center().coordinates();
        d_face_cell=x_face-x_cell;
        distances_[1]=magnitude(d_face_cell);
    }
    d_owner_neighbour_=distances_[0]+distances_[1];
}

double face::d_owner_neighbour(){
    return d_owner_neighbour_;
}

face::vector_of_doubles face::distances(){
    return distances_;
}

double* face::area_ptr(){
    return &area_;
}

void face::set_area(double area){
    area_=area;
}

void face::set_is_boundary(){
    is_boundary_face_=true;
    is_internal_face_=false;
}

void face::set_is_internal(){
    is_boundary_face_=false;
    is_internal_face_=true;
}

face::vector_of_pointers_to_nodes face::face_nodes(){
    return face_nodes_;
}

void face::swap(face &f){
    // face center is a ref variable --> face objects are not changable then
    // future: change face-center to be a pointer to a point not a ref value
    std::cout<<"Not implemented "<<std::endl;
}
