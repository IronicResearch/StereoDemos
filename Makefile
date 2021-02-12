# Makefile for GLUT demos

OBJS	= slut.o

LIBS	= -lglut -lGL

EXES	= gears morph steam logo test

.PHONY:	all
all:	$(EXES)

gears:	$(OBJS) gears.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o gears gears.o $(OBJS) $(LIBS)

morph:	$(OBJS) morph2.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o morph morph2.o $(OBJS) $(LIBS)

steam:	$(OBJS) steam.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o steam steam.o $(OBJS) $(LIBS)

logo:	$(OBJS) def_logo.o opengl_logo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o logo def_logo.o opengl_logo.o $(OBJS) $(LIBS)

test:	$(OBJS) test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o test test.o $(OBJS) $(LIBS)

.PHONY:	clean
clean: 
	rm -f $(OBJS) $(EXES)
