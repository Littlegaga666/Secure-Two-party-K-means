
# This is the minimum cmake requirement
cmake_minimum_required(VERSION 3.12)
project(bioSecure LANGUAGES CXX)

# Adding cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Disable the search for boost-cmake
set(Boost_NO_BOOST_CMAKE ON)

# Set build type to `Release`
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Write built executables and libraries to bin/ and lib/, respectively.
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

# Build Encrypto_utils
find_package(ENCRYPTO_utils QUIET)
if(ENCRYPTO_utils_FOUND)
    message(STATUS "Found ENCRYPTO_utils")
elseif(NOT ENCRYPTO_utils_FOUND AND NOT TARGET ENCRYPTO_utils::encrypto_utils)
    message("ENCRYPTO_utils was not found: add ENCRYPTO_utils subdirectory")
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/ENCRYPTO_utils/CMakeLists.txt")
        find_package(Git REQUIRED)
        message("initialize Git submodule: extern/ENCRYPTO_utils")
        execute_process(COMMAND git submodule update --init extern/ENCRYPTO_utils
                        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
    endif()
    add_subdirectory(extern/ENCRYPTO_utils)
endif()

# Build OTExtension, remember OTExtension depends on Encrypto_utils
find_package(OTExtension QUIET)
if(OTExtension_FOUND)
    message(STATUS "Found OTExtension")
elseif (NOT OTExtension_FOUND AND NOT TARGET OTExtension::otextension)
    message("OTExtension was not found: add OTExtension subdirectory")
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/OTExtension/CMakeLists.txt")
        find_package(Git REQUIRED)
        message("initialize Git submodule: extern/OTExtension")
        execute_process(COMMAND git submodule update --init extern/OTExtension
                        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
    endif()
    add_subdirectory(extern/OTExtension)
endif()

# Required packages
find_package(GMP REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.66.0 REQUIRED COMPONENTS thread system filesystem chrono)

add_subdirectory(extern/libpaillier-0.8)
add_subdirectory(src/abycore)
add_subdirectory(src/utils)
add_subdirectory(src/algorithms)
add_subdirectory(src/tests)