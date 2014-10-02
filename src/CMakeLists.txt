project (teabag)
include (ExternalProject)

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
include_directories (. engine)
link_directories (${DEPENDENCIES_DIR}/lib)
add_executable (teabag WIN32 MACOSX_BUNDLE ${teabag_HEADERS} ${teabag_SOURCES})


################################################################################
# Dependencies
################################################################################

# Common CMake Flags
set(COMMON_CMAKE_FLAGS
	-DCMAKE_INSTALL_PREFIX=${DEPENDENCIES_DIR}
	-DCMAKE_PREFIX_PATH=${DEPENDENCIES_DIR}
	-DCMAKE_INCLUDE_PATH=${DEPENDENCIES_DIR}/include
	-DCMAKE_LIBRARY_PATH=${DEPENDENCIES_DIR}/lib)

# Mac OS X requires dependencies to use the libc++ standard library instead of
# the default stdlibc++ when using C++11 in the main project. Additionally,
# disable the -Wdeprecated-register warning.
if (APPLE)
	set(COMMON_CMAKE_FLAGS
		${COMMON_CMAKE_FLAGS}
		-DCMAKE_OSX_ARCHITECTURES="x86_64"
		-DCMAKE_C_FLAGS="-stdlib=libc++"
		"-DCMAKE_CXX_FLAGS='-std=c++11 -stdlib=libc++ -Wno-deprecated-register'")
endif()


################################################################################
# SDL2
################################################################################

find_package (SDL2 REQUIRED)
include_directories (${SDL2_INCLUDE_DIR})
target_link_libraries (teabag ${SDL2_LIBRARY})


################################################################################
# Bullet
################################################################################
find_package (BULLET COMPONENTS
	BulletDynamics
	BulletCollision
	LinearMath
)
if (BULLET_FOUND)
	include_directories (${BULLET_INCLUDE_DIR})
	target_link_libraries (teabag
		${BULLET_BulletDynamics_LIBRARY}
		${BULLET_BulletCollision_LIBRARY}
		${BULLET_LinearMath_LIBRARY}
	)
else ()
	message (STATUS "Bullet will be downloaded automatically")
	externalproject_add (Bullet_PROJECT
		PREFIX ${DEPENDENCIES_DIR}
		GIT_REPOSITORY "git://github.com/bulletphysics/bullet3"
		GIT_TAG "Bullet-2.83-alpha"
		CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
		-DBUILD_EXTRAS=0
		-DBUILD_UNIT_TESTS=0
		-DBUILD_CPU_DEMOS=0
		-DBUILD_BULLET2_DEMOS=0
		-DBUILD_BULLET3=0
	)
	add_dependencies(teabag Bullet_PROJECT)
	include_directories ("${DEPENDENCIES_DIR}/include/bullet")
	target_link_libraries (teabag
		BulletDynamics
		BulletCollision
		LinearMath
	)
endif ()


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
# Ogre
################################################################################
find_package (OGRE)
if (NOT OGRE_FOUND)
	# Install Ogre's dependencies
	externalproject_add(OgreDeps_PROJECT
		PREFIX ${DEPENDENCIES_DIR}
		HG_REPOSITORY "https://bitbucket.org/davedissian/ogredeps/"
		HG_TAG "default"
		CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
	)

	# Build Ogre
	message (STATUS "Ogre will be downloaded automatically")
	externalproject_add(Ogre_PROJECT
		PREFIX ${DEPENDENCIES_DIR}
		HG_REPOSITORY "https://bitbucket.org/sinbad/ogre"
		HG_TAG "v1-9-0"
		DEPENDS OgreDeps_PROJECT
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
	add_dependencies(teabag Ogre_PROJECT)

	# Set up Ogre variables
	set(OGRE_INCLUDE_DIRS "${DEPENDENCIES_DIR}/include/OGRE;${DEPENDENCIES_DIR}/include/OIS")
	if (APPLE)
		set(OGRE_LIBRARIES "${DEPENDENCIES_DIR}/lib/RelWithDebInfo/Ogre.framework/Ogre")
		set(OGRE_PLUGIN_DIR_REL "${DEPENDENCIES_DIR}/lib/RelWithDebInfo")
	elseif (UNIX)
		set(OGRE_LIBRARIES "OgreMain")
		set(OGRE_PLUGIN_DIR_REL "${DEPENDENCIES_DIR}/lib/OGRE")
	endif()

endif ()

# Link to Ogre
include_directories (${OGRE_INCLUDE_DIRS})
target_link_libraries (teabag ${OGRE_LIBRARIES})

# Copy required plugins to local plugins folder
# TODO - don't do this, instead use the CopyMedia target
if (UNIX AND NOT APPLE)
	file (GLOB OGRE_PLUGINS "${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL*")
	file (COPY ${OGRE_PLUGINS} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif ()


################################################################################
# RakNet
################################################################################
find_package (RakNet)
if (NOT RakNet_FOUND)
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

include_directories (${RAKNET_INCLUDES})
target_link_libraries (teabag ${RAKNET_LIBRARY})


################################################################################
# Post Build
################################################################################

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
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR_REL}/Ogre.framework ${FRAMEWORKS_DIR}/Ogre.framework)
	add_custom_command(TARGET CopyMedia POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.framework ${FRAMEWORKS_DIR}/RenderSystem_GL.framework)
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
