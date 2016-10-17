include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_obj)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_pointer)

set(LIB_SRC
    shm_pointer_count.cpp)

set(LIBRARY_OUTPUT_PATH lib)
add_library(shm_pointer STATIC ${LIB_SRC})
