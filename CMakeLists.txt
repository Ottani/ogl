cmake_minimum_required (VERSION 3.6.2)
project (ogl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/usr/local/include/")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(ogl main.cpp )
target_link_libraries(ogl glfw GLEW GL dl)

