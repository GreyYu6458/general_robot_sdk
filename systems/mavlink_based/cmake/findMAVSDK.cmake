include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

fetchcontent_declare(
        mavsdk_project
    GIT_REPOSITORY
        https://github.com/mavlink/MAVSDK.git
    GIT_TAG
        "v1.0.6"
    GIT_PROGRESS
        1
    CMAKE_ARGS
        "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_INSTALL_PREFIX:PATH=${MAVSDK_INSTALL_DIR}"
        "-DBUILD_SHARED_LIBS=OFF"
    UPDATE_COMMAND
        ""
)

fetchcontent_makeavailable(mavsdk_project)
