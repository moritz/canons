CC=gcc
LDFLAGS=
#CFLAGS=-pipe -Wall -ansi -pedantic -Werror -O2  
CFLAGS=-pipe -Wall  -ansi -pedantic -g -O1
OBJECTS=
BIN=canons

.SUFFIXES: .c 

.c.o: 
	$(CC) -std=c99 $(CFLAGS) -c $< 


$(BIN): $(OBJECTS) canons.c perm.h
	$(CC) -std=c99 $(CFLAGS) $(LDFLAGS) -o $(BIN) canons.c $(OBJECTS)

perm.h: permutation.pl
	perl permutation.pl

strip: $(BIN)
	strip $(BIN)

clean:
	rm -f *.o $(BIN) core

test: $(BIN)
	./tests.sh
	echo "Tests finished sucessfully"

html-doc: canons.1.txt
	asciidoc -d manpage canons.1.txt
