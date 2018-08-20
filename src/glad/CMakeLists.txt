project(glad)

find_package(OpenGL)


set(glad_HEADERS
	src/glad.c
	src/glad_wgl.c
)

set(app_SOURCES
	include/glad/glad.h
	include/glad/glad_wgl.h)
	
set(glad_include_dirs
	PRIVATE ${OPENGL_INCLUDE_DIR})
	

	
	
	
add_library(${PROJECT_NAME}  SHARED
	${app_SOURCES}
	${glad_HEADERS})
	
	
	
target_include_directories(${PROJECT_NAME}
	PUBLIC include
	${glad_include_dirs})
	
target_compile_definitions(${PROJECT_NAME}
	PRIVATE GLAD_GLAPI_EXPORT_BUILD)
	
	
target_link_libraries(${PROJECT_NAME}
	${OPENGL_gl_LIBRARY})
	