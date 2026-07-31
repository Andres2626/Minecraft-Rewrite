set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)
set(COREINCDIR ${INCDIR}/core)
set(GAMEINCDIR ${INCDIR}/game)

# core source files
file(GLOB_RECURSE GAMESRCS *.cpp *.c)
file(GLOB_RECURSE GAMEINCS CONFIGURE_DEPENDS ${GAMEINCDIR}/*.h)

add_executable(${MCVER} ${GAMESRCS} ${GAMEINCS})

# VS filters bassed in real directory structure
source_group(TREE ${CMAKE_SOURCE_DIR}/game PREFIX "game" FILES ${GAMESRCS})
source_group(TREE ${GAMEINCDIR} PREFIX "include\\game" FILES ${GAMEINCS})
set_property(TARGET ${MCVER} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# dependencies include directory
target_include_directories(${MCVER} PRIVATE ${stb_SOURCE_DIR})
target_include_directories(${MCVER} PRIVATE ${gleq_SOURCE_DIR})
target_include_directories(${MCVER} PRIVATE ${glm_SOURCE_DIR})

# include directory
target_include_directories(${MCVER} PRIVATE ${INCDIR})
target_include_directories(${MCVER} PRIVATE ${COREINCDIR})
target_include_directories(${MCVER} PRIVATE ${GAMEINCDIR})
target_include_directories(${MCVER} PRIVATE ${LIBINCDIR})

# link all used libraries
if(EMSCRIPTEN)
    target_link_options(${MCVER} PRIVATE "-sASYNCIFY" "-sASYNCIFY_IMPORTS=WebFS_Mount,WebFS_Sync,WebFS_Download" "--use-port=contrib.glfw3" "-sMAX_WEBGL_VERSION=2" "-sMIN_WEBGL_VERSION=2")
    target_link_libraries(${MCVER} PRIVATE zlibstatic)
    set_target_properties(${MCVER} PROPERTIES SUFFIX ".html")
else()
    set(GLADINCDIR ${LIBINCDIR}/gfx)
    target_include_directories(${MCVER} PRIVATE ${GLADINCDIR})
    target_link_libraries(${MCVER} PRIVATE zlib glfw gfx)
endif()

target_link_libraries(${MCVER} PRIVATE mccore glm stb spdlog::spdlog)

# copy .dll files to .exe directory
if(WIN32 AND NOT EMSCRIPTEN)
    add_custom_command(TARGET ${MCVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:mccore> $<TARGET_FILE_DIR:${MCVER}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${MCVER}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:${MCVER}>)
endif()

install(TARGETS ${MCVER} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
   