# Makefile for skyfly GLUT demo

OBJS	= gm_main.o skyfly.o fly.o database.o perfdraw.o image.o random.o

ifeq ($(shell uname),Darwin)
LIBS	= -framework glut -framework OpenGL
else
LIBS	= -lglut -lGL -lGLU -lm
endif

EXES	= skyfly

# force 32-bit compilation to avoid int/long mismatch
CFLAGS	+= -m32

LDFLAGS += -m32

skyfly:	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o skyfly $(OBJS) $(LIBS)

.PHONY:	clean
clean: 
	rm -f $(OBJS) $(EXES)
