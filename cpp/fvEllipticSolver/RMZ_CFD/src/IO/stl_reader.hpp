//
//  stl_reader.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 26/08/2022.
//

#ifndef stl_reader_hpp
#define stl_reader_hpp

#include <iostream>
#include <string>
#include <vector>

#include "point.hpp"
#include "streiter_stl_reader.h"
#include "vector_operators_overloaded.hpp"

class stl_reader{
public:
    typedef std::string string;
    typedef std::vector<point> vector_of_points;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<vector_of_doubles> vector_of_vector_of_doubles;
    stl_reader(string file_name);
    ~stl_reader(){};
    vector_of_points points_list(){return points_list_;};
    vector_of_vector_of_doubles normals_list(){return normals_list_;};
    vector_of_points centers_list(){return centers_list_;};
private:
    vector_of_points points_list_;
    vector_of_points centers_list_;
    vector_of_vector_of_doubles normals_list_;
};

#endif /* stl_reader_hpp */
