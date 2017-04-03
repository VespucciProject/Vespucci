#ifndef LIBVESPUCCI_H
#define LIBVESPUCCI_H
  #if VESPUCCI_LIBRARY 
    #define VESPUCCI_EXPORT __attribute__((__visibility__("default")))
  #elif VESPUCCI_LIBRARY && defined _MSC_VER
    #define VESPUCCI_EXPORT __declspec(dllexport)
  #elif defined _MSC_VER
    #define VESPUCCI_EXPORT __declspec(dllimport)
  #else
    #define VESPUCCI_EXPORT 
  #endif // VESPUCCI_LIBRARY
#endif // LIBESPUCCI_H
