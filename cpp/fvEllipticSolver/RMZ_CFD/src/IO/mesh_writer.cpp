//
//  mesh_writer.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 23/06/2022.
//

#include "mesh_writer.hpp"

mesh_writer::mesh_writer(mesh* grid,double time):
grid_(grid)
{}

mesh_writer::~mesh_writer()
{}

void mesh_writer::dump_faces_details(string dir)
{
    auto faces_list = grid_->faces_list();
    std::ofstream indices_file_;
    std::ofstream centers_file_;
    std::ofstream normals_file_;
    
    string indices_file_name = (dir+"faces_indices.txt");
    string centers_file_name = (dir+"faces_centers.txt");
    string normals_file_name = (dir+"faces_normals.txt");
    
    indices_file_.open(indices_file_name, std::ofstream::out | std::ofstream::trunc);
    centers_file_.open(centers_file_name, std::ofstream::out | std::ofstream::trunc);
    normals_file_.open(normals_file_name, std::ofstream::out | std::ofstream::trunc);
    
    for(auto f:faces_list){
        auto index = f.index();
        auto x_f = (f.face_center()).coordinates();
        auto normal=f.normal();
        
        indices_file_<<index<<"\n";
        centers_file_<<x_f[0]<<" "<<x_f[1]<<" "<<x_f[2]<<"\n";
        normals_file_<<normal[0]<<" "<<normal[1]<<" "<<normal[2]<<"\n";
    }
    
    indices_file_.close();
    centers_file_.close();
    normals_file_.close();
    
    
}

mesh_writer::mesh_writer(mesh* grid, std::vector<cell_field*> fields_ptr,double time){
    write_cell_fields(grid,fields_ptr,time);
}

mesh_writer::mesh_writer(mesh* grid, std::vector<face_field*> fields_ptr,double time){
    write_face_fields(grid,fields_ptr,time);
}

mesh_writer::mesh_writer(mesh* grid, std::vector<cell_field*> cell_fields_ptr, std::vector<face_field*> face_fields_ptr,double time){
    write_cell_fields(grid,cell_fields_ptr,time);
    write_face_fields(grid,face_fields_ptr,time);
}

void mesh_writer::write_face_fields(mesh* grid, std::vector<face_field*> fields_ptr,double time){
    auto face_list = grid->faces_list();
    std::ofstream case_file_;
    string case_file_name = ("./test_case_face.csv");
    case_file_.open(case_file_name, std::ofstream::out | std::ofstream::trunc);
    case_file_<<"x,y,z,";
    for(auto ptr:fields_ptr){
        case_file_<<ptr->field_name()<<",";
    }
    case_file_<<"NONE\n";
    for(auto f:face_list){
        auto x_f = f.face_center().coordinates();
        case_file_<<x_f[0]<<","<<x_f[1]<<","<<x_f[2]<<",";
        for(auto ptr:fields_ptr){
            case_file_<<ptr->field_values()[f.index()]<<",";
        }
        case_file_<<"0\n";
        }
    
    case_file_.close();
}


void mesh_writer::write_cell_fields(mesh* grid, std::vector<cell_field*> fields_ptr,double time){
    auto cells_list = grid->cells_list();
    std::ofstream case_file_;
    string case_file_name = ("test_case_cell_"+std::to_string(time)+".csv");
    case_file_.open(case_file_name, std::ofstream::out | std::ofstream::trunc);
    case_file_<<"x,y,z,";
    for(auto ptr:fields_ptr){
        case_file_<<ptr->field_name()<<",";
    }
    case_file_<<"NONE\n";
    for(auto c:cells_list){
        auto x_c = c.cell_center().coordinates();
        case_file_<<x_c[0]<<","<<x_c[1]<<","<<x_c[2]<<",";
        for(auto ptr:fields_ptr){
            case_file_<<ptr->field_values()[c.index()]<<",";
        }
        case_file_<<"0\n";
        }
    case_file_.close();
}
