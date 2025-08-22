set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)
set(GLADINCDIR ${LIBINCDIR}/gfx)
set(COREINCDIR ${INCDIR}/core)

# core source files
file(GLOB_RECURSE CORESRCS *.cpp *.c)
file(GLOB_RECURSE COREINCS CONFIGURE_DEPENDS ${COREINCDIR}/*.h)

add_library(mccore SHARED ${CORESRCS} ${COREINCS})

# VS filters bassed in real directory structure
source_group(TREE ${CMAKE_SOURCE_DIR}/core PREFIX "core" FILES ${CORESRCS})
source_group(TREE ${COREINCDIR} PREFIX "include\\core" FILES ${COREINCS})

target_compile_definitions(mccore PRIVATE MC_EXPORT_DLL=1)

# dependencies include directory
target_include_directories(mccore PRIVATE ${stb_SOURCE_DIR})
target_include_directories(mccore PRIVATE ${gleq_SOURCE_DIR})
target_include_directories(mccore PRIVATE ${glm_SOURCE_DIR})

# include directory
target_include_directories(mccore PRIVATE ${INCDIR})
target_include_directories(mccore PRIVATE ${COREINCDIR})
target_include_directories(mccore PRIVATE ${LIBINCDIR})
target_include_directories(mccore PRIVATE ${GLADINCDIR})

# link all used libraries
target_link_libraries(mccore PRIVATE glfw gleq zlib glm stb gfx)