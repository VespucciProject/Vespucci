set(SOURCES    
    settingsdialog.cpp
    mainwindow.cpp
    pythonshelldialog.cpp
    scriptdialog.cpp      
    macrodialog.cpp
    settingsdialog.ui
)
set(DIRS
    Analysis
    Processing 
    QAbstractItemModel    
    Display		
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
