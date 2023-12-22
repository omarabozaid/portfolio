set(elements_list node.cpp face.cpp cell.cpp cell_model.cpp)
add_library(elements ${elements_list})
target_include_directories(elements PUBLIC ${GEOM_DIR} ${TOOLS_DIR} ${EIGEN_DIR})