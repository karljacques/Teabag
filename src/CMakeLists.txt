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
link_directories ("${CMAKE_SOURCE_DIR}/cmake-dep/lib")
add_executable (teabag ${teabag_HEADERS} ${teabag_SOURCES})

###############################################################################
# dependencies
###############################################################################

###############################################################################
# SDL2
find_package (SDL2 REQUIRED)
include_directories (${SDL2_INCLUDE_DIR})
target_link_libraries (teabag ${SDL2_LIBRARY})

###############################################################################
# RakNet
find_package (RakNet)
if (RakNet_FOUND)
	include_directories (${RAKNET_INCLUDES})
	target_link_libraries (teabag ${RAKNET_LIBRARY})
else (RakNet_FOUND)
	externalproject_add (RakNet_PROJECT
		PREFIX "${CMAKE_SOURCE_DIR}/cmake-dep"
		GIT_REPOSITORY "git://github.com/TheComet93/RakNet"
		GIT_TAG "master"
		CMAKE_ARGS -DRAKNET_ENABLE_STATIC=FALSE
		-DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/cmake-dep
	)
	include_directories ("${CMAKE_SOURCE_DIR}/cmake-dep/include/RakNet")
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
	include_directories (${BULLET_INCLUDE_DIR})
	target_link_libraries (teabag
		${BULLET_BulletDynamics_LIBRARY}
		${BULLET_BulletCollision_LIBRARY}
		${BULLET_LinearMath_LIBRARY}
	)
else (BULLET_FOUND)
	externalproject_add (Bullet_PROJECT
		PREFIX ${CMAKE_SOURCE_DIR}/cmake-dep
		GIT_REPOSITORY "git://github.com/bulletphysics/bullet3"
		GIT_TAG "Bullet-2.83-alpha"
		CMAKE_ARGS -DBUILD_EXTRAS:BOOL=OFF -DBUILD_UNIT_TESTS=OFF
		-DBUILD_CPU_DEMOS=OFF -DBUILD_BULLET2_DEMOS=OFF
		-DBUILD_BULLET3=OFF -DBUILD_BULLET3_DEMOS=OFF
		-DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/cmake-dep
	)
	include_directories ("${CMAKE_SOURCE_DIR}/cmake-dep/include/bullet")
	target_link_libraries (teabag
		BulletDynamics
		BulletCollision
		LinearMath
	)
endif (BULLET_FOUND)

###############################################################################
# MathGeoLib
externalproject_add (MathGeoLib_PROJECT
	PREFIX "${CMAKE_SOURCE_DIR}/cmake-dep"
	GIT_REPOSITORY "git://github.com/juj/MathGeoLib"
	GIT_TAG "master"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/cmake-dep
)
add_definitions (
	-DMATH_BULLET_INTEROP
	-DMATH_OGRE_INTEROP
)
include_directories ("${CMAKE_SOURCE_DIR}/cmake-dep/include/MathGeoLib")
target_link_libraries (teabag MathGeoLib)

###############################################################################
# Ogre3D
find_package (OGRE REQUIRED)
include_directories (${OGRE_INCLUDE_DIRS})
target_link_libraries (teabag ${OGRE_LIBRARIES})

# copy required plugins to local plugins folder
file (GLOB OGRE_PLUGINS "${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL*")
file (COPY ${OGRE_PLUGINS} DESTINATION "${CMAKE_BINARY_DIR}")
