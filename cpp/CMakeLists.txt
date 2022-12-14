
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set clang windows
#set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

project(HelloWorld LANGUAGES CXX CUDA)
#project(HelloWorld LANGUAGES  CXX)
find_package(CUDA  REQUIRED)
include_directories(HelloWorld ${CUDA_INCLUDE_DIRS})

add_executable(HelloWorld main.cpp KMedoids.cpp KMedoids.h dist.cu)

#set_target_properties(HelloWorld PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(HelloWorld ${CUDA_LIBRARIES})
