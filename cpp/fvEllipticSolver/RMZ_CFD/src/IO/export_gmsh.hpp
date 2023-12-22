//
//  export_gmsh.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 10/07/2022.
//

#ifndef export_gmsh_hpp
#define export_gmsh_hpp

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


class export_gmsh{
public:
    typedef size_t label;
    typedef std::vector<double> vector_of_double;
    typedef std::vector<std::vector<double>> list_of_vector_of_doubles;
    typedef std::vector<label> vector_of_labels;
    typedef std::vector<vector_of_labels> list_of_vector_of_labels;
    typedef std::string string;
    typedef std::ofstream ofstream;
    typedef std::map<label,string> label_string_map;
    typedef std::map<label,label> label_label_map;

    export_gmsh(
        string file_name,
        list_of_vector_of_doubles& points,
        list_of_vector_of_labels& cells,
        vector_of_labels& cells_regions,
        list_of_vector_of_labels& boundary_faces,
        vector_of_labels& faces_regions,
        label_string_map& phys_label_phys_name,
        label_label_map& phys_label_phys_type
        );
    ~export_gmsh(){};

private:
    ofstream out_file_;
    string file_name_;
    
    list_of_vector_of_doubles& points_;
    list_of_vector_of_labels& cells_;
    vector_of_labels& cells_regions_;
    list_of_vector_of_labels& boundary_faces_;
    vector_of_labels& faces_regions_;
    label_string_map& phys_label_phys_name_;
    label_label_map& phys_label_phys_type_;

    void write_header();
    void write_points();
    void write_phys_names();
    void write_elements();
    void write_cells();
    void write_faces();
    
};


#endif /* gmsh_reader_hpp */
