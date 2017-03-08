set(DIRS
    Accessory
    Baseline
    Clustering
    DimensionReduction
    Fitting
    Normalization
    PeakFinding
    Quantification
    Smoothing
    Stats
    Transform
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()
set(LIBVESPUCCI_SRCS ${LIBVESPUCCI_SRCS} PARENT_SCOPE)
