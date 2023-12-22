//
//  vtk_writer.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 05/08/2022.
//

#ifndef table_reader_hpp
#define table_reader_hpp


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class table_reader{
public:
    
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    

    table_reader(string file_name, int n_cols, bool header=false);
    ~table_reader(){};
    
    std::vector<std::string> header(){return header_;};
    std::vector<std::vector<double>> table(){return table_;};

private:
    std::vector<std::string> header_;
    std::vector<std::vector<double>> table_;


};
#endif /* table_reader_hpp */
