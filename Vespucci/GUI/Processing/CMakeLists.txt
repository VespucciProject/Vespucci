set(SOURCES
    abscissainterpolationdialog.cpp
    abscissatransformdialog.cpp
    baselinedialog.cpp
    binarizedialog.cpp
    bulkconversiondialog.cpp
    cropdialog.cpp
    datasetextractordialog.cpp
    datasetimportdialog.cpp
    filterdialog.cpp
    fouriertransformdialog.cpp
    matrixselectiondialog.cpp
    metadatasetdialog.cpp
    multiimportdialog.cpp
    rangedialog.cpp
    stitchimportdialog.cpp
    textimportdialog.cpp
    thresholddialog.cpp
    univariateconcatenationdialog.cpp
)
set(DIR_SRCS)
foreach(file ${SOURCES})
    set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
set(VESPUCCI_SRCS ${VESPUCCI_SRCS} ${DIR_SRCS} PARENT_SCOPE)
