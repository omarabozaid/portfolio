set(fields_list cell_field.cpp cell_vector_field.cpp face_field.cpp face_vector_field.cpp node_field.cpp)
add_library(fields ${fields_list})
target_include_directories(fields PUBLIC ${GEOM_DIR} ${ELEMENTS_DIR} ${MESH_DIR} ${BC_DIR} ${TOOLS_DIR} ${EIGEN_DIR} ${IO_DIR} ${BC_DIR})