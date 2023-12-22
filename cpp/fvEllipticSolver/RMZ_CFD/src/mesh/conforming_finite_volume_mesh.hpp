//
//  conforming_finite_volume_mesh.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 18/07/2022.
//

#ifndef conforming_finite_volume_mesh_hpp
#define conforming_finite_volume_mesh_hpp

#include <vector>
#include <string>
#include <map>

#include "mesh.hpp"
#include "point.hpp"
#include "node.hpp"
#include "cell.hpp"
#include "face.hpp"

class conforming_finite_volume_mesh:public mesh{
public:
    
    // Type defs and aliasing
    typedef std::string string;
    typedef std::vector<point> vector_of_points;
    typedef std::vector<node> vector_of_nodes;
    typedef std::vector<node*> vector_of_pointers_to_nodes;
    typedef std::vector<face> vector_of_faces;
    typedef std::vector<face*> vector_of_pointers_to_faces;
    typedef std::vector<cell> vector_of_cells;
    typedef std::vector<cell*> vector_of_pointers_to_cells;
    typedef std::map<size_t, vector_of_pointers_to_faces> physical_label_faces_ptrs;
    typedef std::map<std::string, vector_of_pointers_to_faces> physical_name_faces_ptrs;
    typedef std::map<size_t,size_t> label_label_map;
    typedef std::map<size_t,string> label_string_map;
    
    // Constructor/Destructor
    conforming_finite_volume_mesh(
              vector_of_cells& cells_list,
              vector_of_faces& faces_list,
              vector_of_pointers_to_faces single_cell_faces_list,
              vector_of_nodes& nodes_list,
              vector_of_points& points_list,
              vector_of_pointers_to_nodes& single_boundary_nodes_list,
              physical_label_faces_ptrs& physical_label_physical_faces_ptrs,
              physical_name_faces_ptrs& physical_name_physical_faces_ptrs,
              label_label_map physical_label_physical_type,
              label_string_map physical_label_physical_name,
              int n_boundary_nodes,
              int n_boundary_cells,
              int n_boundary_faces,
              int n_triangles,
              int n_quads,
              int n_edges,
              int n_tetras,
              int n_pyramids,
              int n_prisms,
              int n_hexas
              );
    ~conforming_finite_volume_mesh();
    
    vector_of_points mesh_vertices_centers(){return  points_list_;};;
    vector_of_faces faces_list(){return  faces_list_;};
    vector_of_cells cells_list(){return  cells_list_;};
    vector_of_nodes nodes_list() {return  nodes_list_;};
    vector_of_faces& faces_list_ref(){return  faces_list_;};;
    vector_of_cells& cells_list_ref(){return  cells_list_;};
    vector_of_nodes& nodes_list_ref(){return  nodes_list_;};
    
    int n_faces(){return n_faces_;};
    int n_cells(){return n_cells_;};
    int n_vertices(){return n_vertices_;};
    int n_boundary_faces(){return n_boundary_faces_;};
    int n_boundary_cells(){return n_boundary_cells_;};
    int n_boundary_nodes(){return n_boundary_nodes_;};
    
    int n_triangles(){return n_triangles_;};
    int n_quads(){return n_quads_;};
    int n_edges(){return n_edges_;};
    int n_tetras(){return n_tetras_;};
    int n_pyramids(){return n_pyramids_;};
    int n_prisms(){return n_prisms_;};
    int n_hexas(){return n_hexas_;};
    
    physical_label_faces_ptrs physical_label_physical_faces_ptrs(){return physical_label_physical_faces_ptrs_;};
    physical_name_faces_ptrs physical_name_physical_faces_ptrs(){return physical_name_physical_faces_ptrs_;};
    
    vector_of_pointers_to_nodes boundary_nodes_list(){return single_boundary_nodes_list_;};
    vector_of_pointers_to_faces boundary_faces_list(){return single_cell_faces_list_;};

    label_label_map physical_label_physical_type(){return physical_label_physical_type_;};
    label_string_map physical_label_physical_name(){return physical_label_physical_name_;};

    void handle_zero_volume_cells(double vol_tol=1e-10);
    
private:
    
    vector_of_cells& cells_list_;
    vector_of_faces& faces_list_;
    vector_of_pointers_to_faces single_cell_faces_list_;
    vector_of_pointers_to_nodes single_boundary_nodes_list_;
    vector_of_nodes& nodes_list_;
    vector_of_points& points_list_;
    physical_label_faces_ptrs& physical_label_physical_faces_ptrs_;
    physical_name_faces_ptrs& physical_name_physical_faces_ptrs_;
    label_label_map physical_label_physical_type_;
    label_string_map physical_label_physical_name_;
    
    int n_faces_;
    int n_cells_;
    int n_vertices_;
    int n_boundary_faces_;
    int n_boundary_cells_;
    int n_boundary_nodes_;
    
    int n_triangles_;
    int n_quads_;
    int n_edges_;
    int n_tetras_;
    int n_pyramids_;
    int n_prisms_;
    int n_hexas_;
    
};


#endif /* conforming_finite_volume_mesh_hpp */
