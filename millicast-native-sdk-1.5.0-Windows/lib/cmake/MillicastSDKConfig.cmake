
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

####################################################################################

if( NOT MCSDK_USE_FILE_INCLUDED )
  # Protect from re-run
  set ( MCSDK_USE_FILE_INCLUDED 1 )

  if( APPLE )
    if( IOS OR CMAKE_SYSTEM_NAME STREQUAL "tvOS" )

    else()
    # Check if system libraries are there
      find_library( OPENGL_LIBRARY       OpenGL       REQUIRED )
      find_library( APPKIT_LIBRARY       AppKit       REQUIRED )
      find_library( FOUNDATION_LIBRARY   Foundation   REQUIRED )
      find_library( AVFOUNDATION_LIBRARY AVFoundation REQUIRED )
      find_library( COREMEDIA_LIBRARY    CoreMedia    REQUIRED )
      find_library( COREGRAFX_LIBRARY    CoreGraphics REQUIRED )
      find_library( COREVIDEO_LIBRARY    CoreVideo    REQUIRED )
      find_library( COREAUDIO_LIBRARY    CoreAudio    REQUIRED )
      find_library( AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED )
      find_library( IOSURFACE_LIBRARY    IOSurface    REQUIRED )
      find_library( COCOA_LIBRARY        Cocoa        REQUIRED )
    endif()
  elseif( UNIX )
    find_package( X11 REQUIRED )

    if( NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
      message(FATAL_ERROR "Please use clang to compile an app with the MillicastSDK")
    endif()

    set( CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}" )
  endif()

  include( "${CMAKE_CURRENT_LIST_DIR}/MillicastSDKTargets.cmake" )

  if( ON )
    set( MCSDK_NDI_FOUND TRUE )
    get_filename_component( __lib_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY )
    if( WIN32 )
      set( __ndi_loc ${__lib_dir}/Processing.NDI.Lib.x64.lib )
      set( __ndi_dll_loc ${__lib_dir}/../bin/Processing.NDI.Lib.x64.dll )
    elseif( APPLE AND NOT IOS )
      set( __ndi_loc ${__lib_dir}/libndi.dylib )
    else()
      set( __ndi_loc ${__lib_dir}/libndi.so.5.1.1 )
    endif()

    add_library( Millicast::NDI SHARED IMPORTED )
    if( WIN32 )
      set_target_properties(
        Millicast::NDI PROPERTIES
        IMPORTED_LOCATION ${__ndi_dll_loc}
        IMPORTED_IMPLIB ${__ndi_loc}
        )
    else()
      set_target_properties(
        Millicast::NDI PROPERTIES
        IMPORTED_LOCATION ${__ndi_loc}
        )
    endif()
  else()
    set( MCSDK_NDI_FOUND FALSE )
  endif()

  if( ON )
    set( MCSDK_DECKLINK_FOUND TRUE )
  else()
    set( MCSDK_DECKLINK_FOUND FALSE )
  endif()

endif( NOT MCSDK_USE_FILE_INCLUDED )
