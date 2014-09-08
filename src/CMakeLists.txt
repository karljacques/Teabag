###############################################################################
# create executable
###############################################################################

project (teabag)
include (ExternalProject)

# glob header and source files
file (GLOB_RECURSE teabag_HEADERS "*.h" "*.hpp" "*.hxx")
file (GLOB_RECURSE teabag_SOURCES "*.c" "*.cpp")

if (APPLE)
	file (GLOB_RECURSE teabag_SOURCES_OSX "*.mm")
	set (teabag_SOURCES ${teabag_SOURCES} ${teabag_SOURCES_OSX})
endif()

include_directories (
	"."
	"engine"
)

# create executable
add_executable (teabag WIN32 MACOSX_BUNDLE ${teabag_HEADERS} ${teabag_SOURCES})

###############################################################################
# dependencies
################################################################################ Common CMake Flags

# cmake flags common to all dependencies
set(COMMON_CMAKE_FLAGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
)

# the default stdlibc++ when using C++11 in the main project. Additionally,
# disable the -Wdeprecated-register warning.
if (APPLE)
	set(COMMON_CMAKE_FLAGS
		${COMMON_CMAKE_FLAGS}
		-DCMAKE_OSX_ARCHITECTURES="x86_64"
		-DCMAKE_C_FLAGS="-stdlib=libc++"
		"-DCMAKE_CXX_FLAGS='-std=c++11 -stdlib=libc++ -Wno-deprecated-register'"
	)
endif()

###############################################################################
# install ogre's dependencies if required
# zlib, zziplib, freetype, freeimage
find_package (ZLIB)
find_package (ZZip)
find_package (Freetype)
find_package (FreeImage)
if ((NOT ZLIB_FOUND) OR
	(NOT ZZip_FOUND) OR
	(NOT FREETYPE_FOUND) OR
	(NOT FreeImage_FOUND)
)
	message (STATUS "OGREDEPS will be downloaded automatically")
	externalproject_add(OGREDEPS_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		HG_REPOSITORY "https://bitbucket.org/cabalistic/ogredeps/"
		HG_TAG "default"
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
	)
else ()
	add_custom_target (OGREDEPS_PROJECT)
endif ()

###############################################################################
# SDL2
# TODO this sucks ass and only runs on windows right now
find_package (SDL2)
if (SDL2_FOUND)
	add_custom_target (SDL2_PROJECT)
else (SDL2_FOUND)

	message (STATUS "SDL2 will be downloaded automatically")

	# windows specific settings
	if (WIN32)
		set (SDL_CONFIGURE_COMMAND "")
		set (SDL_INSTALL_COMMAND "")
	endif ()

	# MSVC compiler is going to use msbuild to build the solution files
	if (MSVC80)
		message (STATUS "Using MSBuild vs2008") 
		set (SDL_BUILD_COMMAND "MSBuild" "SDL_VS2008.sln")
	elseif (MSVC10)
		message (STATUS "Using MSBuild vs2010")
		set (SDL_BUILD_COMMAND "MSBuild" "SDL_VS2010.sln")
	else ()
	endif ()

	# debug or release
	#if (MSVC)
	#	if (CMAKE_BUILD_TYPE MATCHES "Release")
	#		set (SDL_BUILD_COMMAND ${SDL_CONFIGURE_COMMAND}
	#			" /property:Configuration=Release")
	#	else ()
	#		set (SDL_BUILD_COMMAND ${SDL_CONFIGURE_COMMAND}
	#			" /property:Configuration=Debug")
	#	endif ()
	#endif ()

	externalproject_add (SDL2_PROJECT
		DEPENDS OGREDEPS_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		URL "https://www.libsdl.org/release/SDL2-2.0.3.tar.gz"
		CONFIGURE_COMMAND ${SDL_CONFIGURE_COMMAND}
		BUILD_COMMAND ${SDL_BUILD_COMMAND}
		BINARY_DIR ${EXTERNAL_DEP_DIR}/src/SDL2_PROJECT/VisualC
		INSTALL_COMMAND ""
	)

	set (SDL2_INCLUDE_DIR "${EXTERNAL_DEP_DIR}/include/SDL2")
	set (SDL2_LIBRARY sdl2)
endif (SDL2_FOUND)

include_directories (${SDL2_INCLUDE_DIR})
target_link_libraries (teabag ${SDL2_LIBRARY})

