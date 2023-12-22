//
//  cell_face_interpolation.hpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 28/08/2022.
//

#ifndef cell_face_interpolation_hpp
#define cell_face_interpolation_hpp


#include <vector>
#include <string>

#include "face.hpp"
#include "mesh.hpp"
#include "cell_field.hpp"
#include "face_field.hpp"
#include "cell_vector_field.hpp"
#include "face_vector_field.hpp"
#include "robin_BC.hpp"

class cell_face_interpolation{
public:
    
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    
    cell_face_interpolation(){};
    ~cell_face_interpolation(){};
    face_field interpolate(cell_field* scalar_field);
    face_vector_field interpolate(cell_vector_field* vector_field);

};
#endif /* cell_face_interpolation_hpp */
