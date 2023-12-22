set(mesh_list mesh.cpp conforming_finite_volume_mesh.cpp)
add_library(mesh ${mesh_list})
target_include_directories(mesh PUBLIC ${GEOM_DIR} ${TOOLS_DIR} ${ELEMENTS_DIR} ${EIGEN_DIR})