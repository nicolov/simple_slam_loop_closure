cmake_minimum_required(VERSION 2.8)
project(simple_slam_loop_closure)

include(ExternalProject)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

add_definitions(-std=c++11 -Wall -O3 -march=native)

find_package(OpenCV 3 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
    ${EIGEN3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/install)

macro(GetDependency name other_dependency)
  find_package(${name} QUIET 
    PATHS ${DEPENDENCY_INSTALL_DIR})
  if(${${name}_FOUND})
    message("${name} library found, using it from the system")
    include_directories(${${name}_INCLUDE_DIRS})
    add_custom_target(${name})
  else(${${name}_FOUND})
    message("${name} library not found in the system, it will be downloaded on build")
    option(DOWNLOAD_${name}_dependency "Download ${name} dependency" ON)
    if(${DOWNLOAD_${name}_dependency})
      ExternalProject_Add(${name}
        PREFIX ${DEPENDENCY_DIR}
        GIT_REPOSITORY http://github.com/dorian3d/${name}
        GIT_TAG v1.1-nonfree
        INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        DEPENDS ${other_dependency})
    else()
      message(SEND_ERROR "Please, activate DOWNLOAD_${name}_dependency option or download manually")
    endif(${DOWNLOAD_${name}_dependency})
  endif(${${name}_FOUND})
endmacro(GetDependency)

GetDependency(DLib "")
GetDependency(DBoW2 DLib)
add_custom_target(Dependencies
  ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
  DEPENDS DBoW2 DLib)

include_directories(
  ${DEPENDENCY_DIR}/install/include)


add_executable(new_college
    src/new_college.cpp)
target_link_libraries(new_college
    ${OpenCV_LIBS}
    ${EIGEN3_LIBS}
    ${DLib_LIBS}
    ${DBoW2_LIBS})
add_dependencies(new_college
    Dependencies)
