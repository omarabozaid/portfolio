set (SDF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdf)
set (LAPLACIAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/laplacian)
set (LAPLACIAN_SDF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/laplacian_sdf)
set (CUT_CELL_MESHER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cut_cell_mesher)

add_subdirectory (${SDF_DIR})
add_subdirectory (${LAPLACIAN_DIR})
