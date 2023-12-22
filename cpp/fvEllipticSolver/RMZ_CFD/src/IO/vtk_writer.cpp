//
//  vtk_writer.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 05/08/2022.
//

#include "vtk_writer.hpp"


vtk_writer::vtk_writer(mesh* grid,double time):
grid_(grid)
{}

vtk_writer::~vtk_writer()
{}


vtk_writer::vtk_writer(mesh* grid, std::vector<cell_field*> fields_ptr,double time):
grid_(grid)
{
    std::ofstream case_file;
    string case_file_name = ("test_case_cell_"+std::to_string(time)+".vtk");
    case_file.open(case_file_name, std::ofstream::out | std::ofstream::trunc);
    write_header(case_file);
    write_points(case_file);
    write_cells(case_file);
    write_cell_fields(case_file,fields_ptr);
    case_file.close();
}

vtk_writer::vtk_writer(mesh* grid,
                       std::vector<cell_field*> cell_fields_ptr,
                       std::vector<node_field*> node_fields_ptr,
                       double time
                       ):
grid_(grid)
{
    std::ofstream case_file;
    string case_file_name = ("test_case_cell_"+std::to_string(time)+".vtk");
    case_file.open(case_file_name, std::ofstream::out | std::ofstream::trunc);
    write_header(case_file);
    write_points(case_file);
    write_cells(case_file);
    write_node_fields(case_file, node_fields_ptr);
    write_cell_fields(case_file,cell_fields_ptr);
    case_file.close();
}


void vtk_writer::write_header(std::ofstream& case_file){
    case_file<<"# vtk DataFile Version 2.0 \n"<<"untitled, Created by RMZ v1\n"<<"ASCII\n"<<"DATASET UNSTRUCTURED_GRID\n";
}

void vtk_writer::write_points(std::ofstream& case_file){
    auto n_pts = grid_->n_vertices();
    case_file<<"POINTS "<<n_pts<<" double\n";
    for(auto vertex:grid_->nodes_list()){
        auto xyz = vertex.pt().coordinates();
        for(auto x:xyz){
            case_file<<x<<" ";
        }
        case_file<<"\n";
    }
}

void vtk_writer::write_cells(std::ofstream& case_file){
    auto n_cells = grid_->n_cells();
    auto n_boundary_faces = grid_->n_boundary_faces();
    auto n_triangles = grid_->n_triangles();
    auto n_quads = grid_->n_quads();
    auto n_tetras = grid_->n_tetras();
    auto n_pyramids = grid_->n_pyramids();
    auto n_prisms = grid_->n_prisms();
    auto n_hexas = grid_->n_hexas();
    auto n_entries = int((n_cells*1+n_tetras*4+n_pyramids*5+n_prisms*6+n_hexas*8) + (n_boundary_faces+n_quads*4+n_triangles*3));
    case_file<<"CELLS "<<int(n_cells+n_boundary_faces)<<" "<<n_entries<<"\n";
    for(auto& f:grid_->boundary_faces_list()){
        auto face_nodes_ptrs = f->face_nodes();
        case_file<<face_nodes_ptrs.size()<<" ";
        for(auto& node_ptr:face_nodes_ptrs){
            case_file<<node_ptr->index()<<" ";
        }
        case_file<<"\n";
    }
    for(auto c:grid_->cells_list()){
        case_file<<c.cell_nodes().size()<<" ";
        for(auto n:c.cell_nodes()){
            case_file<<n->index()<<" ";
        }
        case_file<<"\n";
    }
    case_file<<"CELL_TYPES "<<int(n_cells+n_boundary_faces)<<"\n";
    for(auto& f:grid_->boundary_faces_list()){
        if(f->face_nodes().size()==4){
            case_file<<"9\n";
        }
        else if(f->face_nodes().size()==3){
            case_file<<"5\n";
        }
        else{
            std::cout<<"Error non-vtk-type-element";
            case_file<<"100\n";
        }
    }
    for(auto c:grid_->cells_list()){
        if(c.cell_nodes().size()==4){
            case_file<<"10\n";
        }
        else if(c.cell_nodes().size()==5){
            case_file<<"14\n";
        }
        else if(c.cell_nodes().size()==6){
            case_file<<"13\n";
        }
        else if(c.cell_nodes().size()==8){
            case_file<<"12\n";
        }
        else{
            std::cout<<"Error non-vtk-type-element";
            case_file<<"100\n";
        }
    }
}

void vtk_writer::write_cell_fields(std::ofstream& case_file, std::vector<cell_field*> fields_ptr){
    case_file<<"CELL_DATA "<<int(grid_->n_cells()+grid_->n_boundary_faces())<<std::endl;
    for(auto field_ptr:fields_ptr){
        auto field_name = field_ptr->field_name();
        case_file<<"SCALARS "<<field_name<<" double 1\n";
        case_file<<"LOOKUP_TABLE default"<<std::endl;
        auto& field_vals = field_ptr->field_values_ref();
        auto& boundary_field_vals = field_ptr->field_boundary_values_ref();
        for(auto val:boundary_field_vals){
            case_file<<val<<std::endl;
        }
        for(auto val:field_vals){
            case_file<<val<<std::endl;
        }
    }
    string field_name = "is_face";
    case_file<<"SCALARS "<<field_name<<" double 1\n";
    case_file<<"LOOKUP_TABLE default"<<std::endl;
    auto n_boundary_faces = grid_->n_boundary_faces();
    auto is_face = std::vector<int>(n_boundary_faces,1);
    for(auto& check:is_face){
        case_file<<check<<std::endl;
    }
    auto n_cells = grid_->n_cells();
    is_face = std::vector<int>(n_cells,0);
    for(auto& check:is_face){
        case_file<<check<<std::endl;
    }
}

void vtk_writer::write_node_fields(std::ofstream& case_file, std::vector<node_field*> fields_ptr){
    case_file<<"POINT_DATA "<<grid_->n_vertices()<<std::endl;
    for(auto& field_ptr:fields_ptr){
        std::string field_name = field_ptr->field_name();
        case_file<<"SCALARS "<<field_name<<" double 1\n";
        case_file<<"LOOKUP_TABLE default"<<std::endl;
        auto& nodal_values = field_ptr->nodal_values_ref();
        for(auto val:nodal_values){
            case_file<<val<<std::endl;
        }
    }
}
