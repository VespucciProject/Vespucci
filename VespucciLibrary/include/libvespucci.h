#ifndef LIBVESPUCCI_H
#define LIBVESPUCCI_H
#if defined(_WIN32) || defined(_WIN64)
#  define VESPUCCI_EXPORT __declspec(dllexport)
#else
#  define VESPUCCI_EXPORT 
#endif
#endif // LIBVESPUCCI_H
