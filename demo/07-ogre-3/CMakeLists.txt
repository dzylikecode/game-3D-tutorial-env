get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(example ${CURRENT_DIR_NAME})
add_demo(${example} main.cpp)
# this is heuristically generated, and may not be correct
find_package(OGRE CONFIG REQUIRED)
# note: 17 additional targets are not displayed.
target_link_libraries(${example} PRIVATE OgreMain OgreBites Codec_STBI OgrePaging)
if (MY_OGRE_CONFIG_DIR)
    target_compile_definitions(${example} PRIVATE MY_OGRE_CONFIG_DIR="${PROJECT_SOURCE_DIR}/${MY_OGRE_CONFIG_DIR}")
    # set(ENV{OGRE_CONFIG_DIR} ${PROJECT_SOURCE_DIR}/${MY_OGRE_CONFIG_DIR})
endif()
