project(simple-app)


set(HEADERS
	main.h
)

set(SOURCES
	main.cpp)
	
	
add_executable(${PROJECT_NAME} 
	${SOURCES}
	${HEADERS})
	
target_link_libraries(${PROJECT_NAME} 
	avformat
	avcodec
	avutil
	swscale
	SDL2
)