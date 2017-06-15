/*
 * mystery.c
 *
 *  Created on: Mar 20, 2016
 *      Author: pawel
 */

#include "mystery.h"

int num[200];

int add(int x, int y){
	return x + y;
}

int dothething(int x){

	if(num[x] == -1){
		if(x == 0)
			num[x] = 0;
		else if(x == 1)
			num[x] = 1;
		else
			num[x] = add(dothething(x - 2), dothething(x - 1));
	}

	return num[x];
}

int main(int argc, char** argv){
	int x = 0;
	int y = atoi(argv[1]);
	while (x < 200){
		num[x] = -1;
		x++;
	}
	printf("Value:  %d\n", dothething(y));
	return 0;
}

