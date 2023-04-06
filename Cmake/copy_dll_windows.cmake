function( copy_dll_windows exe )

  get_property( _sdk TARGET Millicast::MillicastSDK PROPERTY LOCATION )
  get_property( _ndi TARGET Millicast::NDI PROPERTY IMPORTED_LOCATION )

  add_custom_command( TARGET ${exe} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_sdk} ${_ndi} $<TARGET_FILE_DIR:${exe}>
  )

endfunction()