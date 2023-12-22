//
//  cell.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 09/05/2022.
//

#ifndef cell_hpp
#define cell_hpp

#include <vector>
#include "vector_operators_overloaded.hpp"
#include "point.hpp"
#include "node.hpp"
#include "face.hpp"

class field;
class face;

class cell{
public:
    
    typedef std::vector<node*> vector_of_pointers_to_nodes;
    typedef std::vector<face*> vector_of_pointers_to_faces;
    typedef std::vector<cell*> vector_of_pointers_to_cells;
    typedef std::vector<field*> vector_of_pointers_to_fields;
    typedef std::vector<int> vector_of_ints;
    typedef std::vector<double> vector_of_doubles;
    
    cell(point& center, int index=-1, bool is_boundary_cell=false);
    
    cell(point& center, vector_of_pointers_to_nodes cell_nodes,int index=-1, double vol=0, vector_of_doubles sum_d=vector_of_doubles(3,1e6), bool is_boundary_cell=false);
    
    cell(point& center, vector_of_pointers_to_nodes cell_nodes,vector_of_pointers_to_faces cell_faces,int index=-1, double vol=0, bool is_boundary_cell=false);
    
    cell(point& center, vector_of_pointers_to_nodes cell_nodes,vector_of_pointers_to_faces cell_faces,int index=-1, double vol=0, vector_of_doubles sum_d=vector_of_doubles(3,1e6), bool is_boundary_cell=false);
    
    cell(point& center, vector_of_pointers_to_nodes cell_nodes,vector_of_pointers_to_faces cell_faces,vector_of_pointers_to_cells neighbouring_cells,int index=-1, double vol=0, vector_of_doubles sum_d =vector_of_doubles(3,1e6), bool is_boundary_cell=false);
    
    ~cell();
    
    void append(node* shared_node);
    void append(face* shared_face);
    void append(face* shared_face, int sign);
    void append(cell* neighbour_cell);
    bool is_boundary_cell();
    bool is_internal_cell();
    void reset_index(int index);
    void set_neighbours(vector_of_pointers_to_cells);
    void connect_cell_to_nodes();
    void connect_cell_to_faces();
    void adjust_faces_normals();
    void re_order_faces();
    
    int index();
    double vol();
    vector_of_doubles sum_d_to_faces();
    void set_vol(double vol);
    void set_sum_to_d_to_faces(vector_of_doubles directional_d);
    
    vector_of_pointers_to_cells neighbouring_cells();
    vector_of_pointers_to_faces neighbouring_faces();
    vector_of_pointers_to_nodes cell_nodes();
    vector_of_ints faces_normals_signs();
    point& cell_center();
    
private:
    int index_;
    point cell_center_;
    vector_of_pointers_to_nodes cell_nodes_;
    vector_of_pointers_to_faces cell_faces_;
    vector_of_pointers_to_cells neighbouring_cells_;
    vector_of_ints faces_normals_signs_;
    vector_of_pointers_to_fields pointers_to_cell_fields_;
    bool is_boundary_cell_;
    bool is_internal_cell_;
    double vol_;
    vector_of_doubles sum_d_to_faces_;
};

#endif /* cell_hpp */
