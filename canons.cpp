#include <iostream>
#include "perm.h"
using namespace std;

typedef short int s_type;

void perm_copy(s_type source[9][9], s_type* dest, int i, int j){
	// copy source to dest.
	// Applay transformations (i, j) before copying
	// Generate a minimal permutation on the fly.
	short int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int next_map = 1;
	for(int k = 0; k < 81; k++){
		s_type a = source[perm[i][k % 9]][perm[j][k / 9]];
		if (map[a] == -1){
			map[a] = next_map;
			next_map++;
		}
		dest[k] = map[a];
	}
}

void compare_and_update (s_type* least, s_type tmp[9][9], int i, int j){
	// apply transformation (i, j) to `tmp';
	// then compare the minimal permutation of `tmp' to `least'
	// if the minimal permuatation is smaller then `least', update `least'
	int count = 0;
	short int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	short int next_map = 1;
	for (int y = 0; y <9; y++){
		for (int x = 0; x < 9; x++){
			s_type a = tmp[perm[i][x]][perm[j][y]];
			if (map[a] == -1){
				map[a] = next_map;
				next_map++;
			}
			if (map[a] < least[count]){
				perm_copy(tmp, least, i, j);
				return;
			} else if (map[a] > least[count]){
				return;
			}
			count ++;
		}
	}

}

s_type* transform(s_type* s){
	// transform `s' into its canonincal form
	s_type* least = new s_type[81];
	for (int i = 0; i < 81; i++){
		least[i] = 9;
	}
	// tmp is the 2d representation of the linear `s'
	// tmp2 is the transposed version of tmp
	s_type tmp[9][9];
	s_type tmp2[9][9];
	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			tmp[x][y] = s[9*y + x];
			tmp2[y][x] = s[9*y + x];
		}
	}
	for (int i = 0; i < 1296; i++){
		for(int j = 0; j < 1296; j++){
			compare_and_update(least, tmp, i, j);
			compare_and_update(least, tmp2, i, j);
		}
	}

	return least;

}

int main(int argc, char** argv){


	char buffer[85];
	s_type* s = new s_type[81];
	
	// read from stdin.
	// chose buffer a little larger then 81; maybe some broken OS use two
	// characters as line separators

	while(cin.getline(buffer, 83)){
		for(int i = 0; i < 81; i++){
			if (buffer[i] >= '0' && buffer[i] <= '9'){
				s[i] = (s_type) (buffer[i] - '0');
			} else {
				s[i] = 0;
			}
		}
		s_type* out = transform(s);
		for (int i = 0; i < 81; i++){
			cout << (int) out[i];
		}
		cout << std::endl;
		delete[] out;

	}
	delete[] s;
	return 0;

}
