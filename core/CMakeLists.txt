set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)
set(COREINCDIR ${INCDIR}/core)

# core source files
file(GLOB_RECURSE CORESRCS *.cpp *.c)
file(GLOB_RECURSE COREINCS CONFIGURE_DEPENDS ${COREINCDIR}/*.h)

# VS filters bassed in real directory structure
source_group(TREE ${CMAKE_SOURCE_DIR}/core PREFIX "core" FILES ${CORESRCS})
source_group(TREE ${COREINCDIR} PREFIX "include\\core" FILES ${COREINCS})

if (EMSCRIPTEN)
    add_library(mccore STATIC ${CORESRCS} ${COREINCS})
    target_link_options(mccore PRIVATE "-sASYNCIFY" "-sASYNCIFY_IMPORTS=WebFS_Mount,WebFS_Sync,WebFS_Download")
    target_link_libraries(mccore PRIVATE zlibstatic)
else()
    add_library(mccore SHARED ${CORESRCS} ${COREINCS})
    set(GLADINCDIR ${LIBINCDIR}/gfx)
    target_include_directories(mccore PRIVATE ${GLADINCDIR})
    target_compile_definitions(mccore PRIVATE MC_EXPORT_DLL=1)
    target_link_libraries(mccore PRIVATE glfw gfx zlib)
endif()

source_group(TREE ${CMAKE_SOURCE_DIR}/core PREFIX "core" FILES ${CORESRCS})
source_group(TREE ${COREINCDIR} PREFIX "include\\core" FILES ${COREINCS})
target_include_directories(mccore PRIVATE ${stb_SOURCE_DIR} ${gleq_SOURCE_DIR})
target_include_directories(mccore PRIVATE ${INCDIR} ${COREINCDIR} ${LIBINCDIR})
target_include_directories(mccore PRIVATE ${stb_SOURCE_DIR} ${zlib_SOURCE_DIR})

target_link_libraries(mccore PRIVATE glm stb spdlog::spdlog)

install(TARGETS mccore RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)