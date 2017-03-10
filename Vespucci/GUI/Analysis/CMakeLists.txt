set(SOURCES
    ahcadialog.cpp
    kmeansdialog.cpp
    principalcomponentsdialog.cpp
    bandratiodialog.cpp		    
    metaanalysisdialog.cpp
    representativespectrumdialog.cpp
    classicalleastsquaresdialog.cpp     
    multianalysisdialog.cpp
    transformdialog.cpp
    dimensionalityestimationdialog.cpp
    univariatedialog.cpp
    plotmakerdialog.cpp      
    vcadialog.cpp
    hypothesistestdialog.cpp
    plsdialog.cpp	     
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
