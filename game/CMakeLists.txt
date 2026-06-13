set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)
set(GLADINCDIR ${LIBINCDIR}/gfx)
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
target_include_directories(${MCVER} PRIVATE ${GLADINCDIR})

# link all used libraries
target_link_libraries(${MCVER} PRIVATE mccore glfw gleq zlib glm stb gfx spdlog)

# copy .dll files to .exe directory
add_custom_command(TARGET ${MCVER} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:mccore> $<TARGET_FILE_DIR:${MCVER}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${MCVER}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:${MCVER}>)
    
install(TARGETS ${MCVER} RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
   