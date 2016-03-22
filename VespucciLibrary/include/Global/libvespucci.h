#ifndef LIBVESPUCCI_H
#define LIBVESPUCCI_H
#include <QtCore/QtGlobal>
#ifdef VESPUCCI_LIBRARY
#  define VESPUCCI_EXPORT Q_DECL_EXPORT
#else
#  define VESPUCCI_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBVESPUCCI_H

