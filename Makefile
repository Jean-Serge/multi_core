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
LIBS    = -L$(LIBDIR) -lhardware -lpthread -lreadline
SUFFIX  = .bin

###------------------------------
### Main targets
###------------------------------------------------------------
BINARIES=$(addsuffix ${SUFFIX},\
active-core \
create-disk dmps dvol frmt mkvol print_mbr mknfs rvol tfs\
test_file if_status if_pfile if_cfile if_nfile if_dfile test_convert_blc \
shell \
prod_con sched)


all: clean initialisation disque scheduler shell

disque: create-disk dmps frmt drive volume filesystem ifile dvol print_mbr mkvol mknfs rvol\
if_status if_pfile if_nfile if_dfile if_cfile shell

scheduler: context

test: tconvert_blc tfilesystem test_file

###------------------------------
### Initialisation
###------------------------------------------------------------
initialisation: ${SRCDIR}initialisation.c ${INCLUDE}initialisation.h scheduler
	$(CC) $(CFLAGS) -o initialisation.o -c ${SRCDIR}/initialisation.c ${LIBS} ${INCDIR}

###------------------------------
### Main rules
###------------------------------------------------------------
FSDIR = ${SRCDIR}disque/

# Construction de fichiers .o
drive: context ${FSDIR}drive.c ${INCLUDE}drive.h
	$(CC) $(CFLAGS) -o drive.o -c ${FSDIR}drive.c ${INCDIR}

volume: tools drive ${FSDIR}volume.c ${INCLUDE}volume.h
	$(CC) $(CFLAGS) -o volume.o -c ${FSDIR}volume.c ${INCDIR}

filesystem: volume ${FSDIR}filesystem.c ${INCLUDE}filesystem.h
	$(CC) $(CFLAGS) -o filesystem.o -c ${FSDIR}filesystem.c ${INCDIR}

tools: ${FSDIR}tools.c ${INCLUDE}tools.h
	$(CC) $(CFLAGS) -o tools.o -c ${FSDIR}tools.c ${LIBS} ${INCDIR}

ifile: filesystem ${FSDIR}ifile.c ${INCLUDE}ifile.h
	$(CC) $(CFLAGS) -o ifile.o -c ${FSDIR}ifile.c ${INCDIR}

file: filesystem ${FSDIR}file.c ${INCLUDE}file.h
	$(CC) $(CFLAGS) -o file.o -c ${FSDIR}file.c ${INCDIR}

dir:  ${FSDIR}dir.c ${INCLUDE}dir.h
	$(CC) $(CFLAGS) -o dir.o -c ${FSDIR}dir.c ${INCDIR}

# Executables
OFILE = drive.o context.o hw.o initialisation.o

create-disk: drive ${FSDIR}create_disk.c
	$(CC) $(CFLAGS) -o create-disk${SUFFIX} ${OFILE} ${FSDIR}create_disk.c ${LIBS} ${INCDIR}

dmps: drive volume ${FSDIR}dmps.c
	$(CC) $(CFLAGS) -o dmps${SUFFIX} ${OFILE} ${FSDIR}dmps.c ${LIBS} ${INCDIR}


OFILE_2 = ${OFILE} volume.o tools.o
frmt: drive volume context ${FSDIR}frmt.c
	$(CC) $(CFLAGS) -o frmt${SUFFIX} ${OFILE_2} ${FSDIR}frmt.c ${LIBS} ${INCDIR}

dvol: volume drive ${FSDIR}dvol.c
	$(CC) $(CFLAGS) -o dvol${SUFFIX} ${OFILE_2} ${FSDIR}dvol.c ${LIBS}  ${INCDIR}

mkvol: volume drive ${FSDIR}mkvol.c
	$(CC) $(CFLAGS) -o mkvol${SUFFIX} ${OFILE_2} ${FSDIR}mkvol.c ${LIBS}  ${INCDIR}

print_mbr: volume drive ${FSDIR}print_mbr.c
	$(CC) $(CFLAGS) -o print_mbr${SUFFIX} ${OFILE_2} ${FSDIR}print_mbr.c ${LIBS} ${INCDIR}

rvol: volume drive ${FSDIR}rvol.c
	$(CC) $(CFLAGS) -o rvol${SUFFIX} ${OFILE_2} ${FSDIR}rvol.c ${LIBS} ${INCDIR}

OFILE_3 = ${OFILE_2} filesystem.o
if_status: filesystem ${FSDIR}if_status.c
	$(CC) $(CFLAGS) -o if_status${SUFFIX} ${OFILE_3} ${FSDIR}if_status.c ${LIBS} ${INCDIR}


OFILE_4 = ${OFILE_3} ifile.o
mknfs: volume drive filesystem tools ifile ${FSDIR}mknfs.c
	$(CC) $(CFLAGS) -o mknfs${SUFFIX} ${OFILE_4} ${FSDIR}mknfs.c ${LIBS} ${INCDIR}

if_pfile: ifile ${FSDIR}if_pfile.c
	$(CC) $(CFLAGS) -o if_pfile${SUFFIX} ${OFILE_4} ${FSDIR}if_pfile.c ${LIBS} ${INCDIR}

