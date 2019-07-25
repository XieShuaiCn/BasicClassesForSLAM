cmake_minimum_required(VERSION 3.0)
project(SLAM)

##################### basic settings ##############################
set(SLAM_LIBRARY_NAME "Slam")

include(cmake_modules/SlamUtils.cmake)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -ggdb -W -Wall ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS_ALL ${CMAKE_CXX_FLAGS_DEBUG})
else ()
    set(CMAKE_CXX_FLAGS_ALL ${CMAKE_CXX_FLAGS_RELEASE})
endif ()

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message(STATUS "Using flag -std=c++11.")
else ()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()
set(BUILD_LIBS_NAME "")

################# options for build ################################

option(BUILD_SHARED_LIBS "Build shared library." ON)

option(BUILD_SLAM_CORE "Build the library of SLAM core." ON)
option(BUILD_TEST "Build the test cases." ON)
option(BUILD_TOOLS "Build the tool sets." ON)
option(SLAM_ENABLE_POINT "Use feature points in tracking and mapping." ON)
option(SLAM_ENABLE_CUDA "Use GPU to accelerate computing." ON)
option(SLAM_SINGLE_THREAD "Complete all work in one thread." OFF)

######################## find librarys on system######################
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(OpenCV 3.4 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)
find_package(yaml-cpp)

if(OpenCV_FOUND)
    set(SLAM_HAVE_OPENCV ON)
endif()
if(Eigen3_FOUND OR EIGEN3_FOUND)
    set(SLAM_HAVE_EIGEN ON)
endif()
if (yaml-cpp_FOUND)
    set(SLAM_HAVE_YAML ON)
endif ()

include_directories(
        ${CMAKE_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIR}
        ${OPENCV_INCLUDE_DIRS}
)
if (SLAM_HAVE_YAML)
    include_directories(${YAML_CPP_INCLUDE_DIR})
endif()

# build something
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_INSTALL_PREFIX           ${PROJECT_SOURCE_DIR}/bin)
set(BUILD_BINARY_NAME "")

file(GLOB_RECURSE SOURCE_INCLUDE_FILES include/*.h include/*.hpp LineFlow/*.h)
file(GLOB_RECURSE SOURCE_SRC_FILES src/*.cc src/*.cpp LineFlow/*.cpp)


# build core library
add_library(${SLAM_LIBRARY_NAME}
        ${SOURCE_INCLUDE_FILES}
        ${SOURCE_SRC_FILES})

target_link_libraries(${SLAM_LIBRARY_NAME}
        ${OpenCV_LIBS}
        ${EIGEN3_LIBS}
        )
if (SLAM_HAVE_YAML)
    target_link_libraries(${SLAM_LIBRARY_NAME} ${YAML_CPP_LIBRARIES})
endif()

if (BUILD_TEST)
    add_subdirectory(test bin)

    enable_testing()
    add_test(NAME Test_SlamYaml_int COMMAND ReadYaml ${CMAKE_SOURCE_DIR}/test/data.yaml i num)
    set_tests_properties(Test_SlamYaml_int PROPERTIES PASS_REGULAR_EXPRESSION 1000 FAIL_REGULAR_EXPRESSION "FAIL")
    add_test(NAME Test_SlamYaml_float COMMAND ReadYaml ${CMAKE_SOURCE_DIR}/test/data.yaml f fx)
    set_tests_properties(Test_SlamYaml_float PROPERTIES PASS_REGULAR_EXPRESSION 356.45 FAIL_REGULAR_EXPRESSION "FAIL")
endif(BUILD_TEST)

include(cmake_modules/SlamConfig.cmake)

status("")
status("============================================================================")
status("")
status("    Project Name:       ${PROJECT_NAME}")
status("    Build Time:         ${CMAKE_CURRENT_TIME}")
status("----------------------------------------------------------------------------")
status_list("    OpenCV:             (${OpenCV_VERSION})" ${OpenCV_INCLUDE_DIRS})
status_list("    Eigen3:             (${EIGEN3_VERSION_STRING})" ${EIGEN3_INCLUDE_DIR})
if (SLAM_HAVE_BOOST)
status_list("    Boost:              (${Boost_VERSION})" ${Boost_INCLUDE_DIRS})
else()
    status( "    Boost:              (No)")
endif()
if (SLAM_HAVE_YAML)
status_list("    YAML:               (${yaml-cpp_VERSION})" ${YAML_CPP_INCLUDE_DIR})
else()
    status( "    YAML:               (No)")
endif()
status("----------------------------------------------------------------------------")
status("    Build Type:         ${CMAKE_BUILD_TYPE}")
status("    C++ Flags:          ${CMAKE_CXX_FLAGS_ALL}")
status("    Project Path:       ${PROJECT_SOURCE_DIR}")
status("    Output Path:        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
status("    CMake Path:         ${CMAKE_BINARY_DIR}")
status("============================================================================")
status("")
