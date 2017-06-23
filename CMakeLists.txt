cmake_minimum_required(VERSION 2.8.6)
project(gslam_vins)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)#
endif()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
      message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")# -march=x86-64 -mtune=generic
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
   add_definitions(-DG2O_OPENMP -DNDEBUG -fopenmp)
   #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--retain-symbols-file=${CMAKE_SOURCE_DIR}/cmake/static_export.sym -Wl,--version-script=${CMAKE_SOURCE_DIR}/cmake/shared_export.map")
endif()

include(${PROJECT_SOURCE_DIR}/cmake/PICMake.cmake)

include_directories(src)

if(WIN32)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,.:./libs:../libs:${CMAKE_INSTALL_PREFIX}/lib")
endif()

pi_collect_packages(VERBOSE REQUIRED OpenCV PIL OpenGL GLUT GLEW System MODULES Qt QGLViewer Eigen3 Ceres Glog Gflags)

if(NOT EIGEN3_FOUND)
    set(EIGEN3_INCLUDES ${PROJECT_SOURCE_DIR}/ThirdParty/internal/eigen3)
    set(EIGEN3_FOUND true)
    message("Using internal Eigen.")
endif()
set(NO_SIFTGPU TRUE)
if(NO_SIFTGPU)
    add_definitions(-DNO_SIFTGPU)
else()
    set(SIFTGPU_SOURCES ThirdParty/internal/SiftGPU)
endif()

pi_add_target(gslam_vins SHARED src REQUIRED OpenCV System EIGEN3 Ceres Glog Gflags)

