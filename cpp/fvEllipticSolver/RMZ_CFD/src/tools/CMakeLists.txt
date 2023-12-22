set(tools_list geom_tools.cpp kd_tree.h vector_operators_overloaded.cpp buckets_search.cpp)
add_library(tools ${tools_list})
target_include_directories(tools PUBLIC ${ELEMENTS_DIR} ${EIGEN_DIR} ${GEOM_DIR})