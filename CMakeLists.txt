cmake_minimum_required(VERSION 2.8)

project(ShmObj CXX)

set(CXX_FLAGS
    -g
    -Wall
    -Werror
    )

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(example/shm_obj_test)
add_subdirectory(example/shm_pointer_test)
add_subdirectory(example/shm_pointer_v2_test)
add_subdirectory(src/shm_obj)
