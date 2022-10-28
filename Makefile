lib:
	g++ -Wall -c lib.cpp -o lib.o -std=gnu++17 -pthread -lws2_32
	g++ -Wall -shared lib.o -o lib.dll -std=gnu++17 -pthread -lws2_32

clean:
	rm *.o
	rm *.dll