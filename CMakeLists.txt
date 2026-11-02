cmake_minimum_required(VERSION 3.8)
project(MCRewrite LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(MCVER c0.0.2a)

# Options
option(MC_ENABLE_PROFILER "Enable profiler mode" OFF)
option(MC_COMP_WARNS "Enable all/extra warnings and -Werror option (GCC) or /W4 in MSVC" OFF)
option(MC_AUTODEP "Installs dependencies via FetchContent" ON)

# Compiler warnings
if (MC_COMP_WARNS)
	if(MSVC)
		add_compile_options(/W4)
	else (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-Wall -Wextra -Werror)
	endif()
endif()

if (MC_ENABLE_PROFILER)
    add_compile_definitions(MC_ENABLE_PROFILER)
endif()

# for WIN32 systems avoid GLFW compilation errors
if (WIN32)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
endif()

# Unix systems. '-fPIC' option for '.a' and '.so' files.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(dependencies)
add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(game)

install(DIRECTORY assets/Shaders DESTINATION bin/assets)
install(DIRECTORY include DESTINATION include)
install(DIRECTORY Documentation DESTINATION man)
install(FILES LICENSE NEWS ${MCVER} TODO ChangeLog README.md DESTINATION .)