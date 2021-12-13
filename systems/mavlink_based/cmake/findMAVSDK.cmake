include(ExternalProject)

SET(MAVSDK_INSTALL_DIR ${CMAKE_BINARY_DIR}/mavsdk)

ExternalProject_Add(
    mavsdk_project
  GIT_REPOSITORY
    https://github.com/mavlink/MAVSDK.git
  GIT_TAG           
    "v0.50.1"
  GIT_PROGRESS
    1
  UPDATE_COMMAND 
    ""
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${MAVSDK_INSTALL_DIR}"
    "-DBUILD_SHARED_LIBS=OFF"
)

SET(MAVSDK_LIBRARY_DIR ${MAVSDK_INSTALL_DIR}/lib)
SET(MAVSDK_INCLUDE_DIR ${MAVSDK_INSTALL_DIR}/include)

link_directories(${MAVSDK_LIBRARY_DIR})
include_directories(${MAVSDK_INCLUDE_DIR})
include_directories(${MAVSDK_INCLUDE_DIR}/mavsdk)
