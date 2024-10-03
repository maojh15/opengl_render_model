cmake_minimum_required(VERSION 3.12)
project(TEST_OPENGL)
set(CMAKE_CXX_STANDARD 17)
# add_executable(main MACOSX_BUNDLE main.cpp)
add_executable(main main.cpp)

# set(CMAKE_MACOSX_RPATH OFF)
# set(INSTALL_RPATH ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# set dir
set(IMGUI_DIR "/Users/maojiahao/cppLibraries/package/imgui-master")
set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(BREW_LIBRARIES_DIR "/opt/homebrew/lib")
set(GLAD_DIR "/Users/maojiahao/cppLibraries/package/glad")
set(OPENGL_TOOLS_INCLUDE_DIR "/Users/maojiahao/cppLibraries/include")
set(ASSIMP_INCLUDE_DIR "/Users/maojiahao/cppLibraries/package/assimp/build_install/include")
set(ASSIMP_LIBRARY_DIR "/Users/maojiahao/cppLibraries/package/assimp/build_install/lib")

# add imgui lib
add_library(imgui_lib 
    "${IMGUI_DIR}/imgui.cpp" 
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")
target_include_directories(imgui_lib PUBLIC 
    ${SDL2_INCLUDE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends)
target_link_directories(imgui_lib PUBLIC ${BREW_LIBRARIES_DIR})
target_link_libraries(imgui_lib PUBLIC SDL2 SDL2_image "-framework OpenGL")

# add glad lib
add_subdirectory(${GLAD_DIR} ${CMAKE_BINARY_DIR}/glad_build)

# link imgui and glad
target_include_directories(main PUBLIC ${OPENGL_TOOLS_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})
target_link_directories(main PUBLIC ${ASSIMP_LIBRARY_DIR})

target_link_libraries(main PUBLIC imgui_lib glad_lib assimp)

install(TARGETS main
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

install(FILES ${ASSIMP_LIBRARY_DIR}/libassimp.dylib
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${ASSIMP_LIBRARY_DIR}/libassimp.dylib
                ${CMAKE_CURRENT_BINARY_DIR}/libassimp.dylib)

# set(CUR_TARGET main)
# set_target_properties(${CUR_TARGET} PROPERTIES
#     BUNDLE True
#     MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${CUR_TARGET}
#     MACOSX_BUNDLE_BUNDLE_NAME ${CUR_TARGET}
#     MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
#     MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
#     MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/customtemplate.plist.in
# )