//
//  cell_model.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 15/07/2022.
//

#include "cell_model.hpp"

cell_model::cell_model(){}

cell_model::~cell_model(){}

cell_model::list_of_label_list cell_model::faces_as_nodes_indices(label_list nodes_indices){
    list_of_label_list faces;
    // Tetrahedron element (cell)
    if(nodes_indices.size()==4){
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[2]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[2],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[1],nodes_indices[2],nodes_indices[3]}));
    }
    
    // Pyramid element (cell)
    else if(nodes_indices.size()==5){
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[2],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[1],nodes_indices[2],nodes_indices[4]}));
        faces.push_back(label_list({nodes_indices[2],nodes_indices[3],nodes_indices[4]}));
        faces.push_back(label_list({nodes_indices[3],nodes_indices[0],nodes_indices[4]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[4]}));
    }
    
    // Prism element (cell)
    else if(nodes_indices.size()==6){
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[4],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[2],nodes_indices[5],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[1],nodes_indices[4],nodes_indices[5],nodes_indices[2]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[2]}));
        faces.push_back(label_list({nodes_indices[3],nodes_indices[5],nodes_indices[4]}));
    }
    
    // Hexahedron element (cell)
    else if(nodes_indices.size()==8){
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[2],nodes_indices[3]}));
        faces.push_back(label_list({nodes_indices[4],nodes_indices[5],nodes_indices[6],nodes_indices[7]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[3],nodes_indices[7],nodes_indices[4]}));
        faces.push_back(label_list({nodes_indices[1],nodes_indices[2],nodes_indices[6],nodes_indices[5]}));
        faces.push_back(label_list({nodes_indices[0],nodes_indices[1],nodes_indices[5],nodes_indices[4]}));
        faces.push_back(label_list({nodes_indices[3],nodes_indices[2],nodes_indices[6],nodes_indices[7]}));
    }
    
    // Polyhedron element (cell)
    else{
        std::cout<<"Irregular cell- can not read it"<<std::endl;
    }
    
    return faces;
}

cell_model::list_of_label_list cell_model::faces_as_nodes_indices_ordered_ascending(label_list nodes_indices){
    list_of_label_list faces=this->faces_as_nodes_indices(nodes_indices);
    for(auto& f:faces){
        size_t n = f.size();
        std::sort (f.begin(), f.begin()+n);
    }
    return faces;
}

cell_model::list_of_label_list cell_model::tetrahedralize(label_list nodes_indices){
    list_of_label_list tetras;
    // Tetrahedron element (cell)
    if(nodes_indices.size()==4){
        tetras.push_back(nodes_indices);
    }
    
    // Pyramid element (cell)
    else if(nodes_indices.size()==5){
        tetras.push_back({nodes_indices[0],nodes_indices[2],nodes_indices[3],nodes_indices[4]});
        tetras.push_back({nodes_indices[0],nodes_indices[1],nodes_indices[2],nodes_indices[4]});
    }
    
    // Prism element (cell)
    else if(nodes_indices.size()==6){
        tetras.push_back({nodes_indices[1],nodes_indices[2],nodes_indices[5],nodes_indices[0]});
        tetras.push_back({nodes_indices[1],nodes_indices[4],nodes_indices[5],nodes_indices[0]});
        tetras.push_back({nodes_indices[3],nodes_indices[4],nodes_indices[5],nodes_indices[0]});
    }
    
    // Hexahedron element (cell)
    else if(nodes_indices.size()==8){
        label_list prism1({nodes_indices[3],nodes_indices[0],nodes_indices[1],nodes_indices[7],nodes_indices[4],nodes_indices[5]});
        label_list prism2({nodes_indices[5],nodes_indices[6],nodes_indices[7],nodes_indices[1],nodes_indices[2],nodes_indices[3]});
        auto t1 = cell_model::tetrahedralize(prism1);
        auto t2 = cell_model::tetrahedralize(prism2);
        for(auto& t:t1){
            tetras.push_back(t);
        }
        for(auto& t:t2){
            tetras.push_back(t);
        }
    }
    // Polyhedron element (cell)
    else{
        std::cout<<"Irregular cell- can not read it"<<std::endl;
    }
    return tetras;
}

cell_model::list_of_label_list cell_model::edges_as_nodes_indices(label_list nodes_indices){
    list_of_label_list edges;
    // Tetrahedron element (cell)
    if(nodes_indices.size()==4){
        edges.push_back(label_list({nodes_indices[0],nodes_indices[1]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[3]}));
    }
    
    // Pyramid element (cell)
    else if(nodes_indices.size()==5){
        edges.push_back(label_list({nodes_indices[0],nodes_indices[1]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[3],nodes_indices[4]}));
    }
    
    // Prism element (cell)
    else if(nodes_indices.size()==6){
        edges.push_back(label_list({nodes_indices[0],nodes_indices[1]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[5]}));
        edges.push_back(label_list({nodes_indices[3],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[3],nodes_indices[5]}));
        edges.push_back(label_list({nodes_indices[4],nodes_indices[5]}));
    }
    
    // Hexahedron element (cell)
    else if(nodes_indices.size()==8){
        edges.push_back(label_list({nodes_indices[0],nodes_indices[1]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[0],nodes_indices[4]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[2]}));
        edges.push_back(label_list({nodes_indices[1],nodes_indices[5]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[3]}));
        edges.push_back(label_list({nodes_indices[2],nodes_indices[6]}));
        edges.push_back(label_list({nodes_indices[3],nodes_indices[7]}));
        edges.push_back(label_list({nodes_indices[4],nodes_indices[5]}));
        edges.push_back(label_list({nodes_indices[4],nodes_indices[7]}));
        edges.push_back(label_list({nodes_indices[5],nodes_indices[6]}));
        edges.push_back(label_list({nodes_indices[6],nodes_indices[7]}));
    }
    
    // Polyhedron element (cell)
    else{
        std::cout<<"Irregular cell- can not read it"<<std::endl;
    }
    
    return edges;
}

cell_model::list_of_label_list cell_model::edges_as_nodes_indices_ordered_ascending(label_list nodes_indices){
    list_of_label_list edges=this->edges_as_nodes_indices(nodes_indices);
    for(auto& e:edges){
        size_t n = e.size();
        std::sort (e.begin(), e.begin()+n);
    }
    return edges;
}

cell_model::list_of_label_list cell_model::face_edges_as_nodes_indices(label_list nodes_indices){
    list_of_label_list edges;
    int n_nodes = nodes_indices.size();
    for(int counter=0;counter<n_nodes-1;counter++){
        edges.push_back(label_list({nodes_indices[counter],nodes_indices[counter+1]}));
    }
    edges.push_back(label_list({nodes_indices.back(),nodes_indices[0]}));
    return edges;
}

cell_model::list_of_label_list cell_model::face_edges_as_nodes_indices_ordered_ascending(label_list nodes_indices){
    list_of_label_list edges=this->face_edges_as_nodes_indices(nodes_indices);
    for(auto& e:edges){
        size_t n = e.size();
        std::sort (e.begin(), e.begin()+n);
    }
    return edges;
}