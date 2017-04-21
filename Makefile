#make file for assignment 5

CC=gcc
CFLAGS=-c

p1: TwoPipesTwoChildren.o
	$(CC) -o p1 TwoPipesTwoChildren.o
p2: TwoPipesThreeChildren.o
	$(CC) -o p2 TwoPipesThreeChildren.o
dynpipe: DynPipe.o
	$(CC) -o dynpipe DynPipe.o
TwoPipesTwoChildren.o: TwoPipesTwoChildren.cpp
	$(CC) $(CFLAGS) TwoPipesTwoChildren.cpp
TwoPipesThreeChildren.o: TwoPipesThreeChildren.cpp
	$(CC) $(CFLAGS) TwoPipesThreeChildren.cpp
DynPipe.o: DynPipe.cpp
	$(CC) $(CFLAGS) DynPipe.cpp
clean:
	rm -rf p1 p2 dynpipe
		
