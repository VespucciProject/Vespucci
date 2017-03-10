set(DIRS
    Data
    Global
    Math
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

set(LIBVESPUCCI_SRCS ${LIBVESPUCCI_SRCS} PARENT_SCOPE)
