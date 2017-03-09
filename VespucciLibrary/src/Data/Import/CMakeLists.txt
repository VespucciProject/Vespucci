set(SOURCES
    textimport.cpp
    binaryimport.cpp
)

set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SRC_DIR}/${file})
endforeach()

set(LIBVESPUCCI_SRCS ${LIBVESPUCCI_SRCS} PARENT_SCOPE)
