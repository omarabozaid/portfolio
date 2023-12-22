//
//  boundary_xyz_values_reader.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 10/08/2022.
//

#ifndef boundary_xyz_values_reader_hpp
#define boundary_xyz_values_reader_hpp

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

class boundary_xyz_values_reader{
public:
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    
    boundary_xyz_values_reader(string file_name);
    ~boundary_xyz_values_reader(){};
    vector_of_vector_of_doubles data();
    
private:
    vector_of_vector_of_doubles data_;
};

#endif /* boundary_xyz_values_reader_hpp */
