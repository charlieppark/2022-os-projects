CC=gcc
CFLAGS=-g -Wall
OBJS=main.o prod_cons.o
TARGET=main

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

clean :
	rm *.o main

clear :
	rm *.o main

main.o: prod_cons.h main.c
prod_cons.o: prod_cons.h prod_cons.c