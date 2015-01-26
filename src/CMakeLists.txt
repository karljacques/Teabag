################################################################################
# Executable
################################################################################

# Glob header and source files - save having to bother listing them here
file (GLOB_RECURSE teabag_HEADERS "*.h" "*.hpp" "*.hxx")
file (GLOB_RECURSE teabag_SOURCES "*.c" "*.cpp")

# Apple has some .mm files which need to be included
if (APPLE)
    file (GLOB_RECURSE teabag_SOURCES_OSX "*.mm")
    set (teabag_SOURCES ${teabag_SOURCES} ${teabag_SOURCES_OSX})
endif()

# Add the executable
include_directories (".")
include_directories ("engine")
link_directories (${DEPENDENCIES_DIR}/lib)
add_executable (teabag WIN32 MACOSX_BUNDLE ${teabag_HEADERS} ${teabag_SOURCES})

################################################################################
# Configure some flags for dependencies
################################################################################

# Common CMake Flags
set(COMMON_CMAKE_FLAGS
    -DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_DIR}
    -DCMAKE_PREFIX_PATH=${DEPENDENCIES_DIR}
    -DCMAKE_INCLUDE_PATH=${DEPENDENCIES_DIR}/include
    -DCMAKE_LIBRARY_PATH=${DEPENDENCIES_DIR}/lib)

set (CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${DEPENDENCIES_DIR}/include)
set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${DEPENDENCIES_DIR}/lib)

# Be sure to use the same c++ standard library as the main project. Additionally, disable the
# -Wdeprecated-register warning. CMAKE_CXX_FLAGS needs to be fully quoted so CMake handles the
# block as one thing in the COMMON_CMAKE_FLAGS list. Pretty horrid really.
if (APPLE)
    set(COMMON_CMAKE_FLAGS
        ${COMMON_CMAKE_FLAGS}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_OSX_ARCHITECTURES=x86_64
        "-DCMAKE_CXX_FLAGS='-std=c++11 -stdlib=libc++ -Wno-deprecated-register'")
endif()

include (link_sdl2)
link_sdl2 (teabag ${DEPENDENCIES_DIR} "")
include (link_ogredeps)
link_ogredeps (teabag ${DEPENDENCIES_DIR} "")
include (link_ogre)
link_ogre (teabag ${DEPENDENCIES_DIR} OGREDEPS_PROJECT)
include (link_bullet)
link_bullet (teabag ${DEPENDENCIES_DIR} "")

################################################################################
# MathGeoLib
################################################################################
message (STATUS "MathGeoLib will be downloaded automatically")
externalproject_add (MathGeoLib_PROJECT
    PREFIX ${DEPENDENCIES_DIR}
    GIT_REPOSITORY "git://github.com/juj/MathGeoLib"
    GIT_TAG "master"
    CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
)
add_dependencies(teabag MathGeoLib_PROJECT)
add_definitions (
    -DMATH_BULLET_INTEROP
    -DMATH_OGRE_INTEROP
)

include_directories ("${DEPENDENCIES_DIR}/include/MathGeoLib")
target_link_libraries (teabag MathGeoLib)

################################################################################
# RakNet
################################################################################
find_package (RakNet)
if (RakNet_FOUND)
    add_custom_target (RakNet_PROJECT)
else ()
    # Build RakNet
    message (STATUS "RakNet will be downloaded automatically")
    externalproject_add (RakNet_PROJECT
        PREFIX ${DEPENDENCIES_DIR}
        GIT_REPOSITORY "git://github.com/TheComet93/RakNet"
        GIT_TAG "master"
        CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
        -DRAKNET_ENABLE_STATIC=FALSE
    )
    add_dependencies(teabag RakNet_PROJECT)

    # Set up RakNet variables
    set (RAKNET_INCLUDES "${DEPENDENCIES_DIR}/include/RakNet")
    set (RAKNET_LIBRARY RakNetDLL)
endif ()

include_directories ("${RAKNET_INCLUDES}")
target_link_libraries (teabag ${RAKNET_LIBRARY})

################################################################################
# Post Build
################################################################################

# TODO - David: Ogre plugins are copied using the old method
# See TODO in file cmake/packages/link_ogre.cmake

add_custom_target(CopyMedia ALL)
add_dependencies(CopyMedia teabag)

if (WIN32)
    # TODO - Windows
elseif (APPLE)

    set(BUNDLE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/teabag.app)
    set(FRAMEWORKS_DIR ${BUNDLE_DIR}/Contents/Frameworks)
    set(RESOURCES_DIR ${BUNDLE_DIR}/Contents/Resources)

    # Game Resources
  add_custom_command(TARGET CopyMedia POST_BUILD COMMAND
    mkdir ARGS -p "${RESOURCES_DIR}/media")
  add_custom_command(TARGET CopyMedia POST_BUILD COMMAND
    rsync ARGS -r --delete
    "${CMAKE_SOURCE_DIR}/media/"
    "${RESOURCES_DIR}/media/")

    # Copy dependencies into the app bundle
    add_custom_command(TARGET CopyMedia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR})
    add_custom_command(TARGET CopyMedia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR}/Ogre.framework ${FRAMEWORKS_DIR}/Ogre.framework)
    add_custom_command(TARGET CopyMedia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR}/RenderSystem_GL.framework ${FRAMEWORKS_DIR}/RenderSystem_GL.framework)
    add_custom_command(TARGET CopyMedia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${DEPENDENCIES_DIR}/lib/libRakNetDLL.dylib ${FRAMEWORKS_DIR})

    # We need to write the correct path of each dependency relative to the
    # executable in the app bundle. Use install_name_tool to modify the search
    # paths in the executable
    add_custom_command(TARGET CopyMedia POST_BUILD
        COMMAND install_name_tool -change libRakNetDLL.dylib @executable_path/../Frameworks/libRakNetDLL.dylib ${BUNDLE_DIR}/Contents/MacOS/teabag)
elseif (UNIX)
    # Game Resources
  add_custom_command(TARGET CopyMedia POST_BUILD COMMAND
    mkdir ARGS -p "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/media")
  add_custom_command(TARGET CopyMedia POST_BUILD COMMAND
    rsync ARGS -r --delete
    "${CMAKE_SOURCE_DIR}/media/"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/media/")

    # Copy dependencies to output directory
    # TODO
endif ()
