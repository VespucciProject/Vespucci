set(DIRS
    Import
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set (LIBVESPUCCI_SRC ${LIBVESPUCCI_SRC} PARENT_SCOPE)
