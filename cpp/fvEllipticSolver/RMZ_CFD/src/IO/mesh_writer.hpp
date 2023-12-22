//
//  mesh_writer.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 23/06/2022.
//

#ifndef mesh_writer_hpp
#define mesh_writer_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "mesh.hpp"
#include "cell_field.hpp"
#include "face_field.hpp"

class mesh_writer{
public:
    typedef std::string string;
    mesh_writer(mesh* grid,double time=0);
    mesh_writer(mesh* grid, std::vector<cell_field*> fields_ptr,double time=0);
    mesh_writer(mesh* grid, std::vector<face_field*> fields_ptr,double time=0);
    mesh_writer(mesh* grid, std::vector<cell_field*> cell_fields_ptr, std::vector<face_field*> face_fields_ptr,double time=0);
    ~mesh_writer();
    
    void write_mesh(string dir);
    void write_faces();
    void dump_faces_details(string dir);
    void write_vertices();
    void write_cells();
    
private:
    mesh* grid_;
    void write_cell_fields(mesh* grid, std::vector<cell_field*> fields_ptr,double time);
    void write_face_fields(mesh* grid, std::vector<face_field*> fields_ptr,double time);
};
#endif /* mesh_writer_hpp */
