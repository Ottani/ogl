cmake_minimum_required (VERSION 3.6.2)
project (ogl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

#message(STATUS "lib: ${SDL2_LIBRARIES}")
add_executable(ogl main.cpp program.cpp shader.cpp renderer.cpp window.cpp cube.cpp)

if (WIN32)
	message(STATUS "Using WIN32")
	target_link_libraries(ogl ${GLEW_LIBRARIES} opengl32 gdi32)
else()
	target_link_libraries(ogl ${SDL2_LIBRARIES} GLEW GL dl)
endif()
