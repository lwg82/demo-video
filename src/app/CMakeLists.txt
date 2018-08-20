project(app)


set(app_HEADERS
	targetver.h
	stdafx.h
	graphic-opengl.h
	main.h
)

set(app_SOURCES
	stdafx.cpp
	graphic-opengl.cpp
	main.cpp)
	
	
add_executable(app WIN32
	${app_SOURCES}
	${app_HEADERS})