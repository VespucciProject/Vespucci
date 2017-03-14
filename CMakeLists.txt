cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_CXX_STANDARD 11)
project(vespucci CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
option(BUILD_GUI "Build the GUI application, set to OFF to build only Library" ON)
option(BUILD_SHARED_LIBS "Build shared libraries (.so, .dll, .dylib)" ON)
# If using clang, we have to link against libc++ depending on thea
# OS (at least on some systems). Further, gcc sometimes optimizes calls to
# math.h functions, making -lm unnecessary with gcc, but it may still be
# necessary with clang.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if (APPLE)
    # detect OS X version. Use '/usr/bin/sw_vers -productVersion' to
    # extract V from '10.V.x'.)
    exec_program(/usr/bin/sw_vers ARGS
        -productVersion OUTPUT_VARIABLE MACOSX_VERSION_RAW)
    string(REGEX REPLACE
        "10\\.([0-9]+).*" "\\1"
        MACOSX_VERSION
        "${MACOSX_VERSION_RAW}")

     # OSX Lion (10.7) and OS X Mountain Lion (10.8) doesn't automatically
     # select the right stdlib.
    if(${MACOSX_VERSION} LESS 9)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS
          "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_MODULE_LINKER_FLAGS
          "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
     endif()
  endif()

  # Link everything with -lm.
  set(COMPILER_SUPPORT_LIBRARIES ${COMPILER_SUPPORT_LIBRARIES} "m")
  set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} "m")
endif()

# Debugging CFLAGS.  Turn optimizations off; turn debugging symbols on.
if(DEBUG)
  if (NOT MSVC)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -ftemplate-backtrace-limit=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -O0")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  endif()

  # mlpack uses it's own mlpack::backtrace class based on Binary File Descriptor
  # <bfd.h> and linux Dynamic Loader <libdl.h> and more portable version in future
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Bfd)
    find_package(LibDL)
    if(LIBBFD_FOUND AND LIBDL_FOUND)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic")
      set(LIBVESPUCCI_INCLUDE_DIRS ${LIBVESPUCCI_INCLUDE_DIRS} ${LIBBFD_INCLUDE_DIRS}
          ${LIBDL_INCLUDE_DIRS})
      set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} ${LIBBFD_LIBRARIES}
          ${LIBDL_LIBRARIES})
      add_definitions(-DHAS_BFD_DL)
    else()
      message(WARNING "No libBFD and/or libDL has been found!")
    endif()
  endif()
else()
  add_definitions(-DARMA_NO_DEBUG)
  add_definitions(-DNDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3")
endif()

# Profiling CFLAGS.  Turn profiling information on.
if(CMAKE_COMPILER_IS_GNUCC AND PROFILE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# Now, find the libraries we need to compile against.  Several variables can be
# set to manually specify the directory in which each of these libraries
# resides.
#   ARMADILLO_LIBRARY - location of libarmadillo.so / armadillo.lib
#   ARMADILLO_INCLUDE_DIR - directory containing <armadillo>
#   ARMADILLO_INCLUDE_DIRS - directories necessary for Armadillo includes
#   BOOST_ROOT - root of Boost installation
#   BOOST_INCLUDEDIR - include directory for Boost
#   BOOST_LIBRARYDIR - library directory for Boost
#   MATHJAX_ROOT - root of MathJax installation

find_package(Armadillo 4.200.0 REQUIRED)

# On Windows, Armadillo should be using LAPACK and BLAS but we still need to
# link against it.  We don't want to use the FindLAPACK or FindBLAS modules
# because then we are required to have a FORTRAN compiler (argh!) so we will try
# and find LAPACK and BLAS ourselves, using a slightly modified variant of the
# script Armadillo uses to find these.
if (WIN32)
  find_library(LAPACK_LIBRARY
      NAMES lapack liblapack lapack_win32_MT lapack_win32
      PATHS "C:/Program Files/Armadillo"
      PATH_SUFFIXES "examples/lib_win32/")

  if (NOT LAPACK_LIBRARY)
    message(FATAL_ERROR "Cannot find LAPACK library (.lib)!")
  endif ()

  find_library(BLAS_LIBRARY
      NAMES blas libblas blas_win32_MT blas_win32
      PATHS "C:/Program Files/Armadillo"
      PATH_SUFFIXES "examples/lib_win32/")

  if (NOT BLAS_LIBRARY)
    message(FATAL_ERROR "Cannot find BLAS library (.lib)!")
  endif ()

  # Piggyback LAPACK and BLAS linking into Armadillo link.
  set(ARMADILLO_LIBRARIES
       ${ARMADILLO_LIBRARIES} ${BLAS_LIBRARY} ${LAPACK_LIBRARY})
endif ()

# Unfortunately this configuration variable is necessary and will need to be
# updated as time goes on and new versions are released.
set(Boost_ADDITIONAL_VERSIONS
  "1.55.0" "1.56.0" "1.57.0" "1.58.0" "1.59.0" "1.60.0" "1.61.0" "1.62.0" "1.63.0")
find_package(Boost 1.49
    COMPONENTS
      program_options
      unit_test_framework
      serialization
      random
      math_c99
      REQUIRED
)
link_directories(${Boost_LIBRARY_DIRS})

# In Visual Studio, automatic linking is performed, so we don't need to worry
# about it.  Clear the list of libraries to link against and let Visual Studio
# handle it.
if (MSVC)
  link_directories(${Boost_LIBRARY_DIRS})
  set(Boost_LIBRARIES "")
endif ()

set(LIBVESPUCCI_INCLUDE_DIRS ${LIBVESPUCCI_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} ${Boost_LIBRARIES})
set(LIBVESPUCCI_LIBRARY_DIRS ${LIBVESPUCCI_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})

# Detect OpenMP support in a compiler. If the compiler supports OpenMP, flags
# to compile with OpenMP are returned and added and the HAS_OPENMP definition is
# added for compilation.
#
# This way we can skip calls to functions defined in omp.h with code like:
# #ifdef HAS_OPENMP
# {
#   ... openMP code here ...
# }
# #endif
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
  add_definitions(-DHAS_OPENMP)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else ()
  # Disable warnings for all the unknown OpenMP pragmas.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
endif ()

# Include directories for the previous dependencies.
set(LIBVESPUCCI_INCLUDE_DIRS ${LIBVESPUCCI_INCLUDE_DIRS} ${ARMADILLO_INCLUDE_DIRS})
set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} ${ARMADILLO_LIBRARIES})


