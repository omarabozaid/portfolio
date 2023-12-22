//
//  vector_operators_overloaded.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 09/05/2022.
//

#ifndef vector_operators_overloaded_hpp
#define vector_operators_overloaded_hpp

#include <vector>
#include <math.h>

std::vector<double> operator*(std::vector<double> vec, double lambda);

std::vector<double> operator*(double lambda,std::vector<double> vec);

std::vector<double> operator/(std::vector<double> vec, double lambda);

std::vector<double> operator+(std::vector<double> vec1, std::vector<double> vec2);

std::vector<double> operator-(std::vector<double> vec1, std::vector<double> vec2);


void operator*=(std::vector<double>& vec, double lambda);

void operator/=(std::vector<double>& vec, double lambda);

void operator+=(std::vector<double>& vec1, std::vector<double>& vec2);

void operator-=(std::vector<double>& vec1, std::vector<double>& vec2);

std::vector<double> normalize(std::vector<double>& vec,double small=1e-8);

double magnitude(std::vector<double>& vec);

double dot(std::vector<double>& vec1, std::vector<double>& vec2);

std::vector<double> cross(std::vector<double> vec1, std::vector<double> vec2);

std::vector<double> operator&(std::vector<std::vector<double>> v1,std::vector<std::vector<double>> v2);

#endif /* vector_operators_overloaded_hpp */
