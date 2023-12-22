//
//  gmsh_reader.cpp
//  RMZ_CFD
//
//  Created by Omar Elsayed on 10/07/2022.
//

#include "gmsh_reader.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
gmsh_reader::gmsh_reader(string file_name):
file_name(file_name),
MSHNOD(15),
MSHEDGE(1),
MSHTRI(2),
MSHQUAD(3),
MSHTET(4),
MSHPYR(7),
MSHPRISM(6),
MSHHEX(5),
n_faces(0),
n_cells(0),
n_nodes(0)
{
    in_file.open(file_name);
    this->read_mesh_format();
    this->read_physical_names();
    this->read_points_coordinates_list();
    this->read_elements_list();
    in_file.close();
    n_boundary_cells=0;
    n_boundary_faces=0;
    this->create_mesh();
}

gmsh_reader::~gmsh_reader()
{
    delete grid;
}

void gmsh_reader::go_to_section(string section_tag)
{
    string line;
    do
    {
        std::getline(in_file,line);
        if (!in_file.good())
        {
            std::cout<<"Error wrong file contents \n";
        }
    }
    while (line.size() < 4 || line != section_tag);
}

void gmsh_reader::skip_section(string section_tag)
{
    string line;
    do
    {
        std::getline(in_file,line);
        if (!in_file.good())
        {
            std::cout<<"Error wrong file contents \n";
        }
    }
    while (line.size() < 4 || line != section_tag);
}

void gmsh_reader::read_mesh_format()
{
    this->go_to_section("$MeshFormat");
    double version;
    size_t asciiFlag, nBytes;
    in_file >> version >> asciiFlag >> nBytes;
    if (asciiFlag != 0)
    {
        std::cout<<"readMeshFormat(IFstream&)"
            << "Can only read ascii msh files."
            << std::endl;
    }
    string tag;
    in_file >>tag;
    if (tag != "$EndMeshFormat")
    {
        std::cout<<"readMeshFormat(IFstream&)"
            << "Did not find $ENDNOD tag on line "
            << std::endl;
    }
    mesh_format=version;
}

void gmsh_reader::read_points_coordinates_list()
{
    this->go_to_section("$Nodes");
    label n_nodes;
    in_file >> n_nodes;
    points.resize(n_nodes);
    points_labels.resize(n_nodes);
    int counter=0;
    for (label pointI = 0; pointI < n_nodes; pointI++)
    {
        label point_label;
        double xVal, yVal, zVal;
        in_file >> point_label >> xVal >> yVal >> zVal;
        auto& pt = points[pointI];
        pt.push_back(xVal);
        pt.push_back(yVal);
        pt.push_back(zVal);
        points_labels[counter]=point_label;
        counter+=1;
    }
    std::cout<< "Vertices: "<<std::endl;
    std::cout<< "    n° vertices:" <<points_labels.size() << std::endl;
    std::cout<<std::endl;
    string tag;
    in_file>>tag;
    if (tag != "$ENDNOD" && tag != "$EndNodes")
    {
        std::cout<<"readPoints(..)"
            << "Did not find $ENDNOD tag on line "
            << std::endl;
    }
}

void gmsh_reader::read_physical_names()
{
    this->go_to_section("$PhysicalNames");
    label n_phys_regions;
    in_file >> n_phys_regions;
    regions_i.resize(n_phys_regions);
    regions_names.resize(n_phys_regions);
    phys_types.resize(n_phys_regions);
    
    for (label i = 0; i < n_phys_regions; i++)
    {
        in_file>>phys_types[i];
        in_file>>regions_i[i];
        in_file>>regions_names[i];
        physical_number_physical_type.insert(std::make_pair(regions_i[i], phys_types[i]));
        physical_number_physical_names.insert(std::make_pair(regions_i[i], regions_names[i]));
        physical_names_physical_number.insert(std::make_pair(regions_names[i],regions_i[i]));
    }
    string tag;
    in_file>>tag;
    if (tag != "$EndPhysicalNames")
    {
        std::cout<<"readPhysicalNames(..)"
            << "Did not find $EndPhysicalNames tag on line "
            << std::endl;
    }
}

