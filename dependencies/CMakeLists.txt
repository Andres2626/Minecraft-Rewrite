set(GLFWREPO https://github.com/glfw/glfw.git)
set(ZLIBREPO https://github.com/madler/zlib.git)
set(GLEQREPO https://github.com/glfw/gleq.git)
set(GLMREPO https://github.com/g-truc/glm.git)
set(STBREPO https://github.com/nothings/stb.git)
set(SPDLOGREPO https://github.com/gabime/spdlog.git)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

set(ZLIB_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(ZLIB_BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_MINIZIP OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_WARNINGS OFF CACHE BOOL "" FORCE)

if(MC_AUTODEP)

    include(FetchContent)
    # GLFW
    FetchContent_Declare(glfw GIT_REPOSITORY ${GLFWREPO} GIT_TAG 3.4)
    FetchContent_MakeAvailable(glfw)

    # ZLIB
    FetchContent_Declare(zlib GIT_REPOSITORY ${ZLIBREPO} GIT_TAG master)
    FetchContent_MakeAvailable(zlib)

    # GLEQ
    FetchContent_Declare(gleq GIT_REPOSITORY ${GLEQREPO} GIT_TAG master)
    FetchContent_MakeAvailable(gleq)

    add_library(gleq INTERFACE)
    target_include_directories(gleq INTERFACE ${gleq_SOURCE_DIR})

    # GLM
    FetchContent_Declare(glm GIT_REPOSITORY ${GLMREPO} GIT_TAG master)
    FetchContent_MakeAvailable(glm)

    # STB
    FetchContent_Declare(stb GIT_REPOSITORY ${STBREPO} GIT_TAG master)
    FetchContent_MakeAvailable(stb)

    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

    # SPDLOG
    FetchContent_Declare(spdlog GIT_REPOSITORY ${SPDLOGREPO} GIT_TAG v1.17.0)
    FetchContent_MakeAvailable(spdlog)

else()
    add_library(gleq INTERFACE)
    add_library(stb INTERFACE)
    add_subdirectory(GLFW)
    add_subdirectory(glm)
    add_subdirectory(zlib)
    add_subdirectory(spdlog)
    target_include_directories(gleq INTERFACE gleq/)
    target_include_directories(stb INTERFACE stb/)
endif()