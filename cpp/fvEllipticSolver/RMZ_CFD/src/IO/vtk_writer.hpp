//
//  vtk_writer.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 05/08/2022.
//

#ifndef vtk_writer_hpp
#define vtk_writer_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mesh.hpp"
#include "cell_field.hpp"
#include "face_field.hpp"
#include "node_field.hpp"

class vtk_writer{
public:
    typedef std::string string;
    vtk_writer(mesh* grid, double time=0);
    vtk_writer(mesh* grid, std::vector<cell_field*> fields_ptr, double time=0);
    vtk_writer(mesh* grid, std::vector<cell_field*> cell_fields_ptr, std::vector<node_field*> node_fields_ptr, double time=0);
    ~vtk_writer();
    
private:
    mesh* grid_;
    void write_header(std::ofstream& case_file);
    void write_points(std::ofstream& case_file);
    void write_cells(std::ofstream& case_file);
    
    void write_cell_fields(std::ofstream& case_file, std::vector<cell_field*> fields_ptr);
    void write_node_fields(std::ofstream& case_file, std::vector<node_field*> fields_ptr);
    
    void write_cell_fields(mesh* grid, std::vector<cell_field*> fields_ptr,double time);
};
#endif /* vtk_writer_hpp */
