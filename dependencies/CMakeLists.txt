set(ZLIBREPO https://github.com/madler/zlib.git)
set(GLMREPO https://github.com/g-truc/glm.git)
set(STBREPO https://github.com/nothings/stb.git)
set(SPDLOGREPO https://github.com/gabime/spdlog.git)

set(ZLIB_BUILD_MINIZIP OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_WARNINGS OFF CACHE BOOL "" FORCE)

if (EMSCRIPTEN)
    set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_STATIC ON CACHE BOOL "" FORCE)
else()
    set(GLFWREPO https://github.com/glfw/glfw.git)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED ON CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_STATIC OFF CACHE BOOL "" FORCE)
endif()

if(MC_AUTODEP)

    include(FetchContent)
    
    # GLFW
    if (EMSCRIPTEN)
        message(STATUS "Using Emscripten GLFW")
    else ()
        FetchContent_Declare(glfw GIT_REPOSITORY ${GLFWREPO} GIT_TAG 3.4)
        FetchContent_MakeAvailable(glfw)
    endif()

    # ZLIB
    FetchContent_Declare(zlib GIT_REPOSITORY ${ZLIBREPO} GIT_TAG v1.3.2)
    FetchContent_MakeAvailable(zlib)

    # GLM
    FetchContent_Declare(glm GIT_REPOSITORY ${GLMREPO} GIT_TAG 1.0.3)
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
    add_library(stb INTERFACE)
    
    if (EMSCRIPTEN)
        message(STATUS "Using Emscripten GLFW")
    else ()
        add_subdirectory(GLFW)
    endif()
    
    add_subdirectory(glm)
    add_subdirectory(zlib)
    add_subdirectory(spdlog)
    target_include_directories(stb INTERFACE stb/)
endif()