void gmsh_reader::read_elements_list()
{
    this->go_to_section("$Elements");
    label_list triPoints(3);
    label_list quadPoints(4);
    label_list tetPoints(4);
    label_list pyrPoints(5);
    label_list prismPoints(6);
    label_list hexPoints(8);
    label n_elems;
    in_file >> n_elems;
    label cell_i = 0;
    label face_i = 0;
    n_triangles_=0;
    n_quads_=0;
    n_edges_=0;
    n_tetras_=0;
    n_pyramids_=0;
    n_prisms_=0;
    n_hexas_=0;
    for (label elem_i = 0; elem_i < n_elems; elem_i++)
    {
        label elm_number, elm_type;
        label n_tags;
        label element_region;
        in_file >> elm_number >> elm_type;
        in_file>> n_tags;
        if (n_tags > 0)
        {
            in_file >> element_region;
            elements_region.push_back(element_region);
            for (label i = 1; i < n_tags; i++)
            {
                label dummy;
                in_file>> dummy;
            }
        }
        if (elm_type == MSHTRI)
        {
            in_file >> triPoints[0] >> triPoints[1] >> triPoints[2];
            face_i++;
            n_triangles_++;
            elements_surf.push_back(triPoints);
            elements_surf_region.push_back(element_region);
            elements.push_back(triPoints);
        }
        else if (elm_type == MSHQUAD)
        {
            in_file
                >> quadPoints[0] >> quadPoints[1] >> quadPoints[2]
                >> quadPoints[3];
            face_i++;
            n_quads_++;
            elements_surf.push_back(quadPoints);
            elements_surf_region.push_back(element_region);
            elements.push_back(quadPoints);
        }
        else if (elm_type == MSHTET)
        {
            in_file
                >> tetPoints[0] >> tetPoints[1] >> tetPoints[2]
                >> tetPoints[3];
            n_tetras_++;
            cell_i++;
            elements_vol.push_back(tetPoints);
            elements_vol_region.push_back(element_region);
            elements.push_back(tetPoints);
        }
        else if (elm_type == MSHPYR)
        {
            in_file
                >> pyrPoints[0] >> pyrPoints[1] >> pyrPoints[2]
                >> pyrPoints[3] >> pyrPoints[4];
            n_pyramids_++;
            cell_i++;
            elements_vol.push_back(pyrPoints);
            elements_vol_region.push_back(element_region);
            elements.push_back(pyrPoints);
        }
        else if (elm_type == MSHPRISM)
        {
            in_file
                >> prismPoints[0] >> prismPoints[1] >> prismPoints[2]
                >> prismPoints[3] >> prismPoints[4] >> prismPoints[5];
            n_prisms_++;
            cell_i++;
            elements_vol.push_back(prismPoints);
            elements_vol_region.push_back(element_region);
            elements.push_back(prismPoints);
        }
        else if (elm_type == MSHHEX)
        {
            in_file
                >> hexPoints[0] >> hexPoints[1]
                >> hexPoints[2] >> hexPoints[3]
                >> hexPoints[4] >> hexPoints[5]
                >> hexPoints[6] >> hexPoints[7];
            n_hexas_++;
            cell_i++;
            elements_vol.push_back(hexPoints);
            elements_vol_region.push_back(element_region);
            elements.push_back(hexPoints);
        }
        else
        {
            std::cout<< "Unhandled element " << elm_type << std::endl;
        }
    }
    string tag;
    in_file>>tag;
    if (tag != "$ENDELM" && tag != "$EndElements")
    {
        std::cout<<"readPoints(..)"
            << "Did not find $ENDELEMENTS tag on line "
            << std::endl;
    }
    std::cout<< "Physical faces:" << std::endl
    << "    total:" << face_i <<  std::endl
    << "    tri  :" << n_triangles_ <<  std::endl
    << "    quads:" << n_quads_ <<  std::endl
    << std::endl;
    std::cout<< "Cells:" << std::endl
    << "    total:" << cell_i <<  std::endl
    << "    hex  :" << n_hexas_ <<  std::endl
    << "    prism:" << n_prisms_ <<  std::endl
    << "    pyr  :" << n_pyramids_ <<  std::endl
    << "    tet  :" << n_tetras_ <<  std::endl
    << std::endl;
}

double gmsh_reader::version(){
    return mesh_format;
}

std::vector<std::vector<double>> gmsh_reader::points_coordinates_list(){
    return points;
}

gmsh_reader::label gmsh_reader::n_regions(){
    return n_phys_regions;
}

gmsh_reader::label_list gmsh_reader::physical_types(){
    return phys_types;
}

gmsh_reader::label_list gmsh_reader::regions_I(){
    return regions_i;
}

std::vector<std::string> gmsh_reader::regions_names_list(){
    return regions_names;
}

gmsh_reader::label_list gmsh_reader::element_region_list(){
    return elements_region;
}

