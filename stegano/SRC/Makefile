OUTPUT = jpeg_cpy
CC = gcc
FLAG = -O3

HEADERS = error.h jpeg_manip.h

OBJ = error.o jpeg_manip.o main.o

JPGPATH = jpeg-8/
JPGLIB = $(JPGPATH)libjpeg.o
JPGOBJ = 	jcapistd.o  jchuff.o    jcomapi.o   jdapimin.o	jdcoefct.o	\
jdmainct.o  jdsample.o  jfdctint.o  jmemnobs.o  rdbmp.o		rdrle.o		\
wrgif.o		jcarith.o   jcinit.o    jcparam.o	jdapistd.o  jdcolor.o	\
jdmarker.o  jdtrans.o   jidctflt.o	rdcolmap.o  rdswitch.o  jccoefct.o  \
jcmainct.o  jcprepct.o  jdarith.o   jddctmgr.o  jdmaster.o  jerror.o    \
jidctfst.o  jquant1.o	rdgif.o     rdtarga.o   wrppm.o		jaricom.o   \
jccolor.o   jcmarker.o  jcsample.o  jdatadst.o  jdhuff.o    jdmerge.o   \
jfdctflt.o  jidctint.o  jquant2.o   transupp.o  wrrle.o 	jcapimin.o  \
jcdctmgr.o  jcmaster.o  jctrans.o   jdatasrc.o  jdinput.o   jdpostct.o  \
jfdctfst.o  jmemmgr.o   jutils.o    rdppm.o     wrbmp.o     wrtarga.o

all: $(OUTPUT)

debug: FLAG = -Wall -g 
debug: $(OUTPUT)

$(OUTPUT): $(OBJ) $(JPGLIB)
	$(CC) $(FLAG) $(OBJ) $(JPGLIB) -o $(OUTPUT) -lm

$(JPGLIB):
	cd $(JPGPATH) && make && ld -r $(JPGOBJ) -o libjpeg.o

jpeg_stegano.o: jpeg_manip.c $(HEADERS)
	$(CC) $(FLAG) -c jpeg_manip.c

error.o: error.h error.c
	$(CC) $(FLAG) -c error.c

main.o: main.c
	$(CC) $(FLAG) -c main.c

clean:
	cd $(JPGPATH) && make clean
	rm -rf *~ *.o

uninstall: clean
	rm -f $(OUTPUT)
