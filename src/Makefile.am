AM_CFLAGS = 
bin_PROGRAMS = bacteria
bacteria_SOURCES = async_serv.c main.c signal_handler.c
bacteria_DEPENDENCIES = $(top_builddir)/libbacteria/libbacteria.a
bacteria_LDADD = $(top_builddir)/libbacteria/libbacteria.a

AM_CPPFLAGS = -I$(top_srcdir)/libbacteria/
target_CPPFLAGS = -I$(top_srcdir)/libbacteria/