void gmsh_reader::create_mesh(){
    this->create_mesh_vertices();
    this->create_faces_lookup();
    this->create_mesh_faces();
    this->create_mesh_cells();
    this->update_connectivity();
    grid = new conf_fv_mesh(
                            mesh_cells_,
                            mesh_faces_,
                            mesh_phys_faces,
                            mesh_vertices_,
                            mesh_vertices_centers_,
                            boundary_nodes,
                            physical_label_physical_faces_ptrs,
                            physical_name_physical_faces_ptrs,
                            physical_number_physical_type,
                            physical_number_physical_names,
                            n_boundary_nodes,
                            n_boundary_cells,
                            n_boundary_faces,
                            n_triangles_,
                            n_quads_,
                            n_edges_,
                            n_tetras_,
                            n_pyramids_,
                            n_prisms_,
                            n_hexas_
                            );
}

void gmsh_reader::create_mesh_vertices(){
    for(auto xyz:points){
        mesh_vertices_centers_.push_back(point(xyz));
    }
    label n_nodes_=0;
    for(auto& pt:mesh_vertices_centers_){
        mesh_vertices_.push_back(node(pt,int(n_nodes_)));
        n_nodes_+=1;
    }
}

void gmsh_reader::create_faces_lookup(){
    cell_nodes_indices = elements_vol;
    label face_index=0;
    cell_faces_indices.resize(elements_vol.size());
    cell_model dummy_cell_model;
    label cell_index = 0;
    for(auto elem:elements_vol){
        list_of_label_list ordered_faces = dummy_cell_model.faces_as_nodes_indices_ordered_ascending(elem);
        list_of_label_list un_ordered_faces = dummy_cell_model.faces_as_nodes_indices(elem);
        cell_faces_indices[cell_index].clear();
        int local_index=0;
        for(auto f:ordered_faces){
            if(nodes_indices_face_index_map.find(f) == nodes_indices_face_index_map.end()){
                nodes_indices_face_index_map.insert(std::make_pair(f , face_index) );
                cell_faces_indices[cell_index].push_back(face_index);
                face_nodes_indices.push_back(un_ordered_faces[local_index]);
                face_index+=1;
            }
            else{
                label stored_face_index = nodes_indices_face_index_map.find(f)->second;
                cell_faces_indices[cell_index].push_back(stored_face_index);
            }
            local_index+=1;
        }
        cell_index+=1;
    }
    n_faces = face_index;
    n_cells = cell_nodes_indices.size();
    owner_neighbour.resize(n_faces);
    is_boundary_face.resize(n_faces);
    face_area.resize(n_faces);
    faces_normals.resize(n_faces);
    faces_cells_distances.resize(n_faces);
    is_boundary_cell.resize(n_cells,false);
    cell_volume.resize(n_cells);
    cell_face_outward_sign.resize(n_cells);
    cell_centers_.resize(n_cells);
    face_centers_.resize(n_faces);
    faces_ordered_indices.resize(n_faces,-1);
    
    int face_index_in_array = 0;
    for(auto& pair:nodes_indices_face_index_map){
        auto face_idx = pair.second;
        faces_ordered_indices[face_idx]=face_index_in_array;
        face_index_in_array+=1;
    }
    
    for(auto& pair:nodes_indices_face_index_map){
        auto face_index = pair.second;
        vector_of_double face_i_center(3,0);
        vector_of_double face_i_normal(3,0);
        list_of_vector_of_doubles face_points;
        double face_i_area = 0.0 ;
        for(auto node_idx:pair.first){
            auto idx=node_idx-1;
            auto node_xyz = mesh_vertices_centers_[idx].coordinates();
            face_points.push_back(node_xyz);
        }
        fit_plane(face_points, face_i_normal, face_i_center);
        area(face_points, face_i_center,face_i_area);
        face_centers_[face_index]=point(face_i_center);
        is_boundary_face[face_index]=false;
        owner_neighbour[face_index]=label_list(2,-1);
        faces_cells_distances[face_index]=vector_of_double(2,1e6);
        face_area[face_index]=face_i_area;
        faces_normals[face_index]=face_i_normal;
    }
    
    
    for(auto& boundary_label_boundary_name:physical_number_physical_names){
        label boundary_idx = boundary_label_boundary_name.first;
        boundary_faces_map.insert(std::make_pair(boundary_idx,label_list()));
        auto& label_faces_list_ = boundary_faces_map.find(boundary_idx)->second;
        label_faces_list_.clear();
    }
    
    label b_f_local_idx = 0;
    for(auto b_f:elements_surf){
        label boundary_label = elements_surf_region[b_f_local_idx];
        label n_nodes_face = b_f.size();
        std::sort (b_f.begin(), b_f.begin()+n_nodes_face);
        label face_index = nodes_indices_face_index_map.find(b_f)->second;
        label_list& b_f_list = boundary_faces_map.find(boundary_label)->second;
        b_f_list.push_back(face_index);
        is_boundary_face[face_index]=true;
        n_boundary_faces+=1;
        owner_neighbour[face_index]=label_list(1,-1);
        faces_cells_distances[face_index]=vector_of_double(1,1e6);
        b_f_local_idx+=1;
    }
    
    for(label cell_i = 0; cell_i < n_cells; cell_i++){
        auto nodes_indices = cell_nodes_indices[cell_i];
        std::vector<double> p(3,0);
        for(auto node_idx:nodes_indices){
            auto idx=node_idx-1;
            auto node_xyz = mesh_vertices_centers_[idx].coordinates();
            p=p+node_xyz;
        }
        p=p/nodes_indices.size();
        cell_centers_[cell_i]=point(p);
        auto faces_indices = cell_faces_indices[cell_i];
        label n_cell_faces = faces_indices.size();
        cell_face_outward_sign[cell_i]=label_list(n_cell_faces,1);
        int local_face_index = 0 ;
        for(auto face_index:faces_indices){
            auto face_center = face_centers_[face_index];
            auto vec_c_f = p-face_center.coordinates();
            double d_c_f = magnitude(vec_c_f);
            int sign_c_f = 1 ;
            double check = dot( vec_c_f, faces_normals[face_index]);
            if(check<0){
                sign_c_f = -1;
            }
            if(is_boundary_face[face_index]==true){
                is_boundary_cell[cell_i]=true;
                n_boundary_cells+=1;
                if(owner_neighbour[face_index][0]==-1){
                    owner_neighbour[face_index][0]=cell_i;
                    faces_cells_distances[face_index][0]=d_c_f;
                    cell_face_outward_sign[cell_i][local_face_index]=sign_c_f;
                }
                else{
                    std::cout<<"Boundary face sharing more than one cell "<<face_index<<std::endl;
                }
            }
            else{
                if(owner_neighbour[face_index][0]==-1){
                    owner_neighbour[face_index][0]=cell_i;
                    faces_cells_distances[face_index][0]=d_c_f;
                    cell_face_outward_sign[cell_i][local_face_index]=sign_c_f;
                }
                else if(owner_neighbour[face_index][1]==-1){
                    owner_neighbour[face_index][1]=cell_i;
                    faces_cells_distances[face_index][1]=d_c_f;
                    cell_face_outward_sign[cell_i][local_face_index]=sign_c_f;
                }
                else{
                    std::cout<<"Internal face sharing more than two cells "<<face_index<<std::endl;
                    std::cout<<cell_i<<"  ";
                    for(auto& idx:owner_neighbour[face_index]){
                        std::cout<<idx<<"  ";
                    }
                    std::cout<<std::endl;
                }
            }
            local_face_index+=1;
            list_of_vector_of_doubles cell_faces_centers;
            list_of_vector_of_doubles cell_faces_normals;
            list_of_vector_of_doubles cell_nodes_pts;
            vector_of_double cell_faces_areas;
            
            for(auto face_index:cell_faces_indices[cell_i]){
                cell_faces_centers.push_back(face_centers_[face_index].coordinates());
                cell_faces_normals.push_back(faces_normals[face_index]);
                cell_faces_areas.push_back(face_area[face_index]);
            }
            
            for(auto node_idx:cell_nodes_indices[cell_i]){
                cell_nodes_pts.push_back(mesh_vertices_centers_[node_idx-1].coordinates());
            }
            
            cell_volume[cell_i]= poly_volume(cell_nodes_pts, 4);
        }
    }
}

