OBJS	= main.o plateau.o interface.o historique.o
SOURCE	= main.c plateau.c interface.c historique.c
HEADER	= plateau.h interface.h historique.h
OUT	= morpion
CC	 = gcc
FLAGS	 = -g -c -Wall -Wextra
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

plateau.o: plateau.c
	$(CC) $(FLAGS) plateau.c 

interface.o: interface.c
	$(CC) $(FLAGS) interface.c 

historique.o: historique.c
	$(CC) $(FLAGS) historique.c 


clean:
	rm -f $(OBJS) $(OUT)
