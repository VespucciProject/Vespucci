TEMPLATE = subdirs
SUBDIRS = Vespucci Library Test
Vespucci.depends = Library
Test.depends = Vespucci Library
