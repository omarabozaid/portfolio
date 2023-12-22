//
//  gradient.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 27/06/2022.
//

#include "gradient.hpp"

gradient::gradient(){}

gradient::vector_of_sp_mat gradient::gradient_operator(mesh* grid){
    
    gradient::sp_mat discretization_matrix_x(grid->n_cells(),grid->n_cells());
    gradient::sp_mat discretization_matrix_y(grid->n_cells(),grid->n_cells());
    gradient::sp_mat discretization_matrix_z(grid->n_cells(),grid->n_cells());
    int f_index=0;
    int n_faces=grid->n_faces();
    auto faces_list=grid->faces_list();
    for(f_index=0;f_index<n_faces;f_index++){
        auto f=faces_list[f_index];
        if(f.is_internal_face()){
            
            auto owner_neighbor = f.owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto nIdx=owner_neighbor[1]->index();
            auto distances = f.distances();
            auto o_vol=owner_neighbor[0]->vol();
            auto n_vol=owner_neighbor[1]->vol();
            auto d=distances[0]+distances[1];
            
            auto x_o=owner_neighbor[0]->cell_center().coordinates();
            auto x_n=owner_neighbor[1]->cell_center().coordinates();
            auto n_o_n=x_n-x_o;
            n_o_n=normalize(n_o_n);
            auto f_area=f.area();
            
            auto o_weight_x=f_area*(distances[0]/d)*(n_o_n[0]/o_vol);
            auto o_weight_y=f_area*(distances[0]/d)*(n_o_n[1]/o_vol);
            auto o_weight_z=f_area*(distances[0]/d)*(n_o_n[2]/o_vol);
            
            auto n_weight_x=-f_area*(distances[1]/d)*(n_o_n[0]/n_vol);
            auto n_weight_y=-f_area*(distances[1]/d)*(n_o_n[1]/n_vol);
            auto n_weight_z=-f_area*(distances[1]/d)*(n_o_n[2]/n_vol);
            
            discretization_matrix_x.coeffRef(oIdx,oIdx)+=o_weight_x;
            discretization_matrix_x.coeffRef(oIdx,nIdx)+=o_weight_x;
            discretization_matrix_x.coeffRef(nIdx,nIdx)+=n_weight_x;
            discretization_matrix_x.coeffRef(nIdx,oIdx)+=n_weight_x;
            
            discretization_matrix_y.coeffRef(oIdx,oIdx)+=o_weight_y;
            discretization_matrix_y.coeffRef(oIdx,nIdx)+=o_weight_y;
            discretization_matrix_y.coeffRef(nIdx,nIdx)+=n_weight_y;
            discretization_matrix_y.coeffRef(nIdx,oIdx)+=n_weight_y;
            
            discretization_matrix_z.coeffRef(oIdx,oIdx)+=o_weight_z;
            discretization_matrix_z.coeffRef(oIdx,nIdx)+=o_weight_z;
            discretization_matrix_z.coeffRef(nIdx,nIdx)+=n_weight_z;
            discretization_matrix_z.coeffRef(nIdx,oIdx)+=n_weight_z;
        }
    }
    discretization_matrix_x.makeCompressed();
    discretization_matrix_y.makeCompressed();
    discretization_matrix_z.makeCompressed();
    return std::vector<gradient::sp_mat>({discretization_matrix_x,discretization_matrix_y,discretization_matrix_z});
}

std::vector<gradient::sp_mat> gradient::gradient_matrices(cell_field* field_ptr){
    auto field_name=field_ptr->field_name();
    auto grid=field_ptr->discretization_mesh_ptr();
    return this->gradient_operator(grid);
}

cell_vector_field gradient::gauss_linear(cell_field* field_ptr){
    auto field_name=field_ptr->field_name();
    auto grid=field_ptr->discretization_mesh_ptr();
    auto field_values=field_ptr->field_values();
    
    std::vector<gradient::sp_mat> grad_matrices=this->gradient_matrices(field_ptr);
    
    eigen_vector b_x(grid->n_cells());
    eigen_vector b_y(grid->n_cells());
    eigen_vector b_z(grid->n_cells());
    for(auto c:grid->cells_list()){
        b_x(c.index())=field_values[c.index()];
        b_y(c.index())=field_values[c.index()];
        b_z(c.index())=field_values[c.index()];
    }
    eigen_vector grad_x = grad_matrices[0]*b_x;
    eigen_vector grad_y = grad_matrices[1]*b_y;
    eigen_vector grad_z = grad_matrices[2]*b_z;
    
    gradient::vector_of_cell_fields dfield_dx;
    dfield_dx.push_back(cell_field(grid,"d_"+field_name+"_dx"));
    dfield_dx.push_back(cell_field(grid,"d_"+field_name+"_dy"));
    dfield_dx.push_back(cell_field(grid,"d_"+field_name+"_dz"));
    auto& grad_x_ref = dfield_dx[0].field_values_ref();
    auto& grad_y_ref = dfield_dx[1].field_values_ref();
    auto& grad_z_ref = dfield_dx[2].field_values_ref();
    for(auto c:grid->cells_list()){
        grad_x_ref[c.index()]=grad_x(c.index());
        grad_y_ref[c.index()]=grad_y(c.index());
        grad_z_ref[c.index()]=grad_z(c.index());
    }
    return cell_vector_field(dfield_dx,"grad_"+field_ptr->field_name());
}

gradient::~gradient()
{}
