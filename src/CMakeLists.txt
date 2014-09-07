###############################################################################
# create executable
###############################################################################

project (teabag)
include (ExternalProject)

# glob header and source files
file (GLOB_RECURSE teabag_HEADERS "*.h" "*.hpp" "*.hxx")
file (GLOB_RECURSE teabag_SOURCES "*.c" "*.cpp")

include_directories (
	"."
	"engine"
)

# create executable
add_executable (teabag ${teabag_HEADERS} ${teabag_SOURCES})

###############################################################################
# dependencies
###############################################################################

###############################################################################
# install ogre's dependencies
# zlib, zziplib, freetype, freeimage
find_package (ZLIB)
find_package (ZZip)
find_package (Freetype)
find_package (FreeImage)
if ((NOT ZLIB_FOUND) OR
	(NOT ZZip_FOUND) OR
	(NOT Freetype_FOUND) OR
	(NOT FreeImage_FOUND)
)
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
find_package (SDL2)
if (SDL2_FOUND)
	add_custom_target (SDL2_PROJECT)
	include_directories (${SDL2_INCLUDE_DIR})
	target_link_libraries (teabag ${SDL2_LIBRARY})
else (SDL2_FOUND)

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
endif (SDL2_FOUND)

###############################################################################
# boost
find_package (Boost COMPONENTS system)
if (BOOST_FOUND)
	add_custom_target (BOOST_PROJECT)
	include_directories (${BOOST_INCLUDE_DIRS})
	target_link_libraries (teabag
		boost_system
	)
else (BOOST_FOUND)

	# windows specific settings
	if (WIN32)
		set (BOOST_CONFIGURE_COMMAND "bootstrap.bat")
		set (BOOST_BUILD_COMMAND "b2" "install" "--prefix=${EXTERNAL_DEP_DIR}")
	else ()
		set (BOOST_CONFIGURE_COMMAND "./bootstrap.sh")
		set (BOOST_BUILD_COMMAND "./b2" "install"
			"--ignore-site-config" "--prefix=${EXTERNAL_DEP_DIR}" )
	endif ()

	externalproject_add (BOOST_PROJECT
		DEPENDS OGREDEPS_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		URL "http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.bz2"
		CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND}
		BUILD_COMMAND ${BOOST_BUILD_COMMAND}
		BUILD_IN_SOURCE 1
		INSTALL_COMMAND ""
	)
endif (BOOST_FOUND)

###############################################################################
# RakNet
find_package (RakNet)
if (RakNet_FOUND)
	add_custom_target (RakNet_PROJECT)
	include_directories (${RAKNET_INCLUDES})
	target_link_libraries (teabag ${RAKNET_LIBRARY})
else (RakNet_FOUND)
	message (STATUS "RakNet will be downloaded automatically")
	externalproject_add (RakNet_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		GIT_REPOSITORY "git://github.com/TheComet93/RakNet"
		GIT_TAG "master"
		CMAKE_ARGS -DRAKNET_ENABLE_STATIC=FALSE
		-DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
	)
	include_directories ("${EXTERNAL_DEP_DIR}/include/RakNet")
	target_link_libraries (teabag RakNetDLL)
endif (RakNet_FOUND)

###############################################################################
# Bullet
find_package (BULLET COMPONENTS
	BulletDynamics
	BulletCollision
	LinearMath
)
if (BULLET_FOUND)
	add_custom_target (Bullet_PROJECT)
	include_directories (${BULLET_INCLUDE_DIR})
	target_link_libraries (teabag
		${BULLET_BulletDynamics_LIBRARY}
		${BULLET_BulletCollision_LIBRARY}
		${BULLET_LinearMath_LIBRARY}
	)
else (BULLET_FOUND)
	message (STATUS "Bullet will be downloaded automatically")
	externalproject_add (Bullet_PROJECT
		DEPENDS OGREDEPS_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		GIT_REPOSITORY "git://github.com/bulletphysics/bullet3"
		GIT_TAG "Bullet-2.83-alpha"
		CMAKE_ARGS -DBUILD_EXTRAS:BOOL=OFF -DBUILD_UNIT_TESTS=OFF
		-DBUILD_CPU_DEMOS=OFF -DBUILD_BULLET2_DEMOS=OFF
		-DBUILD_BULLET3=OFF -DBUILD_BULLET3_DEMOS=OFF
		-DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
	)
	include_directories ("${EXTERNAL_DEP_DIR}/include/bullet")
	target_link_libraries (teabag
		BulletDynamics
		BulletCollision
		LinearMath
	)
endif (BULLET_FOUND)

###############################################################################
# MathGeoLib
message (STATUS "MathGeoLib will be downloaded automatically")
externalproject_add (MathGeoLib_PROJECT
	PREFIX ${EXTERNAL_DEP_DIR}
	GIT_REPOSITORY "git://github.com/juj/MathGeoLib"
	GIT_TAG "master"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
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
	include_directories (${OGRE_INCLUDE_DIRS})
	target_link_libraries (teabag ${OGRE_LIBRARIES})

	# copy required plugins to local plugins folder
	file (GLOB OGRE_PLUGINS "${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL*")
	file (COPY ${OGRE_PLUGINS} DESTINATION "${CMAKE_BINARY_DIR}")
else (OGRE_FOUND)
	message (STATUS "Ogre will be downloaded automatically")
	externalproject_add(Ogre_PROJECT
		DEPENDS OGREDEPS_PROJECT SDL2_PROJECT BOOST_PROJECT
		PREFIX ${EXTERNAL_DEP_DIR}
		HG_REPOSITORY "https://bitbucket.org/sinbad/ogre"
		HG_TAG "v1-9-0"
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DEP_DIR}
		-DOGRE_BUILD_SAMPLES=0
		-DCMAKE_LIBRARY_PATH=${EXTERNAL_DEP_DIR}/lib
		-DCMAKE_INCLUDE_PATH=${EXTERNAL_DEP_DIR}/include
	)
	include_directories ("${EXTERNAL_DEP_DIR}/include/OGRE")
	target_link_libraries (teabag OgreMain)

	# copy required plugins to local plugins folder
	file (GLOB OGRE_PLUGINS "${EXTERNAL_DEP_DIR}/lib/OGRE/RenderSystem_GL*")
	file (COPY ${OGRE_PLUGINS} DESTINATION "${CMAKE_BINARY_DIR}")

endif (OGRE_FOUND)
