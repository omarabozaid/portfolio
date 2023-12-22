//
//  coordinates_system_conversions.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 04/08/2022.
//

#ifndef coordinates_system_conversions_hpp
#define coordinates_system_conversions_hpp

#include <vector>

namespace coordinates_system_conversions_tools{
    std::vector<double> spherical_from_cartesian(std::vector<double> cartesian_xyz);
    std::vector<double> spherical_from_cartesian(std::vector<double> cartesian_xyz, std::vector<double> normalization_xyz);
};

#endif /* coordinates_system_conversions_hpp */
