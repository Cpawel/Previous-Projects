/*
 * formula.c
 *
 *  Created on: Mar 20, 2016
 *      Author: pawel
 */

#include "formula.h"

int main(int argc, char **argv){

	struct timeval begin, finish; // variables to output run time
	gettimeofday(&begin, NULL);

	char * tmp = "-h";
	if((strcmp(tmp, argv[1])) == 0){
		printf("Usage: formula <positive integer> \n");
		return 0;
	}

	int result = 0; // Self- explanatory variables
	int index = 1;
	int num = atoi(argv[1]);

	if(num < 0){ // Start of output to console
		printf("ERROR: Input is negative or NaN\n");
		return 1;
	}else if(num == 0){
		printf("(1 + x)^0 = 1\n");
		return 0;
	}else if(num == 1){
		printf("(1 + x)^1 = 1 + x\n");
		return 0;
	}else if(num > 0){
		printf("(1 + x)^%d = 1", num);
		while(index <= num){
			result = nCr(num, index);
			printf(" + %d*x^%d", result, index);
			index++;
		}
		printf("\n");
	}

	gettimeofday(&finish, NULL);
	printf("%ld microseconds \n", ((finish.tv_sec * 1000000 + finish.tv_usec)- (begin.tv_sec * 1000000 + begin.tv_usec)));

	return 0;

}

