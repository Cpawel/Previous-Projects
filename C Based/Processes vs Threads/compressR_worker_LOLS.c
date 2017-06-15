/*
 * compressR_worker_LOLS.c
 *
 *  Created on: Nov 15, 2016
 *      Author: pd304
 */

/*
 * Source code for CHILD of multiprocessing LOL compression
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define _FILE_OFFSET_BITS 64 // Allows for large file sources

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char ** argv){

	if(argc == 1){
		fprintf(stderr, "Usage: %s [process number] [starting location] [# of bytes to read] [# of pieces file was split into] [modified filename] [original filename]\n",argv[0]);
		exit(EXIT_FAILURE);
	}else if(argc != 7)
		handle_error_en(22, "Invalid number of arguments");

	/*
	 * Initialization
	 */

	off_t length, start;
	long int pieces, procnum;
	char ** endptr = (char **)malloc(sizeof(char *));

	procnum = strtol(argv[1],endptr,10);
	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for procnum");

	start = strtol(argv[2],endptr,10);
	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for starting location");

	length = strtol(argv[3],endptr,10);
	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for number of bytes to read");

	pieces = strtol(argv[4],endptr,10);
	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for number of parts to split into");


	int cur, next; // Cur holds current character in question and compares it to the next character gotten from stream. fgetc returns ints, hence why cur and next are int type.
	int count, s; // Holds how many time the current character has been found, s is for error checking

	/*
	 * Update name of file to write to, and create that file
	 */
	char writetemp[ strlen(argv[5]) + procnum/10 + 2]; // writeto is now as large as the originally modified filename + # of digits + null byte
	char * writeto = (char *) &writetemp;
	writeto = strcpy(writeto, argv[5]); // writeto now contains the filename to write to without the ending digits

	if(pieces != 1){
		char ending[procnum/10 +2]; // the math inside the brackets is done to figure out how many digits procnum takes + added null byte
		sprintf(ending, "%d", (int) procnum); // ending contains the digit to be tacked onto writeto
		writeto = strcat(writeto, (char *) &ending); // writeto now contains the full filename
	}


	FILE * fs = fopen(writeto, "w"); // fopen creates a new file with writing capabilities

	if(fs == NULL)
		handle_error("fopen");

	/*
	 * File created. Now to start counting and writing to that file
	 */

	FILE * fso = fopen(argv[6], "r"); // Probably redundant to keep opening the file this way, but I feel it is safer for some odd reason

	s = fseeko(fso, start, 0); // Go to designated starting position for stream
	if(s != 0)
		handle_error("fseeko");


	/*
	 * This block is made so that on the first run of the while loop after this block, cur does not write junk to the new document.
	 * I say junk, since cur does not have a character from the document just yet, but it will after this block runs.
	 * All in all, this next and final block is very janky. I'm sorry, could not find a better way to loop this.
	 */

	next = fgetc(fso);
	off_t i = 1;
	while((next < 65 || next > 122 || (next > 90 && next < 97)) && i < length){ // If next is not a letter, keep looping until it is, or characters run out
		i++;
		next = fgetc(fso);
	}
	cur = next;

	count = 1; // Count will never be less than 1, since there is always at least 1 character
	while(i < length){

		next = fgetc(fso);
		i++;

		while((next < 65 || next > 122 || (next > 90 && next < 97)) && i < length){ // 65 == ascii A, 122 == ascii z, 90 == ascii Z, 97 == ascii a
			i++;
			next = fgetc(fso);
		}

		if(cur == next){
			count++;
		}else{
			if(count == 1){
				// Character appeared only once in a row
				s = fwrite(&cur,1,1,fs);
				if(s == 0)
					handle_error_en(105,"fwrite");

			}else if(count == 2){
				// Character appeared 2 times in a row
				s = fwrite(&cur,1,1,fs);
				if(s == 0)
					handle_error_en(105,"fwrite");

				s = fwrite(&cur,1,1,fs);
				if(s == 0)
					handle_error_en(105,"fwrite");

			}else{
				// Character appeared <count> times in a row
				char tmp[count/10 +2];
				sprintf(tmp, "%d", count);
				s = fwrite(&tmp,1,strlen((char *) &tmp),fs);
				if(s == 0)
					handle_error_en(105,"fwrite");

				s = fwrite(&cur,1,1,fs);
				if(s == 0)
					handle_error_en(105,"fwrite");
			}

			count = 1;
		}

		cur = next;
	}
	if(!(cur < 65 || cur > 122 || (cur > 90 && cur < 97))){
		if(count == 1){
			// Character appeared only once in a row
			s = fwrite(&cur,1,1,fs);
			if(s == 0)
				handle_error_en(105,"fwrite");

		}else if(count == 2){
			// Character appeared 2 times in a row
			s = fwrite(&cur,1,1,fs);
			if(s == 0)
				handle_error_en(105,"fwrite");

			s = fwrite(&cur,1,1,fs);
			if(s == 0)
				handle_error_en(105,"fwrite");

		}else{
			// Character appeared <count> times in a row
			char tmp[count/10 +2];
			sprintf(tmp, "%d", count);
			s = fwrite(&tmp,1,strlen((char *) &tmp),fs);
			if(s == 0)
				handle_error_en(105,"fwrite");

			s = fwrite(&cur,1,1,fs);
			if(s == 0)
				handle_error_en(105,"fwrite");
		}
	}
	fclose(fs);
	fclose(fso);

	exit(EXIT_SUCCESS);
}

