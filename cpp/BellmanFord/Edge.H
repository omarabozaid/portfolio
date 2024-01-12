/*****************************************************
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2024-01-12
 * Bellman Ford: Edge
 *                                                     
 ******************************************************/

#ifndef Edge_HPP
#define Edge_HPP

#include "Node.H"

namespace Bellman{
    class Edge {
    public:
        Edge(Node* src=nullptr, Node* dst=nullptr, double length=0)
        :
        src_(src),dst_(dst),length_(length)
        {}
        ;
        ~Edge(){};

        void Length(){
            auto dx = src_->X()[0] - dst_->X()[0];
            auto dy = src_->X()[1] - dst_->X()[1];
            length_ = std::sqrt(dx*dx+dy*dy);
        }

        double Length() const {
            return length_;
        }

    private:
        Node* src_; 
        Node* dst_; 
        double length_;
    };
};

#endif /* Edge_HPP */