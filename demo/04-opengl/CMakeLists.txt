get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(example ${CURRENT_DIR_NAME})
add_demo(${example} main.cpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${example} PRIVATE glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${example} PRIVATE glm::glm)