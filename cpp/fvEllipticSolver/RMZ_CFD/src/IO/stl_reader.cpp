//
//  stl_reader.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 26/08/2022.
//

#include "stl_reader.hpp"

stl_reader::stl_reader(stl_reader::string file_name){
    streiter_stl_reader::StlMesh <double, unsigned int> mesh (file_name);
    for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
        for(size_t icorner = 0; icorner < 3; ++icorner) {
            const double* c = mesh.tri_corner_coords (itri, icorner);
            vector_of_doubles coord({c[0],c[1],c[2]});
            points_list_.push_back(point(coord));
        }
        const double* n = mesh.tri_normal (itri);
        vector_of_doubles n_v({n[0],n[1],n[2]});
        normals_list_.push_back(n_v);
    }
    centers_list_=vector_of_points(mesh.num_tris(),vector_of_doubles(3,0.0));
    for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
        for(size_t icorner = 0; icorner < 3; ++icorner) {
            size_t index_counter_ = 3*itri+icorner;
            centers_list_[itri]=centers_list_[itri]+points_list_[index_counter_];
        }
        centers_list_[itri]=centers_list_[itri]*double(1.0/3.0);
    }
}
