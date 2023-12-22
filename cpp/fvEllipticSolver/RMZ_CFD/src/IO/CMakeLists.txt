set(io_list boundary_xyz_values_reader.cpp gmsh_reader.cpp mesh_writer.cpp points_list_reader.cpp stl_reader.cpp vtk_writer.cpp table_reader.cpp export_gmsh.cpp)
add_library(IO ${io_list})
target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${GEOM_DIR} ${TOOLS_DIR} ${FIELDS_DIR} ${MESH_DIR} ${ELEMENTS_DIR} ${EIGEN_DIR} ${BC_DIR})