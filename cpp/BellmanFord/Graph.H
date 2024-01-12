/*****************************************************
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2024-01-12
 * Bellman Ford                                
 *                                                     
 ******************************************************/

#ifndef Graph_HPP
#define Graph_HPP

#include "Node.H"
#include "Edge.H"

#include <vector>

namespace Bellman{
    class Graph {
    public:
        typedef std::vector<Node> NodesVector;
        typedef std::vector<Edge> EdgesVector;
        
        Graph(const NodesVector& nodes, const EdgesVector& edges)
        :
        nodes_(nodes),edges_(edges)
        {};
        ~Graph();
    private:
        const NodesVector& nodes_;
        const EdgesVector& edges_;
    };
};
#endif /* Graph_HPP */