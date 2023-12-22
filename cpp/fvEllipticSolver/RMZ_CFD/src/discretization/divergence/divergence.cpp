//
//  divergence.cpp
//  RMZ_cfd
//
//  Created by Omar Elsayed on 29/06/2022.
//

#include "divergence.hpp"


divergence::divergence(){}

divergence::~divergence(){}

divergence::sp_mat divergence::divergence_matrix(face_field *flux_ptr, cell_field *field_ptr){
    auto grid=field_ptr->discretization_mesh_ptr();
    sp_mat discretization_matrix(grid->n_cells(),grid->n_cells());
    auto flux_values=flux_ptr->field_values();
    
    for(auto f:grid->faces_list()){
        if(f.is_internal_face()){
            auto owner_neighbor = f.owner_neighbour();
            auto oIdx=owner_neighbor[0]->index();
            auto nIdx=owner_neighbor[1]->index();
            auto face_index=f.index();
            auto face_flux=flux_values[face_index];
            
            discretization_matrix.coeffRef(oIdx,oIdx)+=fmax(face_flux,0);
            discretization_matrix.coeffRef(oIdx,nIdx)+=fmin(face_flux,0);
            
            discretization_matrix.coeffRef(nIdx,nIdx)-=fmin(face_flux,0);
            discretization_matrix.coeffRef(nIdx,oIdx)-=fmax(face_flux,0);
            
        }
    }
    
    discretization_matrix.makeCompressed();
    return discretization_matrix;
}

cell_field divergence::div(face_field *flux_ptr, cell_field *field_ptr){
    divergence::sp_mat div_matrix=this->divergence_matrix(flux_ptr, field_ptr);
    auto grid=field_ptr->discretization_mesh_ptr();
    auto field_values=field_ptr->field_values();
    eigen_vector b(grid->n_cells());
    for(auto c:grid->cells_list()){
        b(c.index())=field_values[c.index()];
    }
    eigen_vector div_vals = div_matrix*b;
    cell_field div_field(grid,"div_"+field_ptr->field_name());
    auto& div_field_values=div_field.field_values_ref();
    for(auto c:grid->cells_list()){
        div_field_values[c.index()]=div_vals(c.index());
    }
    return div_field;
}

cell_vector_field divergence::div(face_field *flux_ptr, cell_vector_field *field_ptr){
    auto field_x=field_ptr->components()[0];
    auto field_y=field_ptr->components()[1];
    auto field_z=field_ptr->components()[2];
    
    auto div_x=this->div(flux_ptr, &field_x);
    auto div_y=this->div(flux_ptr, &field_y);
    auto div_z=this->div(flux_ptr, &field_z);
    
    std::vector<cell_field> div_components;
    div_components.push_back(div_x);
    div_components.push_back(div_y);
    div_components.push_back(div_z);
    
    cell_vector_field div_field(div_components,"div_"+field_ptr->field_name());
    return div_field;
}


cell_field divergence::div(cell_vector_field *field_ptr){
    auto grid=field_ptr->discretization_mesh_ptr();
    cell_face_interpolation interpolation_object;
    auto field_faces=interpolation_object.interpolate(field_ptr);
    auto field_face_components=field_faces.components();
    
    auto vec_x=field_face_components[0].field_values();
    auto vec_y=field_face_components[1].field_values();
    auto vec_z=field_face_components[2].field_values();
    
    cell_field div_field(grid,"div_"+field_ptr->field_name());
    auto& div_field_ref_values=div_field.field_values_ref();
    
    for(auto f:grid->faces_list()){
        auto o_idx=f.owner_neighbour()[0]->index();
        auto n_idx=f.owner_neighbour()[0]->index();
        
        auto o_vol=f.owner_neighbour()[0]->vol();
        auto n_vol=f.owner_neighbour()[0]->vol();
        
        auto f_idx=f.index();
        auto n_f=f.normal();
        auto area_f=f.area();
        
        
        if(f.is_internal_face()){
            n_idx=f.owner_neighbour()[1]->index();
            n_vol=f.owner_neighbour()[1]->vol();
            n_f =f.owner_neighbour()[1]->cell_center().coordinates() - f.owner_neighbour()[0]->cell_center().coordinates();
            n_f = normalize(n_f);
        }
        
        auto vec_f=std::vector<double>({vec_x[f_idx],vec_y[f_idx],vec_z[f_idx]});
        auto face_div=area_f*dot(vec_f, n_f);
        div_field_ref_values[o_idx]+=(face_div/o_vol);
        if(f.is_internal_face()){
            div_field_ref_values[n_idx]-=(face_div/n_vol);
        }
    }
    return div_field;
}
