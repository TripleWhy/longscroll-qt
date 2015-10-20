MKSPEC = $$basename(QMAKESPEC)
win32-g++: MKSPEC = $$replace(MKSPEC, g\+\+, gcc)	#Bug workaround. Does not recognize that a path containing '+' has to be quoted on windows.
message($$MKSPEC)
LONGSCROLLQT_DST_DIR = $$PWD/../dist/$$MKSPEC/
LONGSCROLLQT_DST_DIR = $$absolute_path($$LONGSCROLLQT_DST_DIR)
