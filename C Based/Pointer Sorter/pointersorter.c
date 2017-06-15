/*
 * pointersorter.c
 *
 *  Created on: Sep 12, 2016
 *      Author: pd304
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


typedef struct{
	int length; // Length of current token
	int size; // Holds the size of memory alloted for the current string - for use with realloc
	char * string; // Holds the partial token
}tokenbuilder;


typedef struct{
	int size; // Memory size of the pointer array - for use with realloc
	char ** stringarr; // Holds pointers to tokens
}tokenstorer;

static int stringcmpr(const void *str1, const void *str2){

	/*
	 * In order to use quicksort as implemented in C stdlib,
	 * need to take the string array down to a string for strcmp
	 */

	return strcmp(* (char * const *) str1, * (char * const *) str2);
}


/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 */

int main(int argc, char **argv){
	/*
	 * Implementing time calculations below
	 * Contains both execution time and CPU time
	 */
	time_t start_t, end_t;
	double total_t;
	time(&start_t);
	// Above segment is to calculate execution time of the program. (Not CPU time)

	clock_t time1, time2, c_total;
	time1 = clock();
	// Above segment is to calculate how many clock cycles the program used.

	/*
	 * Start of error checks
	 */
	if(argv[1] == NULL || strlen(argv[1]) <= 0){
		printf("ERROR: Input string is empty!");
		return 1;
	}

	/*
	 * Initializing a token builder and storer
	 */

	tokenbuilder *builder = (tokenbuilder *)malloc(sizeof(tokenbuilder));
	builder->length = 0;
	builder->size = 5;
	builder->string = (char *) malloc(builder->size);

	tokenstorer *storage = (tokenstorer *)malloc(sizeof(tokenstorer));
	storage->size = 0;

	char tmp; // Holds argv[1][current] as to keep things simpler
	int current = 0; // Holds current location in the input stream
	int fin = 0; // Indicator as to when to finish loops
	/*
	 * Start of reading the input string stream
	 */
	while(fin != 1){

		if(builder->length >= builder->size){
			builder->size += 5;
			builder->string = (char *)realloc(builder->string, builder->size);
		}

		tmp = argv[1][current];

		if(isalpha(tmp)){//test tmp against allowed characters
			builder->string[builder->length] = tmp;
			builder->length++;
		}else if(builder->length != 0){//end of current token, making sure string is not empty

			builder->string[builder->length] = '\0';

			storage->stringarr = (char **)realloc(storage-> stringarr, (storage->size+1)*sizeof(char *));

			storage->stringarr[storage->size] = (char *)malloc(builder->length+1);
			strcpy(storage->stringarr[storage->size], builder->string);
			storage->size++;

			/*
			 * Reset builder for new token
			 */

			builder->length = 0;
			builder->size = 5;

		}
		if(tmp == '\0')
			fin = 1;
		current++;
	}

	if(storage->size != 0 && storage->size != 1){// Sort array
		qsort(storage->stringarr,storage->size,sizeof(char *), stringcmpr);
	}else if(storage->size == 0){
		printf("No string was found!\n");
		return 0;
	}

	// Print out strings

	for(current = 0; current < storage->size; current++){ // Current is being reused, as to not create a new variable. Still a placeholder
		puts(storage->stringarr[current]);
	}

	free(builder);
	free(storage);

	time(&end_t);
	total_t = difftime(end_t, start_t);
	printf("---------------\n");
	printf("Execution time of program is: %f\n", total_t);

	time2 = clock();
	c_total = (double)(time2 - time1) / CLOCKS_PER_SEC;
	printf("CPU time of the program is: %f\n", (double)c_total);
	printf("---------------\n");
	return 0; //Program Terminated
}

