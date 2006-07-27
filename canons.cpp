/*
 *   canon.cpp: bring sudokus in a canonical and compareable form.
 *   Copyright (C) 2006  Moritz Lenz <moritz at faui2k3 dot org>
 *
 *   This program is free software. You may redistribute and change it under the
 *   terms of the GNU General Public License Version 2 (June 1991) as published
 *   by the Free Software Fundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *   or under the terms of the BSD Artistic License.
 *   Chosse the one you like better.
 */


/* This program reads sudokus from stdin, one each line. It expects 81
 * charakters from '0' to '9' (without any whitespaces), followed by a 
 * newline '\n' character. 
 *
 * It performs all operations that don't change the game's logic, briefly
 * described here: http://www.csse.uwa.edu.au/~gordon/sudokumin.php
 * or more verbose here: http://moritz.faui2k3.org/sudoku#aehnliche
 * (essentially: transposition, permutation of rows and cols within blocks,
 * permutation of blocks and finally permutation the numbers). It prints out
 * one of the results of these transformations which is minimal if read as an
 * ordinary decimal number (most significant digit first, that is).
 *
 * The output is a somewhat 'canonical' form, which is the same for all
 * sudokus that can be generate by the above transformations from a chose
 * sudoku. Therefore you may compare sudokus based on there 'canonical'
 * representation.
 * Note that it only works well with "quasi minimal" sudokus, e.g. with only
 * few numbers set (and many zeros).
 *
 * In December 2005 I downloaded a list of 32930 sudokus from the above url.
 * These sudokus have only 17 non-zero numbers. With this program I confirmed
 * that all of the downloaded sudokus are indeed different.
 *
 */

/* Warning: This program does'n do any input checking, it assumes that input
 * is sane. If it is not, it is easy to provoke buffer overflow and execution
 * of arbitrary code.
 */

#include <iostream>
#include <unistd.h>
#include <assert.h>
#include <stack>

using namespace std;

// perform the followin operations:
// 1. transpose
// 2. permuting rows within blocks
// 3. permuting cols within blocks
// 4. permuting blocks row-wise
// 5. permuting blocks col-wise

void trans1(char f[9][9]);
void trans2(char f[9][9]);
void trans3(char f[9][9]);
void trans4(char f[9][9]);
void trans5(char f[9][9]);
void enque(char f[9][9]);
// all 3er-permutations
const unsigned int perm[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2},
			{1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

stack<char*> lstack;
signed int stack_first_order = -1;
signed int stack_second_order = -1;


void trans1(char f[9][9]){
	// transposition
	// identical
	trans2(f);
	// really transposed:
	char tmp[9][9];
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			tmp[x][y] = f[y][x];
		}
	}
	trans2(tmp);
}

void trans2(char f[9][9]){
	// 2. permuting rows within blocks
	char tmp[9][9];
	for (int a = 0; a < 6; a++){
		for (int b = 0; b < 6; b++){
			for (int c = 0; c < 6; c++){
				for (int y = 0; y< 9; y++){
					tmp[0][y] = f[perm[a][0]][y];	
					tmp[1][y] = f[perm[a][1]][y];	
					tmp[2][y] = f[perm[a][2]][y];	
					tmp[3][y] = f[perm[b][0] + 3][y];	
					tmp[4][y] = f[perm[b][1] + 3][y];	
					tmp[5][y] = f[perm[b][2] + 3][y];	
					tmp[6][y] = f[perm[c][0] + 6][y];	
					tmp[7][y] = f[perm[c][1] + 6][y];	
					tmp[8][y] = f[perm[c][2] + 6][y];	
				} // y
				trans3(tmp);

			} // c
		} // b
	} // a

}

void trans3(char f[9][9]){
	// 3. permuting cols within blocks

	char tmp[9][9];
	for (int a = 0; a < 6; a++){
		for (int b = 0; b < 6; b++){
			for (int c = 0; c < 6; c++){
				for (int x = 0; x< 9; x++){
					tmp[x][0] = f[x][perm[a][0]];	
					tmp[x][1] = f[x][perm[a][1]];	
					tmp[x][2] = f[x][perm[a][2]];	
					tmp[x][3] = f[x][perm[b][0] + 3];	
					tmp[x][4] = f[x][perm[b][1] + 3];	
					tmp[x][5] = f[x][perm[b][2] + 3];	
					tmp[x][6] = f[x][perm[c][0] + 6];	
					tmp[x][7] = f[x][perm[c][1] + 6];	
					tmp[x][8] = f[x][perm[c][2] + 6];	
				} // y
				trans4(tmp);

			} // c
		} // b
	} // a
}

