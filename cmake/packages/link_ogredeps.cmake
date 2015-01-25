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

###############################################################################
# install ogre's dependencies if required
# zlib, zziplib, freetype, freeimage
macro (link_ogredeps TARGET_NAME INSTALL_PREFIX DEPS)
    find_package (ZLIB)
    find_package (ZZip)
    find_package (Freetype)
    find_package (FreeImage)
    if ((NOT ZLIB_FOUND) OR (NOT ZZip_FOUND) OR (NOT FREETYPE_FOUND) OR (NOT FreeImage_FOUND))
        message (STATUS "OGREDEPS will be downloaded automatically")

        # needs to be patched so SDK version on mac is ignored
        if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set (PATCH_COMMAND patch ${INSTALL_PREFIX}/src/OGREDEPS_PROJECT/CMakeLists.txt < ${CMAKE_SOURCE_DIR}/cmake/Patches/OgreDepsCMakeLists.patch)
        endif ()
        externalproject_add (OGREDEPS_PROJECT
            DEPENDS ${DEPS}
            PREFIX ${INSTALL_PREFIX}
            HG_REPOSITORY "https://bitbucket.org/cabalistic/ogredeps/"
            HG_TAG "default"
            PATCH_COMMAND ${PATCH_COMMAND}
            CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
        )
    else ()
        add_custom_target (OGREDEPS_PROJECT)
    endif ()
    add_dependencies (${TARGET_NAME} OGREDEPS_PROJECT)
endmacro (link_ogredeps)
