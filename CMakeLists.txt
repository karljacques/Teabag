###############################################################################

cmake_minimum_required (VERSION 2.8.11)



# Default build type: Debug
# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their
# definition and dump it in the cache along with proper documentation, otherwise
# set MAKE_BUILD_TYPE to Debug prior to calling PROJECT()
if (DEFINED CMAKE_BUILD_TYPE)
	if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
		set (CMAKE_BUILD_TYPE Debug)
	endif ()
	set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else()
	set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif()

###############################################################################
# set output locations
# windows should place DLLs in the same folder as the executable
###############################################################################

set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if (WIN32)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else ()
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/packages")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/utils")

###############################################################################
# compiler definitions and flags
###############################################################################

if (MSVC)
elseif (MINGW)
elseif (APPLE)
	add_definitions(
		-std=c++11
		-Wno-deprecated-register
	)
elseif (UNIX)
	add_definitions(
		-std=c++11
	)
else ()
	message(FATAL_ERROR "Unsupported OS")
endif ()

# add debug definitions
# note that CMake defines NDEBUG automatically when building a release build
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(
		-D_DEBUG
	)
endif ()

# global definitions
add_definitions (
)

# Dependency dir
set (DEPENDENCIES_DIR "${CMAKE_BINARY_DIR}/cmake-dep")

###############################################################################
# reconstruct folders ignored by git
###############################################################################

file (MAKE_DIRECTORY "${DEPENDENCIES_DIR}")
file (WRITE "${DEPENDENCIES_DIR}/.gitignore" "*")
file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory (src)
