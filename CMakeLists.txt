##############################################################
# Copyright(c) 2013 Loctronix Corporation
# http://www.loctronix.com
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
##############################################################

##############################################################
# Verify VPATH build
##############################################################

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

##############################################################
# Project setup
##############################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PROJECT_NAME A2300)
PROJECT(${PROJECT_NAME} CXX C)
ENABLE_TESTING()

# select the release build type by default to get optimization flags

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
    MESSAGE(STATUS "Build type not specified: defaulting to release.")
ENDIF()
SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

# Set the version information here

SET(VERSION_MAJOR 0)
SET(VERSION_API_COMPAT 1)
SET(VERSION_MINOR 0)
SET(VERSION_MAINT git)

SET(${PROJECT_NAME}_VERSION_FULL
    ${VERSION_MAJOR}.${VERSION_API_COMPAT}.${VERSION_MINOR}
)
SET(${PROJECT_NAME}_VERSION_PATCH
    ${VERSION_MAJOR}.${VERSION_API_COMPAT}
)

##############################################################
# Define install directories
##############################################################

# when the library suffix should be 64 (applies to redhat linux family)
IF(NOT DEFINED LIB_SUFFIX AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    SET(LIB_SUFFIX 64)
ENDIF()
SET(LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(RUNTIME_DIR bin)
SET(LIBRARY_DIR lib${LIB_SUFFIX})
SET(INCLUDE_DIR include)
SET(PKG_DATA_DIR share/A2300)
SET(PKG_DOC_DIR share/doc/A2300)
SET(PKG_MAN_DIR share/man/man1)
SET(UHD_MODULE_DIR share/uhd/modules)

##############################################################
# Add definition for host OS
##############################################################

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    ADD_DEFINITIONS(-DLINUX)
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    ADD_DEFINITIONS(-DAPPLE)
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    ADD_DEFINITIONS(-DWIN32)
ENDIF()

##############################################################
# Compiler specific setup
##############################################################

INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)
MACRO(ADD_OPTIONAL_COMPILER_FLAG type flag)
    
    IF(${type} STREQUAL "ALL")

        CHECK_C_COMPILER_FLAG(${flag} HAVE_DUMMY)
        IF(HAVE_DUMMY)
            ADD_DEFINITIONS(${flag})
        ENDIF()
        UNSET(HAVE_DUMMY)

    ELSEIF(${type} STREQUAL "C")

        CHECK_C_COMPILER_FLAG(${flag} HAVE_DUMMY)
        IF(HAVE_DUMMY)
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
        ENDIF()
        UNSET(HAVE_DUMMY)

    ELSEIF(${type} STREQUAL "CXX")

        CHECK_CXX_COMPILER_FLAG(${flag} HAVE_DUMMY)
        IF(HAVE_DUMMY)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
        ENDIF()
        UNSET(HAVE_DUMMY)

    ENDIF()
ENDMACRO(ADD_OPTIONAL_COMPILER_FLAG)

IF(CMAKE_COMPILER_IS_GNUCXX)
    # We are not using this yet in A2300; see the url for more info.
    # http://gcc.gnu.org/wiki/Visibility
    # ADD_OPTIONAL_COMPILER_FLAG(CXX -fvisibility=hidden)
    # ADD_OPTIONAL_COMPILER_FLAG(CXX -fvisibility-inlines-hidden)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX AND
    NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_OPTIONAL_COMPILER_FLAG(CXX -s)
ENDIF()

ADD_OPTIONAL_COMPILER_FLAG(ALL -Wall)
ADD_OPTIONAL_COMPILER_FLAG(ALL -Wextra)
ADD_OPTIONAL_COMPILER_FLAG(ALL -Wsign-compare)
ADD_OPTIONAL_COMPILER_FLAG(ALL -Wconversion)
#ADD_OPTIONAL_COMPILER_FLAG(ALL -pedantic)
#ADD_OPTIONAL_COMPILER_FLAG(ALL -ansi)

ADD_OPTIONAL_COMPILER_FLAG(ALL -fmessage-length=0)
ADD_OPTIONAL_COMPILER_FLAG(ALL -fPIC)

##############################################################
# Get sizeof int and long, to make sure common/System/DataTypes.h
# gets them correct.
##############################################################

INCLUDE (CheckTypeSize)
CHECK_TYPE_SIZE("int" SIZEOF_INT)
CHECK_TYPE_SIZE("long" SIZEOF_LONG)

ADD_DEFINITIONS(
    -DSIZEOF_INT=${SIZEOF_INT}
    -DSIZEOF_LONG=${SIZEOF_LONG}
)

##############################################################
# Setup common include paths within this build
##############################################################

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/host/include
    ${CMAKE_CURRENT_BINARY_DIR}/host/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

##############################################################
# Setup CMake Module Path to include local files
##############################################################

LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

##############################################################
# Create Uninstall Target
##############################################################

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

ADD_CUSTOM_TARGET(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

##############################################################
# Install Package Docs
##############################################################

INCLUDE(UHDComponent)
UHD_INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/README.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/AUTHORS.txt
    DESTINATION ${PKG_DOC_DIR}
    COMPONENT readme
)

##############################################################
# Add subdirectories
##############################################################

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(host)

##############################################################
# Final summary of components to build and install
##############################################################

UHD_PRINT_COMPONENT_SUMMARY()
