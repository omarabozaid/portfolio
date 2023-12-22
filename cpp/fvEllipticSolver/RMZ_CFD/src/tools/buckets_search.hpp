#ifndef buckets_search_hpp
#define buckets_search_hpp

#include<cmath>
#include<vector>
#include<set>

#include"point.hpp"
#include"vector_operators_overloaded.hpp"

class buckets_search{
    public:
        buckets_search(
            std::vector<point>& points,
            std::vector<double> x_min,
            std::vector<double> x_max,
            std::vector<double> dx
        );
        ~buckets_search(){};
        size_t nearest_point_ptr(point& pt);

    private:
        std::vector<point>& points_;
        std::vector<double> x_min_;
        std::vector<double> x_max_;
        std::vector<double> dx_;
        std::vector<size_t> spacing_;

        std::vector<std::vector<size_t>> ijk_;
        std::vector<size_t> indices_;
        std::vector<std::vector<size_t>> buckets_;

        std::vector<size_t> ijk(point* pt);
        size_t index(point* pt);
        size_t index(const std::vector<size_t>& pt_ijk);    
};

#endif