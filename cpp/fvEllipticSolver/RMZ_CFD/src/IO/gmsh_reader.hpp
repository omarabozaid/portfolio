//
//  gmsh_reader.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 10/07/2022.
//

#ifndef gmsh_reader_hpp
#define gmsh_reader_hpp

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "mesh.hpp"
#include "conforming_finite_volume_mesh.hpp"
#include "cell.hpp"
#include "face.hpp"
#include "node.hpp"
#include "point.hpp"
#include "cell_model.hpp"
#include "geom_tools.hpp"
#include "vector_operators_overloaded.hpp"

#include "Core"
#include "Dense"

class gmsh_reader{
public:
    typedef size_t label;
    typedef std::string string;
    typedef std::fstream fstream;
    typedef std::vector<size_t> label_list;
    typedef std::vector<label_list> list_of_label_list;
    typedef std::vector<double> vector_of_double;
    typedef std::vector<std::vector<double>> list_of_vector_of_doubles;
    typedef std::vector<std::string> vector_of_string;
    typedef std::vector<bool> vector_of_bool;
    typedef std::vector<point> points_list;
    typedef std::vector<node> nodes_list;
    typedef std::vector<face> faces_list;
    typedef std::vector<cell> cells_list;
    typedef std::vector<point*> points_ptrs_list;
    typedef std::vector<node*> nodes_ptrs_list;
    typedef std::vector<face*> faces_ptrs_list;
    typedef std::vector<cell*> cells_ptrs_list;
    
    typedef std::vector<std::map<label,label_list>> vector_label_label_list_map;
    typedef std::map<label_list,label> label_list_label_map;
    typedef std::map<label,label_list> label_label_list_map;
    typedef std::map<label,std::string> label_string_map;
    typedef std::map<std::string,label> string_label_map;
    typedef std::map<label,label> label_label_map;
    typedef std::map<label,faces_ptrs_list> physical_label_faces_ptrs;
    typedef std::map<string,faces_ptrs_list> physical_name_faces_ptrs;
    
    typedef conforming_finite_volume_mesh conf_fv_mesh;
    double version();
    
    list_of_vector_of_doubles points_coordinates_list();
    vector_of_string cell_shapes_list();
    list_of_label_list mesh_faces_as_indices();
    list_of_label_list mesh_cells_as_indices();
    label n_regions();
    label_list physical_types();
    label_list regions_I();
    label_label_map physical_label_physical_type();
    vector_of_string regions_names_list();
    label_list element_region_list();
    conf_fv_mesh& computational_grid();
    gmsh_reader(string file_name);
    ~gmsh_reader();
private:
    label MSHNOD  ;
    label MSHEDGE ;
    label MSHTRI  ;
    label MSHQUAD ;
    label MSHTET  ;
    label MSHPYR  ;
    label MSHPRISM;
    label MSHHEX  ;
    label n_faces ;
    label n_cells ;
    label n_nodes ;
    string file_name;
    fstream in_file;
    string nodes_section_begin_tag;
    string nodes_section_end_tag;
    string elements_section_begin_tag;
    string elements_section_end_tag;
    string mesh_format_section_begin_tag;
    string mesh_format_section_end_tag;
    label n_phys_regions;
    label_list phys_types;
    label_list regions_i;
    label_list faces_ordered_indices;
    vector_of_string regions_names;
    double mesh_format;
    list_of_vector_of_doubles points;
    label_list points_labels;
    label_list elements_surf_region;
    label_list elements_vol_region;
    label_list elements_region;
    list_of_label_list elements_surf;
    list_of_label_list elements_vol;
    list_of_label_list elements;
    vector_of_bool is_boundary_face;
    vector_of_bool is_boundary_cell;
    list_of_label_list owner_neighbour;
    list_of_label_list face_nodes_indices;
    list_of_label_list cell_faces_indices;
    list_of_label_list cell_nodes_indices;
    list_of_vector_of_doubles faces_normals;
    list_of_vector_of_doubles faces_cells_distances;
    vector_of_double face_area;
    vector_of_double cell_volume;
    list_of_label_list cell_face_outward_sign;
    points_list mesh_vertices_centers_;
    points_list face_centers_;
    points_list cell_centers_;
    nodes_list  mesh_vertices_;
    faces_list mesh_faces_;
    cells_list mesh_cells_;
    label_string_map physical_number_physical_names;
    string_label_map physical_names_physical_number;
    label_label_map physical_number_physical_type;
    label_list_label_map nodes_indices_face_index_map;
    label_label_list_map boundary_faces_map;
    faces_ptrs_list mesh_phys_faces;
    nodes_ptrs_list boundary_nodes;
    int n_boundary_nodes;
    int n_boundary_cells;
    int n_boundary_faces;
    int n_triangles_;
    int n_quads_;
    int n_edges_;
    int n_tetras_;
    int n_pyramids_;
    int n_prisms_;
    int n_hexas_;
    physical_label_faces_ptrs physical_label_physical_faces_ptrs;
    physical_name_faces_ptrs physical_name_physical_faces_ptrs;
    conf_fv_mesh* grid;
    void go_to_section(string section_tag);
    void skip_section(string section_tag);
    void read_mesh_format();
    void read_points_coordinates_list();
    void read_elements_list();
    void read_physical_names();
    void create_mesh_vertices();
    void create_mesh_faces();
    void create_faces_lookup();
    void create_mesh_cells();
    void create_mesh();
    void update_connectivity();
};


#endif /* gmsh_reader_hpp */
