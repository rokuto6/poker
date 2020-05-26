

All: Playing.o Hands.o Computer.o Main.o
	g++ -Wall -o poker Playing.o Hands.o Computer.o Main.o

Playing.o: library/playing.cpp library/playing.h
	g++ -Wall -c library/playing.cpp

Hands.o: library/hands.cpp library/hands.h
	g++ -Wall -c library/hands.cpp

Computer.o: library/computer.cpp library/computer.h
	g++ -Wall -c library/computer.cpp
	
Main.o: main.cpp
	g++ -Wall -c main.cpp

clean:
	rm -f *.o poker