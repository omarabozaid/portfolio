//
//  point.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 08/05/2022.
//

#ifndef point_hpp
#define point_hpp

#include <vector>
#include <iostream>

class point{
public:
    point();
    point(std::vector<double> pt);
    point(const point& another_point);
    ~point();
    std::vector<double> coordinates() ;
    std::vector<double> coordinates() const;
    friend std::ostream& operator<<(std::ostream &os, point& pt);
    friend point operator*(double lambda,point pt);
    
    point operator*(double lambda);
    point operator/(double lambda);
    point operator+(point another_point);
    point operator-(point another_point);
    
    void operator*=(double lambda);
    void operator/=(double lambda);
    
    void operator+=(point another_point);
    void operator-=(point another_point);
    
    void operator=(point another_point);
    
    double operator[](int i);
    double operator[](int i) const;
        
private:
    std::vector<double> xyz;
};


#endif /* point_hpp */
