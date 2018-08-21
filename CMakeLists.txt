cmake_minimum_required(VERSION 2.8.12)

project(demo-video)

# 添加预处理器定义
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

# 模块对于SAFESEH映像是不安全
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

include_directories("${CMAKE_SOURCE_DIR}/dependence32/ffmpeg/include")
link_directories("${CMAKE_SOURCE_DIR}/dependence32/ffmpeg/lib")


include_directories("${CMAKE_SOURCE_DIR}/dependence32/SDL2/include")
link_directories("${CMAKE_SOURCE_DIR}/dependence32/SDL2/lib")

add_subdirectory(src)
