/*****************************************************
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2024-01-12
 * Bellman Ford : Node
 *                                                     
 ******************************************************/

#ifndef Node_HPP
#define Node_HPP

#include <vector>

namespace Bellman{
    class Node {
    public:
        Node(
            unsigned int index=-1, 
            unsigned int weight = 1, 
            std::vector<double> x=std::vector<double>(2,0)
        ):
        index_(index),
        weight_(weight),
        x_(x)
        {};
        ~Node(){};

        const std::vector<double>& X(){return x_;};

    private:
        unsigned int index_;
        unsigned int weight_;
        std::vector<double> x_;
    };
};
#endif /* Node_HPP */