set(SOURCES
    aboutdialog.cpp
    citationdialog.cpp
    colorrangedialog.cpp
    dataviewer.cpp
    datawidget.cpp
    globalgradientdialog.cpp
    historydialog.cpp
    mapdialog.cpp
    mapplot.cpp
    mapresizedialog.cpp
    mapviewer.cpp
    plotviewer.cpp
    plotwidget.cpp
    reportmessagedialog.cpp
    scalebardialog.cpp
    spectrumeditor.cpp
    statsdialog.cpp
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
