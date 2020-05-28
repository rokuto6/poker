

All: Table.o Hands.o Players.o Main.o
	g++ -Wall -o poker Table.o Hands.o Players.o Main.o

Table.o: library/table.cpp library/table.h
	g++ -Wall -c library/table.cpp

Hands.o: library/hands.cpp library/hands.h
	g++ -Wall -c library/hands.cpp

Players.o: library/players.cpp library/players.h
	g++ -Wall -c library/players.cpp
	
Main.o: main.cpp
	g++ -Wall -c main.cpp

clean:
	rm -f *.o poker