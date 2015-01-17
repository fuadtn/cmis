client: 
	g++ -c vigeneer.cpp
	g++ -c aes.cpp
	g++ -c stg.cpp
	g++ -c client.cpp
	g++ -o client client.o vigeneer.o aes.o stg.o
						
server:
	g++ -c vigeneer.cpp
	g++ -c aes.cpp
	g++ -c stg.cpp
	g++ -c server.cpp
	g++ -o server server.o vigeneer.o aes.o stg.o

clean:
	rm -f client
	rm -f server
	rm -f client.o
	rm -f server.o
	rm -f vigeneer.o
	rm -f aes.o
	rm -f stg.o
