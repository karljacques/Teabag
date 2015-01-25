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

macro (link_ogre TARGET_NAME INSTALL_PREFIX DEPS)
    find_package (OGRE)
    if (OGRE_FOUND)
        add_custom_target (Ogre_PROJECT)
    else (OGRE_FOUND)
        message (STATUS "Ogre will be downloaded automatically")
        externalproject_add (Ogre_PROJECT
            DEPENDS ${DEPS}
            PREFIX ${INSTALL_PREFIX}
            HG_REPOSITORY "https://bitbucket.org/sinbad/ogre"
            HG_TAG "v1-9-0"
            CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
            -DOGRE_BUILD_COMPONENT_OVERLAY=0
            -DOGRE_BUILD_COMPONENT_PAGING=0
            -DOGRE_BUILD_COMPONENT_PROPERTY=0
            -DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM=0
            -DOGRE_BUILD_COMPONENT_TERRAIN=0
            -DOGRE_BUILD_COMPONENT_VOLUME=0
            -DOGRE_BUILD_DOCS=0
            -DOGRE_BUILD_PLUGIN_BSP=0
            -DOGRE_BUILD_PLUGIN_OCTREE=0
            -DOGRE_BUILD_PLUGIN_CG=0
            -DOGRE_BUILD_PLUGIN_PCZ=0
            -DOGRE_BUILD_PLUGIN_PFX=0
            -DOGRE_BUILD_RENDERSYSTEM_GL=1
            -DOGRE_BUILD_SAMPLES=0
            -DOGRE_BUILD_TOOLS=0
            -DOGRE_INSTALL_DOCS=0
            -DOGRE_INSTALL_PDB=0
            -DOGRE_INSTALL_SAMPLES=0
            -DOGRE_CONFIG_THREADS=0
            -DOGRE_UNITY_BUILD=1
            -DOGRE_USE_BOOST=0
        )
        set (OGRE_INCLUDE_DIRS "${INSTALL_PREFIX}/include/OGRE")
        if (APPLE)
            set (OGRE_LIBRARIES "${INSTALL_PREFIX}/lib/RelWithDebInfo/Ogre.framework")
            set (OGRE_PLUGIN_DIR_REL "${INSTALL_PREFIX}/lib/RelWithDebInfo")
        else (APPLE)
            set (OGRE_LIBRARIES "OgreMain")
            set (OGRE_PLUGIN_DIR_REL "${INSTALL_PREFIX}/lib/OGRE")
        endif (APPLE)
    endif (OGRE_FOUND)

    include_directories (${OGRE_INCLUDE_DIRS})
    target_link_libraries (${TARGET_NAME} ${OGRE_LIBRARIES})
    
    add_dependencies (${TARGET_NAME} Ogre_PROJECT)

    # copy required plugins to local plugins folder
    # TODO - don't do this, instead use the CopyMedia target
    if (CMAKE_BUILD_TYPE MATCHES Release)
        set (OGRE_PLUGIN_DIR ${OGRE_PLUGIN_DIR_REL})
    else ()
        set (OGRE_PLUGIN_DIR ${OGRE_PLUGIN_DIR_DBG})
    endif ()
    message (STATUS "Ogre plugin dir: ${OGRE_PLUGIN_DIR}")
    file (GLOB OGRE_PLUGINS
        "${OGRE_PLUGIN_DIR}/RenderSystem_GL*"
        "${OGRE_PLUGIN_DIR}/Plugin_OctreeSceneManager*"
    )
    message (STATUS "Copying plugins: ${OGRE_PLUGINS}")
    file (COPY ${OGRE_PLUGINS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endmacro (link_ogre)