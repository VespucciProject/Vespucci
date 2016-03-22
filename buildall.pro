TEMPLATE = subdirs
SUBDIRS = Vespucci Library/VespucciLibrary Test/Test
Vespucci.depends = Library/VespucciLibrary
Test/Test.depends = Vespucci
