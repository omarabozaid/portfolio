//
//  cell.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 09/05/2022.
//

#include "cell.hpp"

cell::cell(point& center,int index, bool is_boundary_cell):
cell_center_(center),
index_(index),
is_boundary_cell_(is_boundary_cell),
is_internal_cell_(!is_boundary_cell)
{}

cell::cell(point& center, cell::vector_of_pointers_to_nodes cell_nodes,int index, double vol, vector_of_doubles sum_d, bool is_boundary_cell):
cell_center_(center),
cell_nodes_(cell_nodes),
index_(index),
is_boundary_cell_(is_boundary_cell),
is_internal_cell_(!is_boundary_cell),
vol_(vol),
sum_d_to_faces_(sum_d)
{}

cell::cell(point& center, vector_of_pointers_to_nodes cell_nodes,vector_of_pointers_to_faces cell_faces,int index, double vol, bool is_boundary_cell):
cell_center_(center),
cell_nodes_(cell_nodes),
cell_faces_(cell_faces),
index_(index),
is_boundary_cell_(is_boundary_cell),
is_internal_cell_(!is_boundary_cell),
vol_(vol)
{
}

cell::cell(point& center, cell::vector_of_pointers_to_nodes cell_nodes, cell::vector_of_pointers_to_faces cell_faces, int index, double vol, vector_of_doubles sum_d, bool is_boundary_cell):
cell_center_(center),
cell_nodes_(cell_nodes),
cell_faces_(cell_faces),
index_(index),
is_boundary_cell_(is_boundary_cell),
is_internal_cell_(!is_boundary_cell),
faces_normals_signs_(cell_faces.size(),1),
vol_(vol),
sum_d_to_faces_(sum_d)
{
}

cell::cell(point& center, vector_of_pointers_to_nodes cell_nodes,vector_of_pointers_to_faces cell_faces,vector_of_pointers_to_cells neighbouring_cells,int index, double vol, vector_of_doubles sum_d, bool is_boundary_cell):
cell_center_(center),
cell_nodes_(cell_nodes),
cell_faces_(cell_faces),
index_(index),
is_boundary_cell_(is_boundary_cell),
neighbouring_cells_(neighbouring_cells),
faces_normals_signs_(cell_faces.size(),1),
vol_(vol),
sum_d_to_faces_(sum_d)
{
}

cell::~cell()
{}

void cell::reset_index(int index){
    index_=index;
}

void cell::append(node* shared_node){
    cell_nodes_.push_back(shared_node);
}

void cell::append(face* shared_face){
    cell_faces_.push_back(shared_face);
    faces_normals_signs_.push_back(1);
    if(!shared_face->is_boundary_face()){
        if(shared_face->owner_neighbour()[0]==this){
            neighbouring_cells_.push_back(shared_face->owner_neighbour()[1]);
        }
        else{
            neighbouring_cells_.push_back(shared_face->owner_neighbour()[0]);
        }
    }
    adjust_faces_normals();
}

void cell::append(face* shared_face,int sign){
    cell_faces_.push_back(shared_face);
    faces_normals_signs_.push_back(sign);
}

void cell::append(cell* neighbour_cell){
    neighbouring_cells_.push_back(neighbour_cell);
}

bool cell::is_boundary_cell(){
    return is_boundary_cell_;
}

bool cell::is_internal_cell(){
    return is_internal_cell_;
}

void cell::connect_cell_to_nodes(){
    for(auto node_pointer:cell_nodes_){
        node_pointer->append_neighbour_cell(this);
    }
}

void cell::connect_cell_to_faces(){
    for(auto face_pointer:cell_faces_){
        face_pointer->append(this);
    }
    adjust_faces_normals();
}


void cell::adjust_faces_normals(){
    int counter=0;
    for(auto face_pointer:cell_faces_){
        point face_center = face_pointer->face_center();
        vector_of_doubles x_face=face_center.coordinates();
        vector_of_doubles x_cell=cell_center_.coordinates();
        vector_of_doubles n_cell = x_face-x_cell;
        n_cell/=(magnitude(n_cell));
        auto n_face=face_pointer->normal();
        double dot_product=dot(n_cell,n_face);
        if (dot_product<-1e-8){
            faces_normals_signs_[counter]=-1;
        }
    }
}


int cell::index(){
    return index_;
}

cell::vector_of_pointers_to_cells cell::neighbouring_cells(){
    return neighbouring_cells_;
}


cell::vector_of_pointers_to_nodes cell::cell_nodes(){
    return cell_nodes_;
}

cell::vector_of_ints cell::faces_normals_signs(){
    return faces_normals_signs_;
}

point& cell::cell_center(){
    return cell_center_;
}

/*
 FIX ME
 BUGGED FUNCTION
 */
void cell::re_order_faces(){
    vector_of_pointers_to_faces ordered_faces;
    for(auto c_ptr:neighbouring_cells_){
        for(auto f_ptr:cell_faces_){
            if(f_ptr->owner_neighbour().size()==2)
            {
                if(f_ptr->owner_neighbour()[0]->index()==c_ptr->index() || f_ptr->owner_neighbour()[1]->index()==c_ptr->index() ){
                    ordered_faces.push_back(f_ptr);
                    break;
                }
            }
        }
    }
    for(auto f_ptr:cell_faces_){
        if(f_ptr->is_boundary_face()==true)
        {
            ordered_faces.push_back(f_ptr);
        }
    }
    std::swap(cell_faces_,ordered_faces);
}

cell::vector_of_pointers_to_faces cell::neighbouring_faces(){
    return cell_faces_;
}

void cell::set_vol(double vol){
    vol_=vol;
}

double cell::vol(){
    return vol_;
}

cell::vector_of_doubles cell::sum_d_to_faces(){
    return sum_d_to_faces_;
}

void cell::set_sum_to_d_to_faces(vector_of_doubles directional_d){
    sum_d_to_faces_=directional_d;
}

void cell::set_neighbours(vector_of_pointers_to_cells stencil){
    neighbouring_cells_=stencil;
}
