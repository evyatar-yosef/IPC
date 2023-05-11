CC = gcc
CFLAGS = -Wall -g

all: stnc

stnc: stnc.o
	gcc -o stnc stnc.o


%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f stnc  *.o *.txt
