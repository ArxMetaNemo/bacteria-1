#SUBDIRS=lua
AUTOMAKE_OPTIONS = subdir-objects
lib_LIBRARIES = libbacteria.a
libbacteria_a_SOURCES = cryptocoins.c  cryptocoins.h  json_rpc.c  macros.h\
		       	lua/afirst.c  lua/libcrypto.c  lua/libjson.c
libbacteria_a_CFLAGS = -g -I /usr/include/luajit-2.1/ -I$(topdir)/libbacteria/

