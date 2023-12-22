#include "export_gmsh.hpp"

export_gmsh::export_gmsh(
    string file_name,
    list_of_vector_of_doubles& points,
    list_of_vector_of_labels& cells,
    vector_of_labels& cells_regions,
    list_of_vector_of_labels& boundary_faces,
    vector_of_labels& faces_regions,
    label_string_map& phys_label_phys_name,
    label_label_map& phys_label_phys_type
    ):
    file_name_(file_name),
    points_(points),
    cells_(cells),
    cells_regions_(cells_regions),
    boundary_faces_(boundary_faces),
    faces_regions_(faces_regions),
    phys_label_phys_name_(phys_label_phys_name),
    phys_label_phys_type_(phys_label_phys_type)
    {
    out_file_.open(file_name_, std::ofstream::out | std::ofstream::trunc);    
    this->write_header();
    this->write_phys_names();
    this->write_points();
    this->write_elements();
    out_file_.close();
}

void export_gmsh::write_header(){
    out_file_<<"$MeshFormat\n2.2 0 8\n$EndMeshFormat\n";
}

void export_gmsh::write_phys_names(){
    int n_phys_names_ = 0;
    for(auto pair:phys_label_phys_name_){
        n_phys_names_+=1;
    }
    out_file_<<"$PhysicalNames\n"<<n_phys_names_<<"\n";
    for(auto pair:phys_label_phys_name_){
        auto phys_label = pair.first;
        auto phys_name = pair.second;
        auto phys_type = phys_label_phys_type_.find(phys_label)->second;
        out_file_<<phys_type<<" "<<phys_label<<" "<<phys_name<<"\n";
    }
    out_file_<<"$EndPhysicalNames\n";
}

void export_gmsh::write_points(){
    out_file_<<"$Nodes\n"<<points_.size()<<"\n";
    int counter = 0;
    for(auto pt:points_){
        out_file_<<counter<<" "<<pt[0]<<" "<<pt[1]<<" "<<pt[2]<<"\n";
        counter+=1;
    }
    out_file_<<"$EndNodes\n";
}

void export_gmsh::write_elements(){
    auto n_elements = cells_.size()+boundary_faces_.size();
    out_file_<<"$Elements\n"<<n_elements<<"\n";
    this->write_faces();
    this->write_cells();
    out_file_<<"$EndElements\n";
}

void export_gmsh::write_faces(){
    int counter = 0;
    for(auto elem:boundary_faces_){
        int elem_type = -1;
        if(elem.size()==3){
            elem_type=2;
        }
        else{
            elem_type=3;
        }
        out_file_<<counter<<" "<<elem_type<<" 1 "<<faces_regions_[counter]<<" ";
        for(auto idx:elem){
            out_file_<<idx<<" ";
        }
        out_file_<<"\n";
        counter+=1;
    }
}

void export_gmsh::write_cells(){
    int counter = boundary_faces_.size();
    int n_boundary_faces = counter;
    for(auto elem:cells_){
        int elem_type = -1;
        if(elem.size()==4){
            elem_type=4;
        }
        else if(elem.size()==5){
            elem_type=7;
        }
        else if(elem.size()==8){
            elem_type=5;
        }
        else if(elem.size()==6){
            elem_type=6;
        }
        out_file_<<counter<<" "<<elem_type<<" 1 "<<cells_regions_[counter-n_boundary_faces]<<" ";
        for(auto idx:elem){
            out_file_<<idx<<" ";
        }
        out_file_<<"\n";
        counter+=1;
    }
}