#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Millicast::MillicastSDK" for configuration "Release"
set_property(TARGET Millicast::MillicastSDK APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Millicast::MillicastSDK PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/MillicastSDK.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/MillicastSDK.dll"
  )

list(APPEND _cmake_import_check_targets Millicast::MillicastSDK )
list(APPEND _cmake_import_check_files_for_Millicast::MillicastSDK "${_IMPORT_PREFIX}/lib/MillicastSDK.lib" "${_IMPORT_PREFIX}/bin/MillicastSDK.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
