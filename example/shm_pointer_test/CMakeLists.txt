include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_obj)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_pointer)

set(APP_SRC
    main.cpp)
 
add_executable(shm_pointer_test ${APP_SRC})

target_link_libraries(shm_pointer_test shm_obj)
