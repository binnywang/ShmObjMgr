include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_obj)

set(LIB_SRC
    shm_pool.cpp
    system.cpp)

set(LIBRARY_OUTPUT_PATH lib)
add_library(shm_obj STATIC ${LIB_SRC})