###############################################################################
# boost - being future-proof since tomorrow!
# TheComet: For some stupid reason teabag really wants to link with
# boost_system. This is happening on Gentoo with boost-1.52.0-r7 installed
# system-wide and ogre picking up on it.
#find_package (Boost COMPONENTS system)
#if (Boost_FOUND)
#	add_custom_target (BOOST_PROJECT)
#	include_directories (${BOOST_INCLUDE_DIRS})
#	target_link_libraries (teabag
#		${Boost_SYSTEM_LIBRARY}
#	)
#endif ()
#else (Boost_FOUND)
#	message (STATUS "Boost will be downloaded automatically")
#
#	# windows specific settings
#	if (WIN32)
#		set (BOOST_CONFIGURE_COMMAND "bootstrap.bat")
#		set (BOOST_BUILD_COMMAND "b2" "install" "--prefix=${EXTERNAL_DEP_DIR}")
#	else ()
#		set (BOOST_CONFIGURE_COMMAND "./bootstrap.sh")
#		set (BOOST_BUILD_COMMAND "./b2" "install"
#			"--ignore-site-config" "--prefix=${EXTERNAL_DEP_DIR}" )
#	endif ()
#
#	externalproject_add (BOOST_PROJECT
#		DEPENDS OGREDEPS_PROJECT
#		PREFIX ${EXTERNAL_DEP_DIR}
#		URL "http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.bz2"
#		CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND}
#		BUILD_COMMAND ${BOOST_BUILD_COMMAND}
#		BUILD_IN_SOURCE 1
#		INSTALL_COMMAND ""
#	)
#	target_link_libraries (teabag
#		boost_system
#	)
#endif (Boost_FOUND)

###############################################################################
# RakNet
find_package (RakNet)
if (RakNet_FOUND)
	add_custom_target (RakNet_PROJECT)
else (RakNet_FOUND)
	message (STATUS "RakNet will be downloaded automatically")
	externalproject_add (RakNet_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		GIT_REPOSITORY "git://github.com/TheComet93/RakNet"
		GIT_TAG "master"
		CMAKE_ARGS -DRAKNET_ENABLE_STATIC=FALSE
		-DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
	)
	set (RAKNET_INCLUDES "${EXTERNAL_DEP_DIR}/include/RakNet")
	set (RAKNET_LIBRARY RakNetDLL)
endif (RakNet_FOUND)
		
include_directories (${RAKNET_INCLUDES})
target_link_libraries (teabag ${RAKNET_LIBRARY})

###############################################################################
# Bullet
find_package (BULLET COMPONENTS
	BulletDynamics
	BulletCollision
	LinearMath
)
if (BULLET_FOUND)
	add_custom_target (Bullet_PROJECT)
else ()
	message (STATUS "Bullet will be downloaded automatically")
	externalproject_add (Bullet_PROJECT
		DEPENDS OGREDEPS_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		GIT_REPOSITORY "git://github.com/bulletphysics/bullet3"
		GIT_TAG "Bullet-2.83-alpha"
		CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
		-DBUILD_EXTRAS=0
		-DBUILD_UNIT_TESTS=0
		-DBUILD_CPU_DEMOS=0
		-DBUILD_BULLET2_DEMOS=0
		-DBUILD_BULLET3=0
		-DBUILD_BULLET3_DEMOS=0
	)
	set (BULLET_INCLUDE_DIR "${EXTERNAL_DEP_DIR}/include/bullet")
	set (BULLET_BulletDynamics_LIBRARY BulletDynamics)
	set (BULLET_BulletCollision_LIBRARY BulletCollision)
	set (BULLET_LinearMath_LIBRARY LinearMath)
endif ()

include_directories ("${BULLET_INCLUDE_DIR}")
target_link_libraries (teabag
	${BULLET_BulletDynamics_LIBRARY}
	${BULLET_BulletCollision_LIBRARY}
	${BULLET_LinearMath_LIBRARY}
)

###############################################################################
# MathGeoLib
message (STATUS "MathGeoLib will be downloaded automatically")
externalproject_add (MathGeoLib_PROJECT
	PREFIX ${EXTERNAL_DEP_DIR}
	GIT_REPOSITORY "git://github.com/juj/MathGeoLib"
	GIT_TAG "master"
	CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
)
add_definitions (
	-DMATH_BULLET_INTEROP
	-DMATH_OGRE_INTEROP
)
include_directories ("${EXTERNAL_DEP_DIR}/include/MathGeoLib")
target_link_libraries (teabag MathGeoLib)

###############################################################################
# Ogre3D
find_package (OGRE)
if (OGRE_FOUND)
	add_custom_target (Ogre_PROJECT)
