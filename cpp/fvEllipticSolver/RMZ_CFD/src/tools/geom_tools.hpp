//
//  geom_tools.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 16/07/2022.
//

#ifndef geom_tools_hpp
#define geom_tools_hpp

#include <iostream>
#include <vector>
#include <numeric>

#include "Core"
#include "Dense"
#include "point.hpp"
#include "vector_operators_overloaded.hpp"
#include "cell_model.hpp"

typedef Eigen::Vector3f eigen_vector;
   
void area(std::vector<std::vector<double>> pts,  std::vector<double> center, double& area_value);
double area( std::vector<std::vector<double>> pts,  std::vector<double> center);
double area( std::vector<double> pt1,  std::vector<double> pt2,  std::vector<double> pt3);

std::vector<double> plane_normal( std::vector<std::vector<double>> pts);
std::vector<double> plane_center( std::vector<std::vector<double>> pts);
void fit_plane( std::vector<std::vector<double>> pts,std::vector<double>& normal,std::vector<double>& center);

/*
double poly_volume(
                    std::vector<std::vector<double>> faces_centers,
                    std::vector<std::vector<double>> faces_normals,    
                    std::vector<double> cell_center,
                    std::vector<double> areas,
                    int cell_type = -1
                    );
*/

double poly_volume(
                    std::vector<std::vector<double>> cell_pts,
                    int cell_type
                    );

double tetra_volume(
                std::vector<std::vector<double>> cell_pts
                );

double tetra_signed_volume(
                std::vector<std::vector<double>> cell_pts
                );

#endif /* geom_tools_hpp */
