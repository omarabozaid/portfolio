/*****************************************************
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2024-01-12
 * Bellman Ford shortest path algorithm
 *                                                     
 ******************************************************/

#ifndef BellmanFord_HPP
#define BellmanFord_HPP

#include "Graph.H"
#include "Node.H"
#include "Edge.H"

#include<vector>
#include<limits>
#include<iostream>

namespace Bellman{
    class BellmanFord {
    public:
        BellmanFord(const Graph& graph):graph_(graph){};
        ~BellmanFord();

        void ShortestPath(){};
    private:
        const Graph& graph_;
    };
};

#endif /* BellmanFord_HPP */