else (OGRE_FOUND)
	message (STATUS "Ogre will be downloaded automatically")
	externalproject_add(Ogre_PROJECT
		DEPENDS OGREDEPS_PROJECT SDL2_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		HG_REPOSITORY "https://bitbucket.org/sinbad/ogre"
		HG_TAG "v1-9-0"
		DEPENDS OGREDEPS_PROJECT
		CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
		-DOGRE_BUILD_COMPONENT_OVERLAY=0
		-DOGRE_BUILD_COMPONENT_PAGING=0
		-DOGRE_BUILD_COMPONENT_PROPERTY=0
		-DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM=0
		-DOGRE_BUILD_COMPONENT_TERRAIN=0
		-DOGRE_BUILD_COMPONENT_VOLUME=0
		-DOGRE_BUILD_PLUGIN_BSP=0
		-DOGRE_BUILD_PLUGIN_CG=0
		-DOGRE_BUILD_PLUGIN_PCZ=0
		-DOGRE_BUILD_PLUGIN_PFX=0
		-DOGRE_BUILD_RENDERSYSTEM_GL=1
		-DOGRE_BUILD_SAMPLES=0
		-DOGRE_BUILD_TOOLS=0
		-DOGRE_INSTALL_DOCS=0
		-DOGRE_INSTALL_PDB=0
		-DOGRE_INSTALL_SAMPLES=0
	)

	# Set up Ogre variables
	set (OGRE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/cmake-dep/include/OGRE;${CMAKE_SOURCE_DIR}/cmake-dep/include/OIS")
	# TODO set OGRE_PLUGIN_DIR_REL and OGRE_PLUGIN_DIR_DBG -- need to do
	# this on Windows because only there are they placed in different
	# folders.
	if (APPLE)
		set(OGRE_LIBRARIES "${CMAKE_SOURCE_DIR}/cmake-dep/lib/RelWithDebInfo/Ogre.framework")
		set(OGRE_PLUGIN_DIR_REL "${CMAKE_SOURCE_DIR}/cmake-dep/lib/RelWithDebInfo")
	elseif (UNIX)
		set(OGRE_LIBRARIES "OgreMain")
		set(OGRE_PLUGIN_DIR_REL "${CMAKE_SOURCE_DIR}/cmake-dep/lib/OGRE")
	endif()

endif ()

# Link to Ogre
include_directories (${OGRE_INCLUDE_DIRS})
target_link_libraries (teabag ${OGRE_LIBRARIES})

# Copy required plugins to local plugins folder
#file (GLOB_RECURSE OGRE_PLUGINS "${OGRE_PLUGIN_DIR_REL}" FOLLOW_SYMLINKS
#	"RenderSystem_GL*"
#)
if (CMAKE_BUILD_TYPE=Release)
	set (OGRE_PLUGIN_DIR ${OGRE_PLUGIN_DIR_REL})
else ()
	set (OGRE_PLUGIN_DIR ${OGRE_PLUGIN_DIR_DBG})
endif ()

file (GLOB OGRE_PLUGINS
	"${OGRE_PLUGIN_DIR}/RenderSystem_GL*"
)
set (OGRE_LOCAL_PLUGIN_DIR ${CMAKE_BINARY_DIR})
message (STATUS "Copying Ogre plugins: ${OGRE_PLUGINS}")
file (COPY ${OGRE_PLUGINS} DESTINATION ${OGRE_LOCAL_PLUGIN_DIR})

###############################################################################
# keep multithreaded compilation happy
add_dependencies (teabag
	Ogre_PROJECT
	SDL2_PROJECT
	RackNet_PROJECT
	Bullet_PROJECT
	MathGeoLib_PROJECT
)

###############################################################################
# OS X specific post build
if (APPLE)
	set(BUNDLE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/teabag.app)
	set(FRAMEWORKS_DIR ${BUNDLE_DIR}/Contents/Frameworks)

	# Copy dependencies into the app bundle
	add_custom_command(TARGET teabag POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${FRAMEWORKS_DIR})
	add_custom_command(TARGET teabag POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR_REL}/Ogre.framework ${FRAMEWORKS_DIR}/Ogre.framework)
	add_custom_command(TARGET teabag POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.framework ${FRAMEWORKS_DIR}/RenderSystem_GL.framework)
	add_custom_command(TARGET teabag POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake-dep/lib/libRakNetDLL.dylib ${FRAMEWORKS_DIR})

	# We need to write the correct path of each dependency relative to the
	# executable in the app bundle. Use install_name_tool to modify the search
	# paths in the executable
	add_custom_command(TARGET teabag POST_BUILD
		COMMAND install_name_tool -change libRakNetDLL.dylib @executable_path/../Frameworks/libRakNetDLL.dylib ${BUNDLE_DIR}/Contents/MacOS/teabag)
endif ()
