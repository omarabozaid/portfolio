//
//  geom_tools.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 16/07/2022.
//

#include "geom_tools.hpp"

void fit_plane(std::vector<std::vector<double>> pts,std::vector<double>& normal,std::vector<double>& center){
    int n_pts = int(pts.size());
    Eigen::Matrix< eigen_vector::Scalar, Eigen::Dynamic, Eigen::Dynamic > coord(3, n_pts);
    for (int col_idx = 0; col_idx < n_pts; col_idx++) {
        eigen_vector pt(3);
        for(int j=0; j<3; j++){
            pt(j)=pts[col_idx][j];
        }
        coord.col(col_idx)=pt;
    };
    eigen_vector centroid = coord.rowwise().mean();
    coord.colwise() -= centroid;
    auto svd = coord.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
    eigen_vector plane_normal = svd.matrixU().rightCols<1>();
    plane_normal /= plane_normal.norm();
    int i = 0;
    for(auto& n:normal){
        n=plane_normal[i];
        i+=1;
    }
    i = 0;
    for(auto& c:center){
        c=centroid[i];
        i+=1;
    }
    std::vector<double> f_xyz(3,0);
    for(auto pt:pts){
        f_xyz=f_xyz+pt;
    }
}

std::vector<double> plane_normal( std::vector<std::vector<double>> pts){
    std::vector<double> normal;
    std::vector<double> center;
    fit_plane(pts,normal,center);
    return normal;
}

std::vector<double> plane_center( std::vector<std::vector<double>> pts){
    std::vector<double> normal;
    std::vector<double> center;
    fit_plane(pts,normal,center);
    return center;
}

double area( std::vector<std::vector<double>> pts,  std::vector<double> center){
    double a=0.0;
    for(size_t i=0; i<pts.size()-1; i++){
        auto pt1 = pts[i];
        auto pt2 = pts[i+1];
        auto pt3 = center;
        a += area(pt1, pt2, pt3);
    }
    auto pt1 = pts[pts.size()-1];
    auto pt2 = pts[0];
    auto pt3 = center;
    a += area(pt1, pt2, pt3);
    return a;
}

double area( std::vector<double> pt1,  std::vector<double> pt2,  std::vector<double> pt3){
    double area_value = 0.0;
    eigen_vector a(3), b(3), c(3);
    for(size_t i = 0; i < 3; i++){
        a(i)=pt1[i];
        b(i)=pt2[i];
        c(i)=pt3[i];
    }
    auto ab = b-a;
    auto ac = c-a;
    auto cross_product = ab.cross(ac);
    area_value = 0.5*cross_product.norm();
    return area_value;
}

void area( std::vector<std::vector<double>> pts,  std::vector<double> center, double& area_value){
    area_value=area(pts,center);
}

/*
double geom_tools::poly_volume(
                              std::vector<std::vector<double>> faces_centers,
                              std::vector<std::vector<double>> faces_normals,
                              std::vector<double> cell_center,
                              std::vector<double> areas,
                              int cell_type
                              ){
    double vol = 0;
    int n_faces = int(areas.size());
    for(int face_i=0; face_i<n_faces;face_i++){
        auto vec = faces_centers[face_i]-cell_center;
        auto d = std::abs(dot(vec, faces_normals[face_i]));
        vol+=(d*areas[face_i]);
    }
    vol/=6;
    return vol;
}
*/

double poly_volume(std::vector<std::vector<double>> cell_pts, int cell_type){
    if(cell_type==4)
    {
        return tetra_volume(cell_pts);
    }
    else if(cell_type != -1){
        auto model = cell_model();
        auto n_points = cell_pts.size();
        std::vector<size_t> vertices_ids(n_points);
        std::iota (std::begin(vertices_ids), std::end(vertices_ids), 0);
        auto tetras_as_indices = model.tetrahedralize(vertices_ids);
        double vol = 0;
        for(auto& tetra:tetras_as_indices){
            std::vector<std::vector<double>> tetras_pts;
            for(auto& vertex_id:tetra){
                tetras_pts.push_back(cell_pts[vertex_id]);
            }
            vol+=tetra_volume(tetras_pts);
        }
        return vol;
    }
    else{
        return -1;
    }
}


double tetra_volume(std::vector<std::vector<double>> cell_pts){
    auto v1 = cell_pts[0]-cell_pts[3];
    auto v2 = cell_pts[1]-cell_pts[3];
    auto v3 = cell_pts[2]-cell_pts[3];
    eigen_vector a(3), b(3), c(3);
    for(size_t i = 0; i < 3; i++){
        a(i)=v1[i];
        b(i)=v2[i];
        c(i)=v3[i];
    }
    auto vol = std::abs(a.dot(b.cross(c))/6);
    return double(vol);
}

double tetra_signed_volume(std::vector<std::vector<double>> cell_pts){
    auto v1 = cell_pts[0]-cell_pts[3];
    auto v2 = cell_pts[1]-cell_pts[3];
    auto v3 = cell_pts[2]-cell_pts[3];
    eigen_vector a(3), b(3), c(3);
    for(size_t i = 0; i < 3; i++){
        a(i)=v1[i];
        b(i)=v2[i];
        c(i)=v3[i];
    }
    auto vol = (a.dot(b.cross(c))/6);
    return double(vol);
}
