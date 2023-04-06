message( STATUS "Configuring build for Millicast Metadata Publisher POC" )

macro( set_compiler_settings target )
  set_target_properties( ${target}
    PROPERTIES CXX_STANDARD 20
    )

  target_compile_options( ${target} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4127>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wno-unused-parameter>
    )

    if( WIN32 )
      target_compile_definitions( ${target} PUBLIC _CRT_SECURE_NO_WARNINGS )
    endif()
endmacro()

add_executable( ${_exe}
  main.cpp
)

set_compiler_settings( ${_exe} )

target_link_libraries( ${_exe} PRIVATE Millicast::MillicastSDK )

if( NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL Debug )
  message(STATUS "Adding debug compile definitions")
  target_compile_definitions( ${_exe} PRIVATE DEBUG_BUILD )
endif()

if( APPLE )
  set_target_properties( ${_exe} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist"
    INSTALL_RPATH @executable_path/
    BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

if( WIN32 )
  copy_dll_windows(${_exe})
endif()
