include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/shm_obj)

set(APP_SRC
    main.cpp)
 
add_executable(shm_obj_test ${APP_SRC})
target_link_libraries(shm_obj_test shm_obj)