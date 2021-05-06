CC = gcc
CFLAGS = -Werror -Wall -ansi -pedantic -g -pthread
OBJ = SRTF.o PP.o file.o main.o setup.o sort.o result.o
EXEC = main

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -pthread -o $(EXEC)

main.o : main.c header.h file.h Task.h sort.h priority.h srtf.h
	$(CC) $(CFLAGS) -c main.c

SRTF.o : SRTF.c header.h srtf.h
	$(CC) $(CFLAGS) -c SRTF.c

PP.o : PP.c header.h priority.h setup.h
	$(CC) $(CFLAGS) -c PP.c

file.o : file.c file.h
	$(CC) $(CFLAGS) -c file.c

setup.o : setup.c setup.h
	$(CC) $(CFLAGS) -c setup.c

sort.o : sort.c sort.h Task.h
	$(CC) $(CFLAGS) -c sort.c

result.o : result.c Task.h
	$(CC) $(CFLAGS) -c result.c

clean: 
	rm -f $(OBJ) $(EXEC)
