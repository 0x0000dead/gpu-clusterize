
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set clang windows
#set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

project(HelloWorld LANGUAGES CXX CUDA)
#project(HelloWorld LANGUAGES  CXX)
find_package(CUDA  REQUIRED)
include_directories(HelloWorld ${CUDA_INCLUDE_DIRS})
# set nvcc cuda machine 64
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-m64)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-arch=sm_61)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-arch=sm_75)
add_executable(HelloWorld main.cpp KMedoids.cpp KMedoids.h dist.cu KNN.cpp KNN.h Points.cpp Points.h utils/CostCalculator.cpp utils/CostCalculator.h utils/Data.h utils/SubgradientCalculator.cpp utils/SubgradientCalculator.h  utils/helper.h utils/constants.h utils/LambdaCalculator.cpp utils/LambdaCalculator.h)

#set_target_properties(HelloWorld PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(HelloWorld ${CUDA_LIBRARIES})
