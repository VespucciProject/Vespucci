set(SOURCES
    abstractdataanalyzer.cpp 
    matrixanalyzer.cpp 
    principalcomponentsdata.cpp 
    multianalyzer.cpp
    analysisresults.cpp
    metaanalyzer.cpp
    univariatedata.cpp
    plsdata.cpp
    mlpackpcadata.cpp
    vcadata.cpp
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} PARENT_SCOPE)
