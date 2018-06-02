aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
add_library(crpc SHARED ${SRC})
#add_library(crpc ${SRC})

install(TARGETS crpc
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION libstatic
)
set(head_files
	${PROJECT_SOURCE_DIR}/include/crpc.h
)
install(FILES ${head_files} DESTINATION  include)

