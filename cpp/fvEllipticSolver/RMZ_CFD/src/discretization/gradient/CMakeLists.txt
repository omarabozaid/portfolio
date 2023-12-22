add_library(grad gradient.cpp)
target_include_directories(grad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${TOOLS_DIR} ${EIGEN_DIR} ${GEOM_DIR} ${MESH_DIR} ${BC_DIR} ${ELEMENTS_DIR} ${IO_DIR} ${FIELDS_DIR})