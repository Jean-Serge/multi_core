# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=.
SRCDIR=src/

CC	= gcc
CFLAGS	= -Wall -m32
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCLUDE = $(ROOTDIR)/include/
INCDIR  = -I$(INCLUDE)
LIBS    = -L$(LIBDIR) -lhardware -lpthread
SUFFIX  = .bin

###------------------------------
### Main targets
###------------------------------------------------------------
BINARIES=active-core${SUFFIX}

all: clean $(BINARIES)


###------------------------------
### Main rules
###------------------------------------------------------------
active-core: ${SRCDIR}active-core.c
	${CC} ${CFLAGS} -o active-core${SUFFIX} ${SRCDIR}active-core.c ${LIBS} ${INCDIR}

active-core-int: ${SRCDIR}active-core.c
	${CC} ${CFLAGS} -o active-core-int${SUFFIX} ${SRCDIR}active-core-int.c ${LIBS} ${INCDIR}


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
realclean: clean
	$(RM) vdiskA.bin vdiskB.bin