# Store the mlpack include directory in MLPACK_INCLUDE_DIR.
# The PATHS variable may be specified to give hints for where to find core.hpp.
find_path(MLPACK_INCLUDE_DIR
  NAMES core.hpp prereqs.hpp
  PATHS "$ENV{ProgramFiles}/mlpack/" 
         /usr/local/include/
         /usr/local/include/mlpack
         /usr/include
         /usr/include/mlpack
         ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/mlpack
  REQUIRED
)

# Find libmlpack.so (or equivalent) and store it in MLPACK_LIBRARY.
# If this example script were smarter, it would also find other dependencies of mlpack and store them in
# an MLPACK_LIBRARIES variable instead.
find_library(MLPACK_LIBRARY
  NAMES mlpack
  PATHS "$ENV{ProgramFiles}/mlpack/"
         ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/mlpack
         /usr/lib64/ /usr/lib/ /usr/local/lib64/ /usr/local/
  REQUIRED
)

set(LIBVESPUCCI_INCLUDE_DIRS ${LIBVESPUCCI_INCLUDE_DIRS} ${MLPACK_INCLUDE_DIR})
set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} ${MLPACK_LIBRARY})

if (BUILD_GUI)
    find_package(Qt5 
        COMPONENTS
         Core
         Gui
         Svg
         Widgets
         PrintSupport
         REQUIRED
    )

    set(LIBVESPUCCI_INCLUDE_DIRS ${LIBVESPUCCI_INCLUDE_DIRS} 
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Svg_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5PrintSupport_INCLUDE_DIRS})
    set(LIBVESPUCCI_LIBRARIES ${LIBVESPUCCI_LIBRARIES} 
        ${Qt5Core_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5Svg_LIBRARES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5PrintSupport_LIBRARIES})

    find_path(EMFENGINE_INCLUDE_DIR
        NAMES EmfEngine.h emfengine.h
        PATHS /usr/include /usr/local/include /usr /usr/local
              ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/EmfEngine
        REQUIRED
    )

    find_library(EMFENGINE_LIBRARY
        NAMES emfengine
        PATHS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/
        ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/EmfEngine
    )
    set(VESPUCCI_INCLUDE_DIRS ${VESPUCCI_INCLUDE_DIRS} ${EMFENGINE_INCLUDE_DIR})
    set(VESPUCCI_LIBRARIES ${VESPUCCI_LIBRARIES} ${EMF_ENGINE_LIBRARY})

    find_path(YAMLCPP_INCLUDE_DIR
        NAMES yaml.h
        PATHS ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/yaml-cpp
              /usr/include /usr/local/include /usr /usr/local/
    )
    find_library(YAMLCPP_LIBRARY
        NAMES libyaml-cpp
        PATHS ${CMAKE_SOURCE_DIR}/../Vespucci_dependencies/yaml-cpp/lib
              /usr/lib64 /usr/lib /usr/local/lib64 /usr/local)
endif()
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${LIBVESPUCCI_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/VespucciLibrary/include)
include_directories(${CMAKE_SOURCE_DIR}/Vespucci)
# On Windows, things end up under Debug/ or Release/.
if (WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else ()
  # If not on Windows, put them under more standard UNIX-like places.  This is
  # necessary, otherwise they would all end up in
  # ${CMAKE_BINARY_DIR}/src/mlpack/methods/... or somewhere else random like
  # that.
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
endif ()

#the above stuff relates to dependencies



#we may need to add more dirs later
set (DIRS
    Vespucci
    VespucciLibrary
    Test
)
set (LIBVESPUCCI_INSTALL_DIRS
    ${CMAKE_CURRENT_SRC_DIR}/include
)
foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# LIBVESPUCCI_SRCS is set in subdirs
add_library(libvespucci ${LIBVESPUCCI_SRCS})
if(BUILD_GUI)
    add_executable(vespucci ${VESPUCCI_SRCS})
    target_link_libraries(vespucci
        ${LIBVESPUCCI_LIBRARIES}
        libvespucci)
    set_target_properties(vespucci
        PROPERTIES
        VERSION 1.1
    )

    install (TARGETS vespucci 
        RUNTIME DESTINATION bin)
endif()
target_link_libraries(libvespucci
    ${LIBVESPUCCI_LIBRARES})
set_target_properties(libvespucci
    PROPERTIES
    VERSION 1.1
    OUTPUT_NAME vespucci
)

#install the files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/VespucciLibrary/include/ 
    DESTINATION include/Vespucci)
install (TARGETS libvespucci
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
