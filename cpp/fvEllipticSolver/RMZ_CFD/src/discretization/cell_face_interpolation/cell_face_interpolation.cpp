//
//  cell_face_interpolation.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 28/08/2022.
//

#include "cell_face_interpolation.hpp"

face_field cell_face_interpolation::interpolate(cell_field* scalar_field){
    face_field interpolated_field(
                                  scalar_field->discretization_mesh_ptr(),
                                  scalar_field->field_name()+"_interpolated"
                                  );
    mesh& grid=*(scalar_field->discretization_mesh_ptr());
    vector_of_doubles scalar_field_values=scalar_field->field_values();
    vector_of_doubles& interpolated_field_values=interpolated_field.field_values_ref();
    
    for(auto f:grid.faces_list()){
        if(f.is_internal_face()){
            int face_index=f.index();
            int oIdx=f.owner_neighbour()[0]->index();
            int nIdx=f.owner_neighbour()[1]->index();
            double oDis=f.distances()[0];
            double nDis=f.distances()[1];
            double total_distance=oDis+nDis;
            interpolated_field_values[face_index] = ((scalar_field_values[oIdx]*oDis+scalar_field_values[nIdx]*nDis)/total_distance);
            }
        else if (scalar_field->bc() == nullptr && f.is_boundary_face()){
            int face_index=f.index();
            int oIdx=f.owner_neighbour()[0]->index();
            interpolated_field_values[face_index] = scalar_field_values[oIdx];
            }
        }
    
    if(scalar_field->bc() != nullptr){
        auto boundary_faces = scalar_field->bc()->boundary_faces_list();
        auto boundary_values = scalar_field->field_boundary_values();
        int counter=0;
        for(auto f:boundary_faces){
            auto face_index=f->index();
            interpolated_field_values[face_index] = boundary_values[counter] ;
            counter+=1;
        }
    }
    
    return interpolated_field;
}

face_vector_field cell_face_interpolation::interpolate(cell_vector_field* vector_field){
    string group_name=vector_field->field_name()+"_interpolated";
    face_field interpolated_x = this->interpolate(&vector_field->components_ref()[0]);
    face_field interpolated_y = this->interpolate(&vector_field->components_ref()[1]);
    face_field interpolated_z = this->interpolate(&vector_field->components_ref()[2]);
    std::vector<face_field> interpolated_components({interpolated_x,interpolated_y,interpolated_z});
    face_vector_field interpolated_field(interpolated_components,group_name);
    return interpolated_field;
}
