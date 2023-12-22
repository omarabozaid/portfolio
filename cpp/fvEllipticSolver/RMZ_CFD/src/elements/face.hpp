//
//  face.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 19/06/2022.
//

#ifndef face_hpp
#define face_hpp

#include <vector>
#include "node.hpp"
#include "point.hpp"

#include "vector_operators_overloaded.hpp"

class cell;

class face{
public:
    
    typedef std::vector<node*> vector_of_pointers_to_nodes;
    typedef std::vector<cell*> vector_of_pointers_to_cells;
    typedef std::vector<int> vector_of_ints;
    typedef std::vector<double> vector_of_doubles;
    
    face(const face& f);
    
    face(point& center, int index=-1, bool is_boundary_face=false, vector_of_doubles distances=vector_of_doubles(2,0), double area=0);
    
    face(point& center, vector_of_pointers_to_nodes face_nodes,int index=-1, bool is_boundary_face=false, vector_of_doubles distances=vector_of_doubles(2,0), double area=0);
    
    face(
         point& center,
         vector_of_pointers_to_nodes face_nodes,
         vector_of_doubles normal,
         int index=-1,
         bool is_boundary_face=false,
         vector_of_doubles distances=vector_of_doubles(2,0),
         double area=0
         );
    
    face(point& center, vector_of_pointers_to_nodes face_nodes,vector_of_pointers_to_cells owner_neighbour,int index=-1, bool is_boundary_face=false, vector_of_doubles distances=vector_of_doubles(2,0), double area=0);
    
    face(point& center, vector_of_pointers_to_nodes face_nodes, vector_of_doubles normal, vector_of_pointers_to_cells owner_neighbour, int index=-1, bool is_boundary_face=false, vector_of_doubles distances=vector_of_doubles(2,0), double area=0);
    
    ~face();
    
    void append(node* face_node);
    void append(cell* shared_cell);
    void reset_index(int index);
    bool is_boundary_face();
    bool is_internal_face();
    const point& face_center();
    vector_of_doubles normal();
    int index();
    vector_of_pointers_to_cells owner_neighbour();
    vector_of_doubles distances();
    double d_owner_neighbour();
    double area();
    vector_of_doubles normal_area();
    double& area_ref();
    double* area_ptr();
    void set_area(double area);
    void set_is_boundary();
    void set_is_internal();
    void swap(face& f);
    vector_of_pointers_to_nodes face_nodes();

private:
   
    int index_;
    bool is_boundary_face_;
    bool is_internal_face_;
    double area_;
    double d_owner_neighbour_;
    vector_of_doubles normal_area_;
    point& face_center_;
    vector_of_doubles normal_;
    vector_of_pointers_to_nodes face_nodes_;
    vector_of_pointers_to_cells owner_neighbour_;
    vector_of_doubles distances_;
    void compute_distances();
    
};

#endif /* face_hpp */
