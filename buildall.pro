TEMPLATE = subdirs
SUBDIRS = VespucciLibrary Vespucci Test
QT       += core gui
QT       += widgets printsupport
QT       += svg
CONFIG   += shared debug_and_release c++11
Vespucci.depends = VespucciLibrary
Test.depends = Vespucci VespucciLibrary
