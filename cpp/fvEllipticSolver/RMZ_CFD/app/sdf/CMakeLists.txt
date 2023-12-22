add_executable(sdf main.cpp)

set(LIB_DIR ${RMZ_PARENT_DIR}/build/RMZ_CFD/src)

target_include_directories(sdf PUBLIC ${GEOM_DIR} ${MESH_DIR} ${IO_DIR} ${ELEMENTS_DIR} ${TOOLS_DIR} ${EIGEN_DIR} ${FIELDS_DIR} ${BC_DIR})

target_link_libraries(sdf
    ${LIB_DIR}/IO/libIO.a 
    ${LIB_DIR}/geom/libgeom.a 
    ${LIB_DIR}/mesh/libmesh.a 
    ${LIB_DIR}/elements/libelements.a 
    ${LIB_DIR}/tools/libtools.a 
    ${LIB_DIR}/fields/libfields.a 
    ${LIB_DIR}/boundary_conditions/libBC.a
    ${LIB_DIR}/IO/libIO.a 
)
