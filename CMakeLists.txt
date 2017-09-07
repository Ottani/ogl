cmake_minimum_required (VERSION 3.6.2)
project (ogl)

#ccmake -DCMAKE_TOOLCHAIN_FILE=CMake/i686-w64-mingw32.cmake .

#SET(CMAKE_SYSTEM_NAME Windows)
#SET(CMAKE_SYSTEM_VERSION 1)
#SET(CMAKE_C_COMPILER   "i686-w64-mingw32-gcc")
#SET(CMAKE_CXX_COMPILER "i686-w64-mingw32-g++")
#SET(CMAKE_RC_COMPILER  "i686-w64-mingw32-windres")
#SET(CMAKE_RANLIB       "i686-w64-mingw32-ranlib")

#SET(CMAKE_FIND_ROOT_PATH "/usr/i686-w64-mingw32")
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/usr/local/include/" ".")

find_package(glfw3 REQUIRED)
#find_package(OpenGL REQUIRED)

add_executable(ogl main.cpp glad.c)
target_link_libraries(ogl glfw)

#t#arget_link_libraries(ogl ${OPENGL_gl_LIBRARY})
