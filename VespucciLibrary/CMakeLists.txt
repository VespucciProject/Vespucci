#we may need to add more dirs later
set (DIRS
    src
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set(LIBVESPUCCI_SRCS ${LIBVESPUCCI_SRCS} PARENT_SCOPE)
