
cc = g++
h = headerFiles/
cpp = cppFiles/
lib = lib/

all: dvd
	./a.out

clean:
	rm *.o
	rm a.out


dvd: dvdbounce.o player.o
	$(cc) dvdbounce.o -I$(h) $(lib)libraylib.a player.o -o waitingScreen

dvdbounce.o: dvdbounce.cpp 
	$(cc) dvdbounce.cpp -c -I$(h) #$(lib)libraylib.a

player.o: $(cpp)player.cpp $(h)raylib.h $(h)player.h
	$(cc) -c $(cpp)player.cpp -I$(h) -o player.o


















