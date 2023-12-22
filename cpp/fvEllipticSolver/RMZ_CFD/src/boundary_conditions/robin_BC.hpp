//
//  robin_BC.hpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 01/07/2022.
//

#ifndef robin_BC_hpp
#define robin_BC_hpp

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "point.hpp"
#include "mesh.hpp"
#include "face.hpp"
#include "table_reader.hpp"
#include "kd_tree.h"

#include "Core"
#include "Sparse"

class cell_field;
class face_field;

class robin_BC{
public:
    
    typedef Eigen::SparseMatrix<double,Eigen::RowMajor> sp_mat;
    typedef Eigen::VectorXd eigen_vector;
    typedef std::vector<Eigen::SparseMatrix<double>> vector_of_sp_mat;
    typedef std::vector<Eigen::VectorXd> vector_of_eigen_vector;
    typedef std::vector<face*> vector_of_pointers_to_faces;
    typedef std::vector<double> vector_of_doubles;
    typedef std::vector<std::vector<double>> vector_of_vector_of_doubles;
    typedef std::map<face*, int> map_pointer_of_face_to_int;
    typedef std::string string;
    typedef std::vector<point> vector_of_points;
    
    robin_BC(mesh* grid,
             vector_of_pointers_to_faces boundary_faces,
             vector_of_doubles alpha_dirichlet,
             vector_of_doubles ref_values,
             vector_of_vector_of_doubles ref_gradients,
             map_pointer_of_face_to_int* boundary_face_to_boundary_field_map=nullptr,
             cell_field* field_ptr=nullptr
             );
    
    robin_BC(mesh* grid,
             vector_of_pointers_to_faces boundary_faces,
             string boundary_file_name,
             map_pointer_of_face_to_int* boundary_face_to_boundary_field_map=nullptr,
             cell_field* field_ptr=nullptr,
             bool interpolate=true
             );
    
    sp_mat laplacian_A_matrix();
    eigen_vector laplacian_B_vector();
    vector_of_sp_mat gradient_A_matrix();
    vector_of_eigen_vector gradient_B_vector();
    vector_of_pointers_to_faces boundary_faces_list();
    void compute_grad_matrices();
    void compute_laplacian_matrices();
    void set_field_ptr(cell_field* field_ptr);
    void update_boundary_values();
    
private:
    
    mesh* grid;
    sp_mat laplacian_A;
    eigen_vector laplacian_B;
    vector_of_sp_mat gradient_A;
    vector_of_eigen_vector gradient_B;
    vector_of_pointers_to_faces boundary_faces;
    vector_of_doubles alpha_dirichlet;
    vector_of_doubles ref_values;
    vector_of_vector_of_doubles ref_gradients;
    sp_mat laplacian_A_;
    eigen_vector laplacian_B_;
    vector_of_sp_mat gradient_A_;
    vector_of_eigen_vector gradient_B_;
    cell_field* field_ptr_;
    map_pointer_of_face_to_int* boundary_face_to_boundary_field_map;
    
};
#endif /* robin_BC_hpp */
