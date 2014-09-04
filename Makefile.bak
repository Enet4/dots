CC = g++

CFLAGS = -Wall -I "./src" -std=c++11
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -s -O2

LFLAGS = -lGL -lGLU -lglut

OBJS  = src/Configurator.o src/Dot.o src/DotConf.o src/GaussFunc.o
OBJS += src/RandGenerator.o src/Simulator.o src/main.o

all: release

release: CFLAGS += $(CFLAGS_RELEASE)
release: dots

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: dots

dots:	$(OBJS)
		$(CC) $(CFLAGS) $(LFLAGS) -o bin/$@ $^

.cpp.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f src/*.o bin/dots
