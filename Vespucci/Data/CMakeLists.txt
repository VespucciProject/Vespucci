set(DIRS
    Analysis
    Dataset
    Imaging
    Import
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS}  PARENT_SCOPE)
