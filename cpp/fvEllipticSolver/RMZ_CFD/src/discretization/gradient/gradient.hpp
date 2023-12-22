//
//  gradient.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#ifndef gradient_hpp
#define gradient_hpp

#include <vector>
#include "cell_field.hpp"
#include "cell_vector_field.hpp"
#include "vector_operators_overloaded.hpp"

#include "Core"
#include "Sparse"

class gradient{
public:
    typedef Eigen::SparseMatrix<double,Eigen::RowMajor> sp_mat;
    typedef Eigen::VectorXd eigen_vector;
    typedef std::vector<Eigen::SparseMatrix<double,Eigen::RowMajor>> vector_of_sp_mat;
    typedef std::vector<Eigen::VectorXd> vector_of_eigen_vector;
    typedef std::vector<cell_field> vector_of_cell_fields;
    typedef std::vector<double> vector_of_doubles;
    
    gradient();
    ~gradient();
    
    vector_of_sp_mat gradient_operator(mesh* grid);
    
    cell_vector_field gauss_linear(cell_field* field_ptr);
    std::vector<sp_mat> gradient_matrices(cell_field* field_ptr);
};

#endif /* gradient_hpp */
