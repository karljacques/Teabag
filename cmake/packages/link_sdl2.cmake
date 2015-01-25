###############################################################################
# Input variables
#   CMAKE_COMMON_ARGS:list
#     - a list of cmake arguments to pass to the cmake command when configuring
#   INSTALL_PREFIX:string
#     - Where to download and install the dependency to
#   DEPS
#     - A list of targets that are dependencies of this target
###############################################################################

include (ExternalProject)

macro (link_sdl2 TARGET_NAME INSTALL_PREFIX DEPS)
    find_package (SDL2)
    if (SDL2_FOUND)
        add_custom_target (SDL2_PROJECT)
    else (SDL2_FOUND)
        message (STATUS "SDL2 will be downloaded automatically")

        # patch for handling proper shared library object extensions
        if (UNIX)
            set (PATCH_COMMAND patch ${INSTALL_PREFIX}/src/SDL2_PROJECT/CMakeLists.txt < ${CMAKE_SOURCE_DIR}/cmake/patches/SDL2CMakeLists.patch)
            message (STATUS "patching SDL2")
        endif (UNIX)

        externalproject_add (SDL2_PROJECT
            DEPENDS ${DEPS}
            PREFIX ${INSTALL_PREFIX}
            URL "https://www.libsdl.org/release/SDL2-2.0.3.tar.gz"
            PATCH_COMMAND ${PATCH_COMMAND}
            CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
        )

        set (SDL2_INCLUDE_DIR "${INSTALL_PREFIX}/include/SDL2")
        set (SDL2_LIBRARY SDL2)
    endif (SDL2_FOUND)

    include_directories (${SDL2_INCLUDE_DIR})
    target_link_libraries (${TARGET_NAME} ${SDL2_LIBRARY})
    
    add_dependencies (${TARGET_NAME} SDL2_PROJECT)
endmacro (link_sdl2)