if_nfile: ifile ${FSDIR}if_nfile.c
	$(CC) $(CFLAGS) -o if_nfile${SUFFIX} ${OFILE_4} ${FSDIR}if_nfile.c ${LIBS} ${INCDIR}

if_dfile: ifile ${FSDIR}if_dfile.c
	$(CC) $(CFLAGS) -o if_dfile${SUFFIX} ${OFILE_4} ${FSDIR}if_dfile.c ${LIBS} ${INCDIR}

if_cfile: ifile ${FSDIR}if_cfile.c
	$(CC) $(CFLAGS) -o if_cfile${SUFFIX} ${OFILE_4} ${FSDIR}if_cfile.c ${LIBS} ${INCDIR}


###------------------------------
### Shell rules
###------------------------------------------------------------

CMDDIR = ${SRCDIR}shell/

OFILE_CMD = ${addsuffix .o, \
my_cd my_compute my_ls my_mkdir my_dumps my_pwd cmd}

shell: cmd dir ifile volume filesystem tools scheduler ${CMDDIR}shell.c
	$(CC) $(CFLAGS) -o shell${SUFFIX} ${OFILE_4} file.o dir.o ${OFILE_CMD} ${CMDDIR}shell.c ${LIBS} ${INCDIR}

cmd: my_cd my_ls my_pwd my_compute my_mkdir my_dumps ${CMDDIR}cmd.c ${INCLUDE}cmd.h
	$(CC) $(CFLAGS) -o cmd.o -c ${CMDDIR}cmd.c ${INCDIR}

my_ls: ${CMDDIR}my_ls.c ${INCLUDE}my_ls.h
	$(CC) $(CFLAGS) -o my_ls.o -c ${CMDDIR}my_ls.c ${INCDIR}

my_dumps: ${CMDDIR}my_dumps.c
	$(CC) $(CFLAGS) -o my_dumps.o -c ${CMDDIR}my_dumps.c ${INCDIR}

my_cd: file ${CMDDIR}my_cd.c ${INCLUDE}my_cd.h
	$(CC) $(CFLAGS) -o my_cd.o -c ${CMDDIR}my_cd.c ${INCDIR}

my_pwd: ${CMDDIR}my_pwd.c ${INCLUDE}my_pwd.h
	$(CC) $(CFLAGS) -o my_pwd.o -c ${CMDDIR}my_pwd.c ${INCDIR}

my_mkdir: ${CMDDIR}my_mkdir.c ${INCLUDE}my_mkdir.h
	$(CC) $(CFLAGS) -o my_mkdir.o -c ${CMDDIR}my_mkdir.c ${INCDIR}

my_compute: ${CMDDIR}my_compute.c ${INCLUDE}my_compute.h
	$(CC) $(CFLAGS) -o my_compute.o -c ${CMDDIR}my_compute.c ${INCDIR}

###------------------------------
### Scheduler rules
###------------------------------------------------------------

SCHEDDIR = ${SRCDIR}ordonnancement/

context: hw ${SCHEDDIR}context.c ${INCLUDE}context.h
	${CC} ${CFLAGS} -o context.o -c ${SCHEDDIR}context.c ${INCDIR}

hw: ${INCLUDE}hw.h ${SCHEDDIR}hw.c
	${CC} ${CFLAGS} -o hw.o -c ${SCHEDDIR}hw.c ${INCDIR}

tp5: context hw
	${CC} ${CFLAGS} -o sched${SUFFIX} context.o hw.o ${SCHEDDIR}tp5.c ${INCDIR}

###------------------------------
### Testing rules
###------------------------------------------------------------

TESTDIR = ${SRCDIR}test/

tconvert_blc: volume drive ${TESTDIR}test_convert_blc.c
	$(CC) $(CFLAGS) -o test_convert_blc${SUFFIX} initialisation.o drive.o volume.o tools.o ${TESTDIR}test_convert_blc.c ${LIBS} ${INCDIR}

tfilesystem: filesystem volume drive ${TESTDIR}test_filesystem.c
	$(CC) $(CFLAGS) -o tfs${SUFFIX} drive.o volume.o  initialisation.o filesystem.o tools.o ${TESTDIR}test_filesystem.c ${LIBS} ${INCDIR}

test_file: dir ifile file tools ${TESTDIR}test_file.c
	$(CC) $(CFLAGS) -o test_file${SUFFIX} drive.o volume.o initialisation.o filesystem.o ifile.o file.o dir.o tools.o ${TESTDIR}test_file.c ${LIBS} ${INCDIR}



###------------------------------
### Test core
###------------------------------------------------------------
COREDIR = ${SRCDIR}core_test/
active-core: ${COREDIR}active-core.c
	${CC} ${CFLAGS} -o active-core${SUFFIX} ${COREDIR}active-core.c ${LIBS} ${INCDIR}

active-core-int: ${COREDIR}active-core.c
	${CC} ${CFLAGS} -o active-core-int${SUFFIX} ${COREDIR}active-core-int.c ${LIBS} ${INCDIR}


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
realclean: clean
	$(RM) vdiskA.bin vdiskB.bin
