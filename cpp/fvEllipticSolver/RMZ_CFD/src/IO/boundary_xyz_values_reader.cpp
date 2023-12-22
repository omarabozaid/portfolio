//
//  boundary_xyz_values_reader.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 10/08/2022.
//

#include "boundary_xyz_values_reader.hpp"

boundary_xyz_values_reader::boundary_xyz_values_reader(string file_name){
    std::fstream in_file;
    in_file.open(file_name);
    while (!in_file.eof()) {
        vector_of_doubles line(8,-1);
        for(auto& val:line){
            in_file >> val;
        }
        data_.push_back(line);
    }
    data_.pop_back();
    in_file.close();
}

boundary_xyz_values_reader::vector_of_vector_of_doubles boundary_xyz_values_reader::data(){
    return data_;
}
