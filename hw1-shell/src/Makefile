CC=gcc
CFLAGS=-g -Wall
OBJS=SiSH.o parser.o list.o execute.o keyboard.o console.o base.o error.o
TARGET=SiSH

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean :
	rm *.o SiSH

SiSH.o: color.h execute.h base.h SiSH.h SiSH.c
parser.o: parser.h base.h list.h parser.c
list.o: list.h list.c
execute.o : execute.h base.h parser.h execute.c
keyboard.o: keyboard.h keyboard.c
console.o: console.h console.c
base.o: base.h base.c
error.o: error.h error.c