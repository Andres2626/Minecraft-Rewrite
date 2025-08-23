cmake_minimum_required(VERSION 3.8)
project(MCRewrite LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Options
option(MC_DEBUG_MODE "Enable debug mode" OFF)
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

# debug option
if(MC_DEBUG_MODE)
	add_compile_definitions(DEBUG)
else()
	add_compile_definitions(NDEBUG)
endif()

# Unix systems. '-fPIC' option for '.a' and '.so' files.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(dependencies)
add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(game)