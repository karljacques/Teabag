###############################################################################
# CMake Build System for Teabag
###############################################################################
# Variables
#   DEFINE_CMD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#                This is used to define compiler-wide #defines. MSVC uses /D
#                and every other compiler uses -D. This variable contains the
#                compiler specific switch to use. Defining a variable thus
#                looks like: add_definitions (${DEFINE_CMD}MY_DEFINITION)
#   DEPENDENCIES_DIR - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#                This is the directory where cmake will download and install
#                external dependencies to.
###############################################################################

###############################################################################
# CMake prerequisits
###############################################################################

# required for ExternalProject module
cmake_minimum_required (VERSION 2.8.11)

# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their
# definition and dump it in the cache along with proper documentation, otherwise
# set MAKE_BUILD_TYPE to Debug prior to calling PROJECT()
if (DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else()
    set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif()

# CMake module paths for external .cmake scripts
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/packages")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/utils")

###############################################################################
# Project name
###############################################################################

project (teabag)

###############################################################################
# set output locations
#  -> Windows should place DLLs in the same folder as the executable
#  -> Other systems should place shared libs in the lib folder
###############################################################################

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
if (WIN32)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else ()
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif ()

# Visual Studio nonsense (copy the above configuration to all configurations)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

###############################################################################
# Configure compiler definitions and flags
###############################################################################

if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    # Creating global #defines in gcc is done with -D
    set (DEFINE_CMD -D)
    # using C++11
    add_definitions (-std=c++11)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    # Creating global #defines in clang is done with -D
    set (DEFINE_CMD -D)
    # using C++11
    add_definitions (-std=c++11)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Intel")
    # TODO?
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
    # Creating global #defines in MSVC is done with /D
    set (DEFINE_CMD /D)
    # Remove "secure function" warnings, since this is a cross platform project
    add_definitions (/D _CRT_SECURE_NO_WARNINGS)
    # Link against static runtime library by adding /MTd and /MT.
    # This means we have to remove /MDd and /MD from the compiler command (it's
    # there by default) so the compiler doesn't warn about conflicting command
    # line arguments.
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        STRING(REGEX REPLACE "/MDd" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
        add_definitions (/MTd)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        STRING(REGEX REPLACE "/MD" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
        add_definitions (/MT)
    endif ()
endif ()

# Add DEBUG and _DEBUG defines, required by some libs (Ogre for instance)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(
        ${DEFINE_CMD}_DEBUG
        ${DEFINE_CMD}DEBUG
    )
endif ()

# Define the directory where cmake should download and install external
# dependencies to
set (DEPENDENCIES_DIR "${CMAKE_BINARY_DIR}/cmake-dep")

###############################################################################
# reconstruct folders ignored by git
###############################################################################

file (MAKE_DIRECTORY "${DEPENDENCIES_DIR}")
file (WRITE "${DEPENDENCIES_DIR}/.gitignore" "*\n!.gitignore")

###############################################################################
# Add subdirectories
###############################################################################

add_subdirectory ("src")
