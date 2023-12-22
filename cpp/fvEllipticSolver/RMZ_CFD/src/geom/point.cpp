//
//  point.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 08/05/2022.
//

#include "point.hpp"
#include "../tools/vector_operators_overloaded.hpp"

point::point()
{
    xyz=std::vector<double>(3,0.0);
};

point::point(std::vector<double> pt)
:
xyz(pt)
{
};

point::point(const point& another_point)
:
xyz(another_point.xyz)
{};

point::~point(){};

std::vector<double> point::coordinates(){
    return xyz;
}

std::vector<double> point::coordinates() const{
    return xyz;
}

point point::operator*(double lambda){
    std::vector<double> scaled_point=xyz*lambda;
    return point(scaled_point);
};

point point::operator/(double lambda){
    return this->operator*(1.0/lambda);
};

point point::operator+(point another_point){
    std::vector<double> added_point=this->xyz+another_point.xyz;
    return point(added_point);
};

point point::operator-(point another_point){
    return this->operator+(another_point*double(-1.0));
};

void point::operator*=(double lambda){
    xyz*=lambda;
};

void point::operator/=(double lambda){
    xyz/=lambda;
};

void point::operator=(point another_point){
    this->xyz=another_point.xyz;
};

std::ostream& operator<<(std::ostream &os, point& pt){
    os<<"X: "<<pt.xyz[0]<<", Y= "<<pt.xyz[1]<<", Z= "<<pt.xyz[2]<<"\n";
    return os;
};

point operator*(double lambda,point pt){
    return pt*lambda;
};

void point::operator+=(point another_point){
    xyz+=another_point.xyz;
};

void point::operator-=(point another_point){
    xyz-=another_point.xyz;
};

double point::operator[](int i){
    return xyz[i];
}

double point::operator[](int i) const {
    return xyz[i];
}
