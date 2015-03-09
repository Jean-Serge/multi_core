# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=.
SRCDIR=src/

CC	= gcc
CFLAGS	= -Wall -m32  -fdiagnostics-color=auto
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCLUDE = $(ROOTDIR)/include/
INCDIR  = -I$(INCLUDE)
LIBS    = -L$(LIBDIR) -lhardware -lpthread -lreadline
SUFFIX  = .bin

###------------------------------
### Main targets
###------------------------------------------------------------
BINARIES=active-core${SUFFIX} create-disk${SUFFIX} dmps${SUFFIX} dvol${SUFFIX} frmt${SUFFIX} mkvol${SUFFIX} print_mbr${SUFFIX} mknfs${SUFFIX} test_file${SUFFIX}\
rvol${SUFFIX} tfs${SUFFIX} if_status${SUFFIX} if_pfile${SUFFIX} if_cfile${SUFFIX} if_nfile${SUFFIX} if_dfile${SUFFIX} test_convert_blc${SUFFIX}\
shell${SUFFIX}\
prod_con${SUFFIX} sched${SUFFIX}

all: clean disque scheduler shell

disque: create-disk dmps frmt drive volume filesystem ifile dvol print_mbr mkvol mknfs rvol\
if_status if_pfile if_nfile if_dfile if_cfile shell

scheduler: context

test: tconvert_blc tfilesystem test_file


###------------------------------
### Main rules
###------------------------------------------------------------
FSDIR = ${SRCDIR}disque/

create-disk: drive ${FSDIR}create_disk.c
	$(CC) $(CFLAGS) -o create-disk${SUFFIX} drive.o hw.o context.o ${FSDIR}create_disk.c ${LIBS} ${INCDIR}

dmps: drive volume ${FSDIR}dmps.c
	$(CC) $(CFLAGS) -o dmps${SUFFIX} drive.o hw.o context.o ${FSDIR}dmps.c ${LIBS} ${INCDIR}

frmt: drive volume context ${FSDIR}frmt.c
	$(CC) $(CFLAGS) -o frmt${SUFFIX} context.o hw.o drive.o volume.o tools.o ${FSDIR}frmt.c ${LIBS} ${INCDIR}

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

dvol: volume drive ${FSDIR}dvol.c
	$(CC) $(CFLAGS) -o dvol${SUFFIX} drive.o context.o hw.o volume.o tools.o ${FSDIR}dvol.c ${LIBS}  ${INCDIR}

mkvol: volume drive ${FSDIR}mkvol.c
	$(CC) $(CFLAGS) -o mkvol${SUFFIX} drive.o context.o hw.o volume.o tools.o ${FSDIR}mkvol.c ${LIBS}  ${INCDIR}

mknfs: volume drive filesystem tools ifile ${FSDIR}mknfs.c
	$(CC) $(CFLAGS) -o mknfs${SUFFIX} drive.o context.o hw.o volume.o filesystem.o tools.o ifile.o ${FSDIR}mknfs.c ${LIBS} ${INCDIR}

print_mbr: volume drive ${FSDIR}print_mbr.c
	$(CC) $(CFLAGS) -o print_mbr${SUFFIX} drive.o hw.o context.o volume.o tools.o ${FSDIR}print_mbr.c ${LIBS} ${INCDIR}

rvol: volume drive ${FSDIR}rvol.c
	$(CC) $(CFLAGS) -o rvol${SUFFIX} drive.o volume.o context.o hw.o tools.o ${FSDIR}rvol.c ${LIBS} ${INCDIR}

if_status: filesystem ${FSDIR}if_status.c
	$(CC) $(CFLAGS) -o if_status${SUFFIX} drive.o volume.o hw.o context.o filesystem.o tools.o ${FSDIR}if_status.c ${LIBS} ${INCDIR}

if_pfile: ifile ${FSDIR}if_pfile.c
	$(CC) $(CFLAGS) -o if_pfile${SUFFIX} drive.o volume.o hw.o context.o filesystem.o ifile.o tools.o ${FSDIR}if_pfile.c ${LIBS} ${INCDIR}

if_nfile: ifile ${FSDIR}if_nfile.c
	$(CC) $(CFLAGS) -o if_nfile${SUFFIX} drive.o volume.o hw.o context.o filesystem.o ifile.o tools.o ${FSDIR}if_nfile.c ${LIBS} ${INCDIR}

if_dfile: ifile ${FSDIR}if_dfile.c
	$(CC) $(CFLAGS) -o if_dfile${SUFFIX} drive.o volume.o hw.o context.o filesystem.o ifile.o tools.o ${FSDIR}if_dfile.c ${LIBS} ${INCDIR}

if_cfile: ifile ${FSDIR}if_cfile.c
	$(CC) $(CFLAGS) -o if_cfile${SUFFIX} drive.o volume.o hw.o context.o filesystem.o ifile.o tools.o ${FSDIR}if_cfile.c ${LIBS} ${INCDIR}


###------------------------------
### Shell rules
###------------------------------------------------------------

CMDDIR = ${SRCDIR}shell/

shell: cmd dir ifile volume filesystem tools scheduler ${CMDDIR}shell.c
	$(CC) $(CFLAGS) -o shell${SUFFIX} cmd.o file.o ifile.o drive.o filesystem.o tools.o volume.o dir.o my_ls.o my_cd.o my_pwd.o my_compute.o my_mkdir.o my_dumps.o context.o hw.o ${CMDDIR}shell.c ${LIBS} ${INCDIR}

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

# prod_con: context
# 	${CC} ${CFLAGS} -o prod_con${SUFFIX} context.o hw.o ${SCHEDDIR}prod_con.c ${INCDIR}

###------------------------------
### Testing rules
###------------------------------------------------------------

TESTDIR = ${SRCDIR}test/

tconvert_blc: volume drive ${TESTDIR}test_convert_blc.c
	$(CC) $(CFLAGS) -o test_convert_blc${SUFFIX} drive.o volume.o tools.o ${TESTDIR}test_convert_blc.c ${LIBS} ${INCDIR}

tfilesystem: filesystem volume drive ${TESTDIR}test_filesystem.c
	$(CC) $(CFLAGS) -o tfs${SUFFIX} drive.o volume.o  filesystem.o tools.o ${TESTDIR}test_filesystem.c ${LIBS} ${INCDIR}

test_file: dir ifile file tools ${TESTDIR}test_file.c
	$(CC) $(CFLAGS) -o test_file${SUFFIX} drive.o volume.o filesystem.o ifile.o file.o dir.o tools.o ${TESTDIR}test_file.c ${LIBS} ${INCDIR}



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
