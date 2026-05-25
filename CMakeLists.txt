cmake_minimum_required(VERSION 3.21)

project(
    DualWieldParryingNG
    VERSION 1.0.1
    LANGUAGES CXX
)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

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
# # CommonLibSSE NG / CommonLibVR
# #######################################################################################################################
set(COMMONLIBSSE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE-NG" CACHE PATH "Path to CommonLibVR ng checkout")

set(BUILD_TESTS OFF CACHE BOOL "Do not build CommonLib tests for this plugin" FORCE)
set(ENABLE_SKYRIM_SE ON CACHE BOOL "Enable Skyrim SE support")
set(ENABLE_SKYRIM_AE ON CACHE BOOL "Enable Skyrim AE support")
set(ENABLE_SKYRIM_VR OFF CACHE BOOL "Disable Skyrim VR support for this SE/AE plugin" FORCE)
set(SKSE_SUPPORT_XBYAK ON CACHE BOOL "Enable CommonLib trampoline support")

add_subdirectory("${COMMONLIBSSE_SOURCE_DIR}" CommonLibSSE EXCLUDE_FROM_ALL)
include("${COMMONLIBSSE_SOURCE_DIR}/cmake/CommonLibSSE.cmake")

# #######################################################################################################################
# # Find dependencies
# #######################################################################################################################
find_path(CLIB_UTIL_INCLUDE_DIRS "ClibUtil/utils.hpp" REQUIRED)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/Plugin.h
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/Version.rc
    @ONLY
)

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

set(PLUGIN_SOURCES
    src/XSEPlugin.cpp
    src/InputCode.cpp
    src/InputDeviceStubs.cpp
    src/InputEventHandler.cpp
    src/Settings.cpp
    src/UI.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/Plugin.h
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/Version.rc
)

add_commonlibsse_plugin(
    ${PROJECT_NAME}
    AUTHOR "Dual Wield Parrying NG Contributors"
    USE_ADDRESS_LIBRARY
    SOURCES ${PLUGIN_SOURCES}
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
        src/PCH.h
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CLIB_UTIL_INCLUDE_DIRS}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        UNICODE
        _UNICODE
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
            /MP
            /W4
            /permissive-
            /Zc:preprocessor
    )
endif()

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
