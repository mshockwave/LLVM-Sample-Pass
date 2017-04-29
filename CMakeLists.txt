cmake_minimum_required(VERSION 3.7)
project(LLVMPassSample)

set(CMAKE_CXX_STANDARD 11)

set(LLVM_DIST_PATH "/usr/local/opt/llvm-clang/current"
        CACHE STRING "LLVM distribution install path")

set(LLVM_DIR ${LLVM_DIST_PATH}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Using LLVM version ${LLVM_PACKAGE_VERSION}")
list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib)
include(HandleLLVMOptions)
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(RedundantStore)