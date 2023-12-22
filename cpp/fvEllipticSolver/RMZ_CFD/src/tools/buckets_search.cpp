#include"buckets_search.hpp"

buckets_search::buckets_search(
        std::vector<point>& points,
        std::vector<double> x_min,
        std::vector<double> x_max,
        std::vector<double> dx
    ):
    points_(points),
    x_min_(x_min),
    x_max_(x_max),
    dx_(dx)
    {
        spacing_=std::vector<size_t>(3,0);
        int counter = 0;
        for(auto& val_:spacing_){
            val_ = std::floor((x_max_[counter]-x_min_[counter])/dx_[counter]);
            counter += 1;
        }
        size_t n_buckets_ = 1;
        for(auto& val_:spacing_){
            n_buckets_*=val_;
        }
        buckets_.resize(n_buckets_);
        std::cout<<"n° buckets is "<<n_buckets_<<std::endl;
        size_t pt_idx = 0 ;
        for(auto& pt:points_){
            auto idx_ = index(&pt);
            buckets_[idx_].push_back(pt_idx);
            pt_idx+=1;
        }
    }

size_t buckets_search::nearest_point_ptr(point& pt){
    auto bucket_ijk_ = ijk(&pt);
    std::set<size_t> search_buckets_indices;
    for(size_t di=-1; di<2; di++){
        for(size_t dj=-1; dj<2; dj++){
            for(size_t dk=-1; dk<2; dk++){
                std::vector<size_t> dijk({bucket_ijk_[0]+di,bucket_ijk_[1]+dj,bucket_ijk_[2]+dk});
                bool is_inside_ = true;
                int counter = 0;
                for(auto& dijk_index:dijk){
                    if(dijk_index<0 || dijk_index>=spacing_[counter]){
                        is_inside_=false;
                        break;
                    }
                    counter+=1;
                }
                if(is_inside_){
                    search_buckets_indices.insert(index(dijk));
                }
            }
        }
    }
    double min_dist_ = 1e6;
    size_t nearest_pt_idx_ = -1;
    for(auto search_bucket_index:search_buckets_indices){
        auto search_bucket = buckets_[search_bucket_index];
        for(auto pt_idx:search_bucket){
            auto pt_xyz_ = pt.coordinates();
            auto bucket_pt_xyz_ = points_[pt_idx].coordinates();
            auto vect = pt_xyz_-bucket_pt_xyz_;
            auto dist = magnitude(vect);
            if(dist<=min_dist_){
                min_dist_=dist;
                nearest_pt_idx_ = pt_idx;
            }
        }
    }
    return nearest_pt_idx_;
}

std::vector<size_t> buckets_search::ijk(point* pt){
    std::vector<size_t> pt_ijk_(3,0);
    auto pt_xyz_ = pt->coordinates();
    int counter = 0;
    for(auto& idx_:pt_ijk_){
        idx_ = std::floor((pt_xyz_[counter]-x_min_[counter])/dx_[counter]);
        counter += 1;
    }
    return pt_ijk_;
}

size_t buckets_search::index(point* pt){
    return index(ijk(pt));
}

size_t buckets_search::index(const std::vector<size_t>& pt_ijk){
    return pt_ijk[0]+spacing_[0]*pt_ijk[1]+spacing_[0]*spacing_[1]*pt_ijk[2];
}