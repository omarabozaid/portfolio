//
//  nodes_list_reader.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 11/05/2022.
//

#include "points_list_reader.hpp"

std::vector<point> parse_points_file(std::string file_name){
    std::vector<point> points_list;
    std::ifstream points_file(file_name);
    std::vector<double> pt(3,0.0);
    while (points_file >> pt[0] >> pt[1] >> pt[2])
      {
          points_list.push_back(point(pt));
      }
    return points_list;
};
