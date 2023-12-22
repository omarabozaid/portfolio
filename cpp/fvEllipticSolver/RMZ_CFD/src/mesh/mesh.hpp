//
//  mesh.hpp
//  test_xCode
//
//  Created by Omar Elsayed on 11/05/2022.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <vector>
#include <map>

#include "point.hpp"
#include "node.hpp"
#include "cell.hpp"
#include "face.hpp"

class mesh{
public:
    mesh();
    ~mesh();
    
    virtual std::vector<face> faces_list()=0;
    virtual std::vector<cell> cells_list()=0;
    virtual std::vector<node> nodes_list()=0;
    
    virtual std::vector<face>& faces_list_ref()=0;
    virtual std::vector<cell>& cells_list_ref()=0;
    virtual std::vector<node>& nodes_list_ref()=0;
    
    
    virtual int n_faces()=0;
    virtual int n_cells()=0;
    virtual int n_vertices()=0;
    
    virtual int n_boundary_faces()=0;
    virtual int n_boundary_cells()=0;
    virtual int n_boundary_nodes()=0;
    
    virtual std::vector<node*> boundary_nodes_list()=0;
    virtual std::vector<face*> boundary_faces_list()=0;
    
    virtual int n_triangles()=0;
    virtual int n_quads()=0;
    virtual int n_edges()=0;
    virtual int n_tetras()=0;
    virtual int n_pyramids()=0;
    virtual int n_prisms()=0;
    virtual int n_hexas()=0;

    virtual void handle_zero_volume_cells(double vol_tol=1e-10)=0;
};

#endif /* mesh_hpp */
