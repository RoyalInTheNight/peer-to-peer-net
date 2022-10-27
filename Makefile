lib:
	g++ -Wall -c lib.cpp -o lib.o -std=gnu++17 -pthread
	ar rc libnet.a lib.o

clean:
	rm *.o
	rm *.a