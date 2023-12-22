//
//  laplacian.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 26/06/2022.
//

#include "laplacian.hpp"

laplacian::laplacian(){}

laplacian::sp_mat laplacian::laplacian_operator(mesh *grid){
    sp_mat discretization_matrix(grid->n_cells(),grid->n_cells());
    discretization_matrix.reserve(Eigen::VectorXi::Constant(grid->n_cells(),10));
    for(auto f:grid->faces_list()){
        if(f.is_internal_face()){
            auto owner_neighbor = f.owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto nIdx=owner_neighbor[1]->index();
            auto distances = f.distances();
            auto o_vol=owner_neighbor[0]->vol();
            auto n_vol=owner_neighbor[1]->vol();
            auto d=distances[0]+distances[1];
            
            discretization_matrix.coeffRef(oIdx,oIdx)-=(f.area()/(o_vol*d));
            discretization_matrix.coeffRef(oIdx,nIdx)+=(f.area()/(o_vol*d));
            discretization_matrix.coeffRef(nIdx,nIdx)-=(f.area()/(n_vol*d));
            discretization_matrix.coeffRef(nIdx,oIdx)+=(f.area()/(n_vol*d));
        }
    }
    discretization_matrix.makeCompressed();
    return discretization_matrix;
}

    
laplacian::sp_mat laplacian::laplacian_matrix(cell_field* field_ptr){
    auto grid=field_ptr->discretization_mesh_ptr();
    sp_mat discretization_matrix(grid->n_cells(),grid->n_cells());
    for(auto f:grid->faces_list()){
        if(f.is_internal_face()){
            auto owner_neighbor = f.owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto nIdx=owner_neighbor[1]->index();
            auto distances = f.distances();
            auto o_vol=owner_neighbor[0]->vol();
            auto n_vol=owner_neighbor[1]->vol();
            auto d=distances[0]+distances[1];
            
            discretization_matrix.coeffRef(oIdx,oIdx)-=(f.area()/(o_vol*d));
            discretization_matrix.coeffRef(oIdx,nIdx)+=(f.area()/(o_vol*d));
            discretization_matrix.coeffRef(nIdx,nIdx)-=(f.area()/(n_vol*d));
            discretization_matrix.coeffRef(nIdx,oIdx)+=(f.area()/(n_vol*d));
        }
    }
    discretization_matrix.makeCompressed();
    return discretization_matrix;
}

laplacian::~laplacian()
{}

cell_field laplacian::laplacian_field(cell_field* field_ptr){
    laplacian::sp_mat discretization_matrix = this->laplacian_matrix(field_ptr);
    auto grid=field_ptr->discretization_mesh_ptr();
    laplacian::eigen_vector b(grid->n_cells());
    auto field_values=field_ptr->field_values();
    for(auto c:grid->cells_list()){
        b(c.index())=field_values[c.index()];
    }
    auto laplacian_eigen = discretization_matrix*b;
    cell_field laplacian_field(grid,"laplacian_"+field_ptr->field_name());
    auto& laplacian_field_ref = laplacian_field.field_values_ref();
    for(auto c:grid->cells_list()){
        laplacian_field_ref[c.index()]=laplacian_eigen(c.index());
    }
    return laplacian_field;
}

cell_vector_field laplacian::laplacian_field(cell_vector_field* field_ptr){
    auto field_components_ref=field_ptr->components_ref();
    cell_field laplacian_x = this->laplacian_field(&field_components_ref[0]);
    cell_field laplacian_y = this->laplacian_field(&field_components_ref[1]);
    cell_field laplacian_z = this->laplacian_field(&field_components_ref[2]);
    std::vector<cell_field> laplacian_vector_field_components;
    laplacian_vector_field_components.push_back(laplacian_x);
    laplacian_vector_field_components.push_back(laplacian_y);
    laplacian_vector_field_components.push_back(laplacian_z);
    cell_vector_field laplacian_vector_field(laplacian_vector_field_components,"laplacian_"+field_ptr->field_name());
    return laplacian_vector_field;
}
