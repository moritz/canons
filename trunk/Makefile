CC=g++
LDFLAGS=-lm
#CFLAGS=-pipe -Wall -ansi -pedantic -Werror -O2  
CFLAGS=-pipe -Wall -ansi -pedantic -g -O1
OBJECTS=
BIN=canons

.SUFFIXES: .cpp 

.cpp.o: 
	$(CC) $(CFLAGS) -c $< 


$(BIN): $(OBJECTS) canons.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) canons.cpp $(OBJECTS)


clean:
	rm -f *.o $(BIN) core

test: $(BIN)
	./tests.sh
	echo "Tests finished sucessfully"
