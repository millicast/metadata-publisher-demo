
# Copyright 2023 Dolby.io

macro( sign_and_notarize exe sub_wd )
  # -- Code signing
  if( XCODE_IDENTITY )
    message( STATUS "${exe} app bundle will be signed" )

    # codesign last, as otherwise, fix_bundle modifications will invalidate the signature
    install(
      CODE
        "set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT)
        execute_process( COMMAND
          codesign --timestamp --options runtime
            --entitlements ${CMAKE_SOURCE_DIR}/install/entitlements.plist
            --deep --force --sign ${XCODE_IDENTITY}
            \${CMAKE_INSTALL_PREFIX}/${sub_wd}/${exe}
        )"
      )

    if( USE_NOTARIZATION )
      set( ZIPNAME /tmp/${exe}.zip )
      install(
          CODE "
          set( CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT )
          execute_process(
              COMMAND ditto -c -k --keepParent ${exe} ${ZIPNAME}
              COMMAND xcrun altool --notarize-app --primary-bundle-id
              \"io.cosmosoftware.millicast.cli\"
              -p \"@keychain:AC_PASSWORD\" --file ${ZIPNAME}
              WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}/${sub_wd}
              )
          "
          )
    endif()
  else()
    message( WARNING "${exe} app bundle will not be signed. Re-run cmake with -DXCODE_IDENTITY=your_identity to enable code signing" )
  endif()
endmacro()
