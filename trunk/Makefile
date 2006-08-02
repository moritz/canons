CC=g++
LDFLAGS=
#CFLAGS=-pipe -Wall -ansi -pedantic -Werror -O2  
CFLAGS=-pipe -Wall -ansi -pedantic -g -O1
OBJECTS=
BIN=canons

.SUFFIXES: .cpp 

.cpp.o: 
	$(CC) $(CFLAGS) -c $< 


$(BIN): $(OBJECTS) canons.cpp perm.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN) canons.cpp $(OBJECTS)

perm.h: permutation.pl
	perl permutation.pl

strip: $(BIN)
	strip $(BIN)

clean:
	rm -f *.o $(BIN) core

test: $(BIN)
	./tests.sh
	echo "Tests finished sucessfully"
