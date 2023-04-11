# Copyright 2023 David Baldassin @ Dolby.io
cmake_minimum_required( VERSION 3.16 FATAL_ERROR )

# --- Extend Cmake with our own modules and scripts
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} # for integration in superbuilds
  ${CMAKE_CURRENT_SOURCE_DIR}/Cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/millicast-sdk
)

#-------------------------------------------------
# --- Project and Options ------------------------
#-------------------------------------------------

project( millicast-metadata-publisher VERSION 1.0.0 )

if( APPLE )
  option( USE_NOTARIZATION "Should we notarize app at install time" OFF )
endif()
  
if( UNIX AND NOT APPLE )
  set( LINUX ON )
endif()
  
#-------------------------------------------------
# Packages and external dependencies -------------
# #-------------------------------------------------
set( MILLICAST_SDK_ROOT "${MillicastSDK_DIR}/../.." )

find_package( MillicastSDK REQUIRED )
message( STATUS "MillicastSDK found here : ${MillicastSDK_DIR}" )
  
#-------------------------------------------------
# --- External Source Code
#-------------------------------------------------

#-------------------------------------------------
# --- build
#-------------------------------------------------

if( APPLE )
  # Set apple visibility
  set( CMAKE_CXX_FLAGS "-fvisibility=hidden ${CMAKE_CXX_FLAGS}" )
  set( CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO" )
endif()

# -- Set the executables names
set( _exe metadata-publisher )

include( copy_dll_windows )

add_subdirectory( src )