void gmsh_reader::create_mesh_faces(){
    std::cout<<"n° faces is "<<n_faces<<std::endl;
    for(auto& pair:nodes_indices_face_index_map){
        auto face_idx = pair.second;
        auto& p_f = face_centers_[face_idx];
        auto axis_f = faces_normals[face_idx];
        auto is_boundary = is_boundary_face[face_idx];
        auto nodes_indices = face_nodes_indices[face_idx];
        auto distances = faces_cells_distances[face_idx];
        auto area = face_area[face_idx];
        nodes_ptrs_list face_nodes;
        for(auto node_idx:nodes_indices){
            face_nodes.push_back(&mesh_vertices_[node_idx-1]);
        }
        mesh_faces_.push_back(face(
                                   p_f,
                                   face_nodes,
                                   axis_f,
                                   int(face_idx),
                                   is_boundary,
                                   distances,
                                   area
                                   )
                              );
    }
    
    for(auto& pair:physical_number_physical_names){
        auto phys_label=pair.first;
        auto phys_name=pair.second;
        physical_label_physical_faces_ptrs.insert(std::make_pair(phys_label, faces_ptrs_list()));
        faces_ptrs_list& dummy_faces_ptrs_list = physical_label_physical_faces_ptrs.find(phys_label)->second;
        dummy_faces_ptrs_list.clear();
        physical_name_physical_faces_ptrs.insert(std::make_pair(phys_name, dummy_faces_ptrs_list));
    }
    
    
    for(auto& pair:boundary_faces_map){
        auto faces_indices = pair.second;
        auto phys_idx = pair.first;
        auto phys_name = physical_number_physical_names.find(phys_idx)->second;
        
        auto& phys_faces_list_by_label = physical_label_physical_faces_ptrs.find(phys_idx)->second;
        auto& phys_faces_list_by_name  = physical_name_physical_faces_ptrs.find(phys_name)->second;
        
        for(auto face_idx:faces_indices){
            int face_index_in_array = int(faces_ordered_indices[face_idx]) ;
            phys_faces_list_by_label.push_back(&mesh_faces_[face_index_in_array]);
            phys_faces_list_by_name.push_back(&mesh_faces_[face_index_in_array]);
            mesh_phys_faces.push_back(&mesh_faces_[face_index_in_array]);
        
        }
    }
}

