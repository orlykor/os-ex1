CC=g++
RANLIB=ranlib

LIBSRC=osm.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

INCS=-I.
CPPFLAGS = -Wall -g $(INCS) -O0
LOADLIBES = -L./ 

OSMLIB = libosm.a
TARGETS = $(OSMLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex1.tar
TARSRCS=$(LIBSRC) Makefile README

all: $(TARGETS) 
	$(CC) $(CPPFLAGS) -c -o osm.o osm.cpp

$(TARGETS): $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@


clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) *~ *core


depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)


tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
