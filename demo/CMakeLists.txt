function(add_demo example files)
    add_executable(${example} ${files})
    # target_include_directories(${example} PRIVATE ${Easy3D_INCLUDE_DIR})
    target_link_libraries(${example} PRIVATE core)
    # this is heuristically generated, and may not be correct
    find_package(bgfx CONFIG REQUIRED)
    # note: 1 additional targets are not displayed.
    target_link_libraries(${example} PRIVATE bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode)

    # this is heuristically generated, and may not be correct
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${example} PRIVATE glfw)

    # Or use the header-only version
    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(${example} PRIVATE spdlog::spdlog_header_only)
endfunction()

file(GLOB SUB_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${SUB_DIRS})
    if(IS_DIRECTORY ${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()