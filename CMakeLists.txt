cmake_minimum_required(VERSION 3.10.1)
project(wrp_io VERSION 0.1.5)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

## Project Options
option(BUILD_TESTS OFF)
# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
if (NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX _d)
endif()
if (NOT CMAKE_RELWITHDEBINFO_POSTFIX)
    set(CMAKE_RELWITHDEBINFO_POSTFIX _rd)
endif()


## Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Set compiler to use c++ 11 features
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Chosse build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${default_build_type})
endif()

## Use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# Build libraries
find_package(Threads REQUIRED)


if (NOT MSVC)
    # import asio library
    add_library(asio INTERFACE)
    target_compile_definitions(asio INTERFACE "-DASIO_STANDALONE")
    target_include_directories(asio INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/asio/include>
        $<INSTALL_INTERFACE:include>)
    add_library(westonrobot::asio ALIAS asio)
else()
    find_package(asio CONFIG REQUIRED)
    add_library(westonrobot::asio ALIAS asio::asio)
endif()

add_library(${PROJECT_NAME} 
    include/wrp_io/ring_buffer.hpp
    include/wrp_io/async_serial.hpp
    include/wrp_io/async_listener.hpp
    include/wrp_io/async_can.hpp
    src/async_serial.cpp
    src/async_can.cpp
)

target_link_libraries(${PROJECT_NAME} westonrobot::asio Threads::Threads)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/wrp_io>
        $<$<CXX_COMPILER_ID:MSVC>:$<INSTALL_INTERFACE:include/wrp_io/win_dummy_can>>
    PRIVATE 
        src
        $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/src/win_dummy_can>
)

# Build tests
if(BUILD_TESTS)
    message(STATUS "Tests will be built")
    enable_testing()
    add_subdirectory(test)
else()
    message(STATUS "Tests will not be built")
endif()

## Installation configuration

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX} with 'make install'")
# Offer the user the choice of overriding the installation directories
# targets to install

install(TARGETS ${PROJECT_NAME}
    EXPORT wrp_ioTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

if (NOT MSVC)
    install(TARGETS asio
        EXPORT wrp_ioTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
    install(DIRECTORY include/asio/include/asio
        DESTINATION include/wrp_io)
    install(FILES include/asio/include/asio.hpp
        DESTINATION include/wrp_io)
else()
    install(DIRECTORY src/win_dummy_can
        DESTINATION include/wrp_io/)
endif()

install(DIRECTORY include/wrp_io
    DESTINATION include/wrp_io)

# export target configuration 
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    wrp_ioConfigVersion.cmake
    VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion)
install(EXPORT wrp_ioTargets
    FILE wrp_ioTargets.cmake
    NAMESPACE westonrobot::
    DESTINATION lib/cmake/wrp_io)
configure_file(cmake/wrp_ioConfig.cmake.in wrp_ioConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wrp_ioConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/wrp_ioConfigVersion.cmake"
        DESTINATION lib/cmake/wrp_io)
# Packaging support
set(CPACK_PACKAGE_VENDOR "Weston Robot")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Weston Robot Platform - Linux Driver for IO")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/weston_robot")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruixiang Du (ruixiang.du@westonrobot.com)") 
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libasio-dev")
set(CPACK_SOURCE_IGNORE_FILES .git dist .*build.* /\\\\.DS_Store)
include(CPack)

