cmake_minimum_required (VERSION 3.6.2)
project (ogl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(ogl main.cpp program.cpp shader.cpp geometry.cpp window.cpp)

if (WIN32)
	message(STATUS "Using WIN32")
	target_link_libraries(ogl glfw ${GLEW_LIBRARIES} opengl32 gdi32)
else()
	target_link_libraries(ogl glfw GLEW GL dl)
endif()