void gmsh_reader::create_mesh_cells(){
    int n_cells = int(elements_vol.size());
    for(int cell_i = 0; cell_i < n_cells; cell_i++){
        auto& cell_center = cell_centers_[cell_i];
        nodes_ptrs_list cell_nodes;
        faces_ptrs_list cell_faces;
        double vol = cell_volume[cell_i];
        auto is_boundary = is_boundary_cell[cell_i];
        for(auto& node_idx:elements_vol[cell_i]){
            cell_nodes.push_back(&mesh_vertices_[node_idx-1]);
        }
        
        for(auto& face_idx:cell_faces_indices[cell_i]){
            int face_index_in_array = int(faces_ordered_indices[face_idx]) ;
            cell_faces.push_back(&mesh_faces_[face_index_in_array]);
        }
        
        mesh_cells_.push_back(cell(
                                   cell_center,
                                   cell_nodes,
                                   cell_faces,
                                   cell_i,
                                   vol,
                                   is_boundary
                                   )
                              );
    }
}

void gmsh_reader::update_connectivity(){
    for(auto& f:mesh_faces_){
        auto face_owner_neighbor_indices = owner_neighbour[f.index()];
        for(auto& cell_index:face_owner_neighbor_indices){
            f.append(&mesh_cells_[cell_index]);
        }
    }
    for(auto& c:mesh_cells_){
        cells_ptrs_list neighbouring_cells;
        label cell_index = c.index();
        auto cell_faces_ptrs = c.neighbouring_faces();
        auto cell_nodes = c.cell_nodes();
        for(auto& n:cell_nodes){
            mesh_vertices_[n->index()].append(&c);
        }
        for(auto face_ptr:cell_faces_ptrs){
             if(face_ptr->is_internal_face()){
                auto owner_neighbor_ptrs = face_ptr->owner_neighbour();
                for(auto check_cell:owner_neighbor_ptrs){
                    if(check_cell->index()!=cell_index){
                        neighbouring_cells.push_back(&mesh_cells_[check_cell->index()]);
                    }
                }
            }
            c.set_neighbours(neighbouring_cells);
        }
    }
    for(auto&f:mesh_phys_faces){
        auto face_xyz = f->face_center().coordinates();
        for(auto n:f->face_nodes()){
            auto node_idx = n->index();
            mesh_vertices_[node_idx].set_is_boundary_node(true);
            mesh_vertices_[node_idx].append(f);
        }
    }
    boundary_nodes.clear();
    n_boundary_nodes = 0;
    for(auto& n:mesh_vertices_){
        if(n.is_boundary_node()){
            boundary_nodes.push_back(&n);
            n_boundary_nodes+=1;
        }
    }
}

gmsh_reader::conf_fv_mesh& gmsh_reader::computational_grid(){
    return *grid;
}

gmsh_reader::label_label_map gmsh_reader::physical_label_physical_type(){
    return physical_number_physical_type;
}