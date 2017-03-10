set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(DIRS
    Data
    External
    Global
    GUI
)
set(SOURCES
    main.cpp
    qcustomplot.cpp
    resources.qrc
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
