//
//  vtk_writer.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 05/08/2022.
//

#include "table_reader.hpp"

table_reader::table_reader(string file_name, int n_cols, bool header){
    
    std::fstream in_file;
    in_file.open(file_name);

    if(header == true){
        std::string name_;
        for(int i=0; i<n_cols; i++){
            in_file >> name_;
            header_.push_back(name_);
        }
    }
    else{
        header_=std::vector<std::string>(n_cols,"NO_HEADER");
    }

    while (!in_file.eof()) {
        std::vector<double> line(n_cols,-1);
        for(int i=0; i<n_cols; i++){
            in_file >> line[i];
        }
        table_.push_back(line);
    }
    table_.pop_back();
}