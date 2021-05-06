CC = gcc
CFLAGS = -Werror -Wall -ansi -pedantic -g
OBJ = SRTF.o file.o main.o setup.o sort.o
EXEC = main

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o : main.c header.h file.h Task.h sort.h priority.h srtf.h
	$(CC) $(CFLAGs) -c main.c

SRTF.o : SRTF.c header.h srtf.h
	$(CC) $(CFLAGS) -c SRTF.c

file.o : file.c file.h
	$(CC) $(CFLAGS) -c file.c

setup.o : setup.c setup.h
	$(CC) $(CFLAGS) -c setup.c

sort.o : sort.c sort.h Task.h
	$(CC) $(CFLAGS) -c sort.c

clean: 
	rm -f $(OBJ) $(EXEC)
