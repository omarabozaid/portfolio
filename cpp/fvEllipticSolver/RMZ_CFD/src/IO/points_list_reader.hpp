//
//  nodes_list_reader.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 11/05/2022.
//

#ifndef points_list_reader_hpp
#define points_list_reader_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "point.hpp"

std::vector<point> parse_points_file(std::string file_name);

#endif /* nodes_list_reader_hpp */
