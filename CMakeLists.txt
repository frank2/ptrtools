cmake_minimum_required(VERSION 3.10)

project(ptrtools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(TEST_PTRTOOLS "Enable testing for ptrtools." OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADER_FILES})
add_library(ptrtools INTERFACE)
target_include_directories(ptrtools INTERFACE
  "${PROJECT_SOURCE_DIR}/include"
)

if (TEST_PTRTOOLS)
  enable_testing()
  add_executable(testptrtools ${PROJECT_SOURCE_DIR}/test/main.cpp ${PROJECT_SOURCE_DIR}/test/framework.hpp)
  target_link_libraries(testptrtools PUBLIC ptrtools)
  target_include_directories(testptrtools PUBLIC
    "${PROJECT_SOURCE_DIR}/test"
  )
  add_test(NAME testptrtools COMMAND testptrtools)
endif()
