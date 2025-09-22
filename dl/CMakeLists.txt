set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(LIBINCDIR ${INCDIR}/lib)

# core source files
file(GLOB_RECURSE DLSRCS dl.c loader.c)
file(GLOB_RECURSE DLINCS CONFIGURE_DEPENDS ${INCDIR}/dl.h ${INCDIR}/loader.h ${INCDIR}/errors.h ${INCDIR}/mod.h)

add_library(dl STATIC ${DLSRCS} ${DLINCS})

# VS filters bassed in real directory structure
source_group(TREE ${CMAKE_SOURCE_DIR}/dl PREFIX "dl" FILES ${DLSRCS})
source_group(TREE ${INCDIR} PREFIX "include\\dl" FILES ${DLINCS})

# include directory
target_include_directories(dl PRIVATE ${INCDIR})
target_include_directories(dl PRIVATE ${LIBINCDIR})