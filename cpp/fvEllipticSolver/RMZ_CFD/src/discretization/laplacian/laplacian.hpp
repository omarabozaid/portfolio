//
//  laplacian.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 26/06/2022.
//

#ifndef laplacian_hpp
#define laplacian_hpp

#include <vector>
#include <iostream>

#include "Sparse"
#include "mesh.hpp"
#include "cell_field.hpp"
#include "cell_vector_field.hpp"

class laplacian{
public:
    
    typedef Eigen::SparseMatrix<double,Eigen::RowMajor> sp_mat;
    typedef Eigen::VectorXd eigen_vector;
     
    laplacian();
    ~laplacian();
    
    sp_mat laplacian_operator(mesh* grid);
    sp_mat laplacian_matrix(cell_field* field_ptr);
    cell_field laplacian_field(cell_field* field_ptr);
    cell_vector_field laplacian_field(cell_vector_field* field_ptr);
};

#endif /* laplacian_hpp */
