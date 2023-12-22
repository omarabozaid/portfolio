add_executable(poisson main.cpp)

set(LIB_DIR ${RMZ_PARENT_DIR}/build/RMZ_CFD/src)

target_include_directories(poisson PUBLIC 
    ${GEOM_DIR} 
    ${MESH_DIR} 
    ${IO_DIR} 
    ${ELEMENTS_DIR} 
    ${TOOLS_DIR} 
    ${EIGEN_DIR} 
    ${FIELDS_DIR} 
    ${BC_DIR}
    ${DISCRETIZATION_DIR}
    ${DISCRETIZATION_DIR}/cell_node_interpolation
    ${DISCRETIZATION_DIR}/node_cell_interpolation
    ${DISCRETIZATION_DIR}/laplacian
    )

target_link_libraries(poisson
    ${LIB_DIR}/IO/libIO.a 
    ${LIB_DIR}/geom/libgeom.a 
    ${LIB_DIR}/mesh/libmesh.a 
    ${LIB_DIR}/elements/libelements.a 
    ${LIB_DIR}/tools/libtools.a 
    ${LIB_DIR}/fields/libfields.a 
    ${LIB_DIR}/boundary_conditions/libBC.a
    ${LIB_DIR}/IO/libIO.a 
    ${LIB_DIR}/discretization/cell_node_interpolation/libc_n_interpolation.a
    ${LIB_DIR}/discretization/node_cell_interpolation/libn_c_interpolation.a
    ${LIB_DIR}/discretization/laplacian/liblaplacian.a
)
