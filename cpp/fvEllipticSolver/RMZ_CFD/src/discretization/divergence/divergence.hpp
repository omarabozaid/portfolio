//
//  divergence.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 29/06/2022.
//

#ifndef divergence_hpp
#define divergence_hpp


#include "cell_field.hpp"
#include "cell_vector_field.hpp"
#include "face_field.hpp"
#include "face_vector_field.hpp"
#include "mesh.hpp"
#include "cell_face_interpolation.hpp"
#include "vector_operators_overloaded.hpp"


#include <vector>
#include <algorithm>
#include <math.h>


#include "Core"
#include "Sparse"


class divergence{
public:
    typedef Eigen::SparseMatrix<double,Eigen::RowMajor> sp_mat;
    typedef std::vector<Eigen::SparseMatrix<double>> vector_of_sp_mat;
    typedef Eigen::VectorXd eigen_vector;
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    
    divergence();
    ~divergence();
    
    cell_field div(face_field* flux_ptr,cell_field* field_ptr);
    cell_vector_field div(face_field* flux_ptr,cell_vector_field* field_ptr);
    sp_mat divergence_matrix(face_field* flux_ptr,cell_field* field_ptr);
    cell_field div(cell_vector_field* field_ptr);
};

#endif /* divergence_hpp */
