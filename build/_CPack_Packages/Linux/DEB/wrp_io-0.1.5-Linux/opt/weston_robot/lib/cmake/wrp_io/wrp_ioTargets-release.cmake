#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "westonrobot::wrp_io" for configuration "Release"
set_property(TARGET westonrobot::wrp_io APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(westonrobot::wrp_io PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libwrp_io.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS westonrobot::wrp_io )
list(APPEND _IMPORT_CHECK_FILES_FOR_westonrobot::wrp_io "${_IMPORT_PREFIX}/lib/libwrp_io.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
