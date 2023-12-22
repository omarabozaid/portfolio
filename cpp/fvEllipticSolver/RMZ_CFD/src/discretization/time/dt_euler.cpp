//
//  dt_euler.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 29/06/2022.
//

#include "dt_euler.hpp"

/*
dt_euler::dt_euler(mesh* grid, int n_boundary_faces, double dt)
:
grid(grid),
dt(dt),
n_boundary_faces(n_boundary_faces)
{
    compute_A_B();
};

void dt_euler::compute_A_B(){
    auto n_cells = grid->n_cells();
    A_mat = sp_mat(n_cells,n_cells);
    for(auto c:grid->cells_list()){
        auto c_index=c.index();
        A_mat.coeffRef(c_index,c_index)=(1.0/dt);
    }
    A_mult_v_mat=A_mat;
    for(auto c:grid->cells_list()){
        auto c_index=c.index();
        A_mult_v_mat.coeffRef(c_index,c_index)*=c.vol();
    }
    B_mat = sp_mat(n_boundary_faces,n_boundary_faces);
    for(auto counter=0;counter<n_boundary_faces;counter++){
        B_mat.coeffRef(counter,counter)=(1.0/dt);
    }
}

dt_euler::~dt_euler(){};

dt_euler::sp_mat dt_euler::dt_matrix_A(){
    return A_mat;
}

dt_euler::sp_mat dt_euler::dt_matrix_B(){
    return B_mat;
}

dt_euler::sp_mat dt_euler::dt_A_mult_v_matrix(){
    return A_mult_v_mat;
}

cell_field dt_euler::ddt(cell_field* field_ptr){
    
    cell_field dt_field(grid,"dt_"+field_ptr->field_name());
    vector_of_doubles field_curr=field_ptr->field_values();
    vector_of_doubles field_prev=field_ptr->field_values_prev();
    vector_of_doubles delta_field=field_curr-field_prev;
    eigen_vector delta_field_eigen(grid->n_cells());
    for(auto c:grid->cells_list()){
        delta_field_eigen(c.index())=delta_field[c.index()];
    }
    eigen_vector derivative = A_mat*delta_field_eigen;
    vector_of_doubles& derivative_ref = dt_field.field_values_ref();
    for(auto c:grid->cells_list()){
        derivative_ref[c.index()]=derivative(c.index());
    }
    vector_of_doubles boundary_field_curr=field_ptr->field_boundary_values();
    vector_of_doubles boundary_field_prev=field_ptr->field_boundary_values_prev();
    vector_of_doubles boundary_delta_field=boundary_field_curr-boundary_field_prev;
    
    eigen_vector boundary_delta_field_eigen(n_boundary_faces);
    for(auto counter=0;counter<n_boundary_faces;counter++){
        boundary_delta_field_eigen(counter)=boundary_delta_field[counter];
    }
    eigen_vector boundary_derivative = B_mat*boundary_delta_field_eigen;
    vector_of_doubles& boundary_derivative_ref = dt_field.field_boundary_values_ref();
    for(auto counter=0;counter<n_boundary_faces;counter++){
        boundary_derivative_ref[counter]=boundary_delta_field_eigen(counter);
    }
    return dt_field;

}

cell_field dt_euler::ddt_mult_v(cell_field* field_ptr){
    cell_field dt_field(grid,"dt_"+field_ptr->field_name());
    vector_of_doubles field_curr=field_ptr->field_values();
    vector_of_doubles field_prev=field_ptr->field_values_prev();
    vector_of_doubles delta_field=field_curr-field_prev;
    eigen_vector delta_field_eigen(grid->n_cells());
    for(auto c:grid->cells_list()){
        delta_field_eigen(c.index())=delta_field[c.index()];
    }
    eigen_vector derivative = A_mult_v_mat*delta_field_eigen;
    vector_of_doubles& derivative_ref = dt_field.field_values_ref();
    for(auto c:grid->cells_list()){
        derivative_ref[c.index()]=derivative(c.index());
    }
    return dt_field;
}

cell_vector_field dt_euler::ddt(cell_vector_field* field_ptr){
    cell_field dt_field_x = this->ddt(&(field_ptr->components_ref()[0]));
    cell_field dt_field_y = this->ddt(&(field_ptr->components_ref()[1]));
    cell_field dt_field_z = this->ddt(&(field_ptr->components_ref()[2]));
    std::vector<cell_field> dt_vector_field;
    dt_vector_field.push_back(dt_field_x);
    dt_vector_field.push_back(dt_field_y);
    dt_vector_field.push_back(dt_field_z);
    cell_vector_field dt_field(dt_vector_field,"dt_"+field_ptr->field_name());
    return dt_field;
}

cell_vector_field dt_euler::ddt_mult_v(cell_vector_field* field_ptr){
    cell_field dt_field_x = this->ddt_mult_v(&(field_ptr->components_ref()[0]));
    cell_field dt_field_y = this->ddt_mult_v(&(field_ptr->components_ref()[1]));
    cell_field dt_field_z = this->ddt_mult_v(&(field_ptr->components_ref()[2]));
    std::vector<cell_field> dt_vector_field;
    dt_vector_field.push_back(dt_field_x);
    dt_vector_field.push_back(dt_field_y);
    dt_vector_field.push_back(dt_field_z);
    cell_vector_field dt_field(dt_vector_field,"dt_"+field_ptr->field_name());
    return dt_field;
}


*/
