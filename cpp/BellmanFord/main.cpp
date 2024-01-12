/*****************************************************
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2024-01-12
 * Bellman Ford : test
 *                                                     
 ******************************************************/

#include "Node.H"
#include "Edge.H"
#include "Graph.H"

#include <vector>
#include <iostream>

void CreateGraph(
    std::vector<Bellman::Node>& nodes,
    std::vector<Bellman::Edge>& edges)
    {

    Bellman::Node node1(0,1,std::vector<double>({0,3}));
    Bellman::Node node2(1,1,std::vector<double>({-3,2}));
    Bellman::Node node3(2,1,std::vector<double>({11,5}));
    Bellman::Node node4(3,1,std::vector<double>({-2,1}));
    Bellman::Node node5(4,1,std::vector<double>({0.5,4}));
    Bellman::Node node6(5,1,std::vector<double>({-2.5,1}));
    Bellman::Node node7(6,1,std::vector<double>({13,13}));

    nodes.push_back(node1);
    nodes.push_back(node2);
    nodes.push_back(node3);
    nodes.push_back(node4);
    nodes.push_back(node5);
    nodes.push_back(node6);
    nodes.push_back(node7);

    edges.push_back(Bellman::Edge (&nodes[0],&nodes[1]));
    edges.push_back(Bellman::Edge (&nodes[0],&nodes[2]));
    edges.push_back(Bellman::Edge (&nodes[1],&nodes[3]));
    edges.push_back(Bellman::Edge (&nodes[2],&nodes[3]));
    edges.push_back(Bellman::Edge (&nodes[3],&nodes[4]));
    edges.push_back(Bellman::Edge (&nodes[4],&nodes[5]));
    edges.push_back(Bellman::Edge (&nodes[4],&nodes[6]));

    for(auto& edge:edges){
        edge.Length();
    }

};

int main(){
    std::vector<Bellman::Node> nodes;
    std::vector<Bellman::Edge> edges;
    CreateGraph(nodes,edges);
    for(const auto& edge:edges){
        std::cout<<" edge length is "<<edge.Length()<<"\n";
    }
    return 0;
};