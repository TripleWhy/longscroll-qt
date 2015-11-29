#Copyright (C) 2015 Yannick Mueller
#
#This file is part of longscroll-qt.
#
#longscroll-qt is free software; you can redistribute it and/or modify it under
#the terms of the GNU Affero General Public License as published by the Free
#Software Foundation; either version 3, or (at your option) any later
#version.
#
#longscroll-qt is distributed in the hope that it will be useful, but WITHOUT ANY
#WARRANTY; without even the implied warranty of MERCHANTABILITY or
#FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
#for more details.
#
#You should have received a copy of the GNU Affero General Public License
#along with longscroll-qt.  If not see <http://www.gnu.org/licenses/>.

MKSPEC = $$basename(QMAKESPEC)
win32-g++: MKSPEC = $$replace(MKSPEC, g\+\+, gcc)	#Bug workaround. Does not recognize that a path containing '+' has to be quoted on windows.
LONGSCROLLQT_DST_DIR = $$PWD/../dist/$$MKSPEC/
LONGSCROLLQT_DST_DIR = $$absolute_path($$LONGSCROLLQT_DST_DIR)
