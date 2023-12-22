//
//  dt_euler.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 29/06/2022.
//

#ifndef dt_euler_hpp
#define dt_euler_hpp

/*
#include "Core"
#include "Sparse"

#include "../../mesh/mesh.hpp"
#include "../../fields/cell_field.hpp"
#include "../../fields/cell_vector_field.hpp"
#include "../../boundary_conditions/robin_BC.hpp"

class dt_euler{
public:
    
    typedef Eigen::SparseMatrix<double> sp_mat;
    typedef std::vector<Eigen::SparseMatrix<double>> vector_of_sp_mat;
    typedef Eigen::VectorXd eigen_vector;
    typedef std::string string;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    
    dt_euler(mesh* grid, int n_boundary_faces, double dt);
    ~dt_euler();
    
    sp_mat dt_matrix_A();
    sp_mat dt_matrix_B();
    sp_mat dt_A_mult_v_matrix();
    cell_field ddt(cell_field* field_ptr);
    cell_field ddt_mult_v(cell_field* field_ptr);
    cell_vector_field ddt(cell_vector_field* field_ptr);
    cell_vector_field ddt_mult_v(cell_vector_field* field_ptr);
    
    void compute_A_B();
    
private:
    mesh* grid;
    double dt;
    int n_boundary_faces;
    sp_mat A_mat;
    sp_mat B_mat;
    sp_mat A_mult_v_mat;
    
};

 */

#endif /* dt_euler_hpp */
