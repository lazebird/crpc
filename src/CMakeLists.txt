aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
add_library(crpc SHARED ${SRC})
target_include_directories(crpc PUBLIC ${PROJECT_SOURCE_DIR}/include/crpc)
