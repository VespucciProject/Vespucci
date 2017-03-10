set(SOURCES
    datasetlistmodel.cpp
    datasettreemodel.cpp
    filetablemodel.cpp
    maplistmodel.cpp
    spectratablemodel.cpp
    treeitem.cpp
    vespuccitablemodel.cpp
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
