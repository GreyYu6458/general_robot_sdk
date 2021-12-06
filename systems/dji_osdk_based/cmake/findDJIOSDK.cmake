include(ExternalProject)

SET(DJI_OSDK_INSTALL_DIR ${CMAKE_BINARY_DIR}/dji_osdk)
SET(DJI_OSDK_TAG "insky_log_callback")

ExternalProject_Add(
    dji_osdk
  GIT_REPOSITORY
    git@gitee.com:insky-cache/insky-dji-osdk.git
  GIT_TAG           
    ${DJI_OSDK_TAG}
  GIT_PROGRESS
    1
  UPDATE_COMMAND 
    "" # git pull origin ${DJI_OSDK_TAG} && git checkout ${DJI_OSDK_TAG} && git submodule update
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${DJI_OSDK_INSTALL_DIR}"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
)

OPTION(BUILD_DJI_TEST ON)

SET(DJI_OSDK_LIBRARY_DIR ${DJI_OSDK_INSTALL_DIR}/lib)
SET(DJI_OSDK_INCLUDE_DIR ${DJI_OSDK_INSTALL_DIR}/include)

link_directories(${DJI_OSDK_LIBRARY_DIR})
include_directories(${DJI_OSDK_INCLUDE_DIR})

add_definitions(-DADVANCED_SENSING)
SET(OSDK_CORE_LIBRARY     ${DJI_OSDK_LIBRARY_DIR}/libdjiosdk-core.a)
SET(OSDK_ADVANCED_LIBRARY ${DJI_OSDK_LIBRARY_DIR}/libadvanced-sensing.a)
SET(OSDK_LINKER_LIBRARY   ${DJI_OSDK_LIBRARY_DIR}/libdji-linker.a)