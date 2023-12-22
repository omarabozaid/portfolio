//
//  vector_operators_overloaded.cpp
//  test_xCode
//
//  Created by Omar Elsayed on 09/05/2022.
//

#include "vector_operators_overloaded.hpp"
#include <iostream>

std::vector<double> operator*(std::vector<double> vec, double lambda){
    std::vector<double> scaled_vector(vec.size(),double(0.0));
    auto i=0;
    for(double x:vec){
        scaled_vector[i]=x*lambda;
        i+=1;
    }
    return scaled_vector;
};

std::vector<double> operator*(double lambda,std::vector<double> vec){
    return (vec*lambda);
};

std::vector<double> operator/(std::vector<double> vec, double lambda){
    double inv=double(1.0/lambda);
    return (vec*inv);
};

std::vector<double> operator+(std::vector<double> vec1, std::vector<double> vec2){
    auto n1=vec1.size();
    auto n2=vec2.size();
    if (n1==n2){
        auto i=0;
        std::vector<double> added(n1,double(0.0));
        for(auto& x:added){
            x+=(vec1[i]+vec2[i]);
            i+=1;
        }
        return added;
    }
    else{
        //std::cout<<"vectors do not have same length---return vec1";
        return vec1;
    }
};

std::vector<double> operator-(std::vector<double> vec1, std::vector<double> vec2){
    return (vec1+(vec2*double(-1.0)));
};


void operator*=(std::vector<double>& vec, double lambda){
    for(auto& element:vec){
        element*=lambda;
    }
};

void operator/=(std::vector<double>& vec, double lambda){
    vec*(double(1/lambda));
};


void operator+=(std::vector<double>& vec1, std::vector<double>& vec2){
    auto n1=vec1.size();
    auto n2=vec2.size();
    if (n1==n2){
        auto i=0;
        for(double& x:vec2){
            vec1[i]+=x;
            i+=1;
        }
    }
}

void operator-=(std::vector<double>& vec1, std::vector<double>& vec2){
    vec1+(-1.0*vec2);
}

std::vector<double> normalize(std::vector<double>& vec,double small){
    double mag=magnitude(vec)+small;
    for(auto& e:vec){
        e/=mag;
    }
    return vec;
}

double magnitude(std::vector<double>& vec){
    double mag=0.0;
    for(auto& e:vec){
        mag+=(e*e);
    }
    return sqrt(mag);
}

double dot(std::vector<double>& vec1,std::vector<double>& vec2){
    double dot_=0.0;
    int counter=0;
    for(auto& e:vec1){
        dot_+=(e*(vec2[counter]));
        counter+=1;
    }
    return dot_;
}

std::vector<double> operator&(std::vector<std::vector<double>> v1,std::vector<std::vector<double>> v2){
    auto vec_size=v1[0].size();
    std::vector<double> result(vec_size,0.0);
    for(int counter=0;counter<vec_size;counter++){
        std::vector<double> vec1({v1[counter][0],v1[counter][1],v1[counter][2]});
        std::vector<double> vec2({v2[counter][0],v2[counter][1],v2[counter][2]});
        result[counter]=dot(vec1,vec2);
    }
    return result;
}

std::vector<double> cross(std::vector<double> u, std::vector<double> v) {
    std::vector<double> w(3,0.0);
    w[0] = u[1] * v[2] - u[2] * v[1];
    w[1] = -(u[0] * v[2] - u[2] * v[0]);
    w[2] = u[0] * v[1] - u[1] * v[0];
    return w;
}