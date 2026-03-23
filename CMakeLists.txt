cmake_minimum_required(VERSION 3.21)

project(
    DualWieldParryingNG
    VERSION 1.0.1
    LANGUAGES CXX
)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ########################################################################################################################
# ## Build options
# ########################################################################################################################
message(STATUS "Options:")
option(AUTO_PLUGIN_DEPLOYMENT "Copy the build output and addons to env:${PROJECT_NAME}OutputDir." OFF)
option(ZIP_TO_DIST "Zip the base mod and addons to their own archive in dist." OFF)
option(AIO_ZIP_TO_DIST "Zip the base mod and addons to an AIO archive in dist." OFF)
message(STATUS "\tAuto plugin deployment: ${AUTO_PLUGIN_DEPLOYMENT}")
message(STATUS "\tZip to dist: ${ZIP_TO_DIST}")
message(STATUS "\tAIO Zip to dist: ${AIO_ZIP_TO_DIST}")

# #######################################################################################################################
# # Add CMake features
# #######################################################################################################################
include(XSEPlugin)

# #######################################################################################################################
# # Find dependencies
# #######################################################################################################################
find_package(magic_enum CONFIG REQUIRED)
find_package(xbyak CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/utils.hpp" REQUIRED)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Settings.in.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Settings.h
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Settings.in.ini
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ini
    @ONLY
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        src/XSEPlugin.cpp
        src/InputEventHandler.cpp
        src/Settings.cpp
        src/DummyBSInputDevice.cpp
        src/DummyBSIInputDevice.cpp
        src/DummyBSWin32KeyboardDevice.cpp
        src/DummyBSKeyboardDevice.cpp
		src/UI.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CLIB_UTIL_INCLUDE_DIRS}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        UNICODE
        _UNICODE
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        magic_enum::magic_enum
        xbyak::xbyak
        nlohmann_json::nlohmann_json
)

# https://gitlab.kitware.com/cmake/cmake/-/issues/24922#note_1371990
if(MSVC_VERSION GREATER_EQUAL 1936)
    file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
<Project>
  <ItemDefinitionGroup>
    <ClCompile>
      <BuildStlModules>false</BuildStlModules>
    </ClCompile>
  </ItemDefinitionGroup>
</Project>
]==] @ONLY)
endif()

# #######################################################################################################################
# # Optional packaging
# #######################################################################################################################
if(ZIP_TO_DIST OR AIO_ZIP_TO_DIST)
    message(STATUS "Packaging is enabled")

    set(CPACK_GENERATOR "7Z;ZIP")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

    install(FILES "$<TARGET_FILE:${PROJECT_NAME}>" DESTINATION "SKSE/Plugins")
    install(FILES "$<TARGET_PDB_FILE:${PROJECT_NAME}>" DESTINATION "SKSE/Plugins" OPTIONAL)
    install(FILES "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.ini" DESTINATION "SKSE/Plugins")

    include(CPack)

    add_custom_target(package_mod
        COMMAND ${CMAKE_COMMAND} -E echo "Packaging for distribution..."
        COMMAND ${CMAKE_COMMAND} -E echo "CPack configuration: ${CPACK_GENERATOR}"
        COMMAND cpack -C $<CONFIG> --config ${CMAKE_BINARY_DIR}/CPackConfig.cmake
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Packaging for distribution..."
    )
endif()

# #######################################################################################################################
# # Automatic deployment
# #######################################################################################################################
if(AUTO_PLUGIN_DEPLOYMENT)
    if(DEFINED ENV{${PROJECT_NAME}OutputDir})
        foreach(DEPLOY_TARGET $ENV{${PROJECT_NAME}OutputDir})
            message(STATUS "Copying output to ${DEPLOY_TARGET}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET}/SKSE/Plugins"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DEPLOY_TARGET}/SKSE/Plugins/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DEPLOY_TARGET}/SKSE/Plugins/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.ini" "${DEPLOY_TARGET}/SKSE/Plugins/"
            )
        endforeach()
    else()
        message(STATUS "AUTO_PLUGIN_DEPLOYMENT is ON but environment variable '${PROJECT_NAME}OutputDir' is not set")
    endif()
endif()