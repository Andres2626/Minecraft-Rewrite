# set all variables
set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)
set(GLADINCDIR ${LIBINCDIR}/gfx)

# GL includes
find_package(OpenGL REQUIRED)

# File for GLAD library
add_library(gfx STATIC glad.c)

# include directory
target_include_directories(gfx PRIVATE ${INCDIR})
target_include_directories(gfx PRIVATE ${LIBINCDIR})
target_include_directories(gfx PRIVATE ${GLADINCDIR})