void trans4(char f[9][9]){
// 4. permuting blocks row-wise
	char tmp[9][9];

	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (f[x][y] > 9 ){
				cerr << "Fehler: Werte > 9 nicht erlaubt\n";
			}
		}
	}

	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			tmp[x][y] = 20;
		}
	}
	for (int a = 0; a < 6; a++){
		assert(a >= 0);
		assert(a < 6);
		assert(perm[a][0] != perm[a][1]);
		assert(perm[a][0] != perm[a][2]);
		assert(perm[a][1] != perm[a][2]);
		for (int y = 0; y < 9; y++){
			tmp[y][0] = f[y][3 * perm[a][0] + 0];
			tmp[y][1] = f[y][3 * perm[a][0] + 1];
			tmp[y][2] = f[y][3 * perm[a][0] + 2];
			tmp[y][3] = f[y][3 * perm[a][1] + 0];
			tmp[y][4] = f[y][3 * perm[a][1] + 1];
			tmp[y][5] = f[y][3 * perm[a][1] + 2];
			tmp[y][6] = f[y][3 * perm[a][2] + 0];
			tmp[y][7] = f[y][3 * perm[a][2] + 1];
			tmp[y][8] = f[y][3 * perm[a][2] + 2];

		}

		trans5(tmp);
	}
}

void trans5(char f[9][9]){
// 5. permuting blocks column-wise

	char tmp[9][9];
	for (int a = 0; a < 6; a++){
		for (int x = 0; x < 9; x++){
			tmp[0][x] = f[3 * perm[a][0] + 0][x];
			tmp[1][x] = f[3 * perm[a][0] + 1][x];
			tmp[2][x] = f[3 * perm[a][0] + 2][x];
			tmp[3][x] = f[3 * perm[a][1] + 0][x];
			tmp[4][x] = f[3 * perm[a][1] + 1][x];
			tmp[5][x] = f[3 * perm[a][1] + 2][x];
			tmp[6][x] = f[3 * perm[a][2] + 0][x];
			tmp[7][x] = f[3 * perm[a][2] + 1][x];
			tmp[8][x] = f[3 * perm[a][2] + 2][x];
		}
		enque(tmp);
	}
}

void clear_stack(){
	while (not lstack.empty()){
		delete[] lstack.top();
		lstack.pop();
	}
	assert(lstack.empty());
}

char* linearize(char f[9][9]){
	char* res = new char[81];
	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			res[y*9+x] =  f[x][y];
		}
	}
	return res;
}
			


void enque(char f[9][9]){
	int found = 0;
	int fo = -1;
	int so = -1;
	for (int i = 0; i < 81; i++){
		if (f[i%9][i/9] != 0){
			found++;
			if (found == 1){
				fo = i;
			} else {
				so = i;
				break;
			}
		}
	}
	//cerr << "fo: " << fo << " so: " << so << endl;
	if (fo >= stack_first_order and so >= stack_second_order){
		if (fo > stack_first_order or so > stack_second_order){
			stack_first_order = fo;
			stack_second_order = so;
			clear_stack();
		} 
		lstack.push(linearize(f));
	}
	// else: discard item, it is not the canonical form
	// since f is shallow copy on the stack there is no need to delete[]
	// anything
}

// returns -1 if a < b, 0 if a == b and 1 if a > b
int compare(char* a, char* b ){
	for (int i = 0; i < 81; i++){
		if (a[i] < b[i]){
			return -1;
		}
		if (a[i] > b[i]){
			return 1;
		}
	}
	return 0;
}

// attention: delete[]s its argument ;)!!!
char * permute(char * a){
	short int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int next_map = 1;
	char *res = new char[81];
	for (int i = 0; i < 81; i++){
		short int t = (short int) a[i];
		assert(t < 10);
		if (map[t] == -1){
			map[t] = next_map;
			next_map++;
		}
		res[i] = map[t];
	}
	delete[] a;
	return res;
}


char* canonical(char f[9][9]){
	stack_first_order = -1;
	stack_second_order = -1;
	// start the real work
	trans1(f);
	// done ;)
	// now the stack should contain all items which are worth looking at
	char *res, *tmp;
//	cerr << lstack.size() << endl;
	assert(!lstack.empty());
	res = permute(lstack.top());
	lstack.pop();
	int sc = lstack.size();
	while (!lstack.empty()){
		tmp = permute(lstack.top());
		lstack.pop();
		if (compare(res, tmp) == 1){
			delete[] res;
			res = tmp;
			sc--;
		} else {
			delete[] tmp;
			sc--;
		}
	}
	assert(sc == 0);


	clear_stack();
	trans1(f);
	return res;
}

int main(int argc, char** argv){
	char f[9][9];
	// 81 characters, one new-line character
	char *buffer = new char[85];
	while (!cin.eof()){
		buffer[80] = -1;
		cin.read(buffer, 82);
		if (buffer[80] == -1){
			clear_stack();
			break;
		}
		for (int x = 0; x < 9; x++){
			for (int y = 0; y < 9; y++){
				f[x][y] = buffer[9*x + y] - '0';
				if (f[x][y] < 0){
					cerr << x << " " << y << " '" << (int) buffer[9*x+y] << "'\n";
				}
				assert(f[x][y] >= 0);
				assert(f[x][y] <= 9);

			}
		}
		char *b = canonical(f);
		for (int i = 0; i < 81; i++){
			cout << (int) b[i];
		}
		delete[] b;
		cout << endl;
	}
	delete[] buffer;
	return 0;
}



