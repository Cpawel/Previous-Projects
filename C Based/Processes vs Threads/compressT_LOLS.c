/*
 * compressT_LOLS.c
 *
 *  Created on: Nov 15, 2016
 *      Author: pd304
 */

/*
 * Source code for threading LOL compression
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define _FILE_OFFSET_BITS 64 // Allows for large file sources

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info{
	pthread_t thread_id; // ID returned from creating the thread
	int start, length, threadnum, pieces; // Starting location for thread, # of bytes that the thread should read, thread number, and # of pieces
	char * filename, * modified; // filename is the original filename given by argv, and modified is the original with the modifications in main
};

static void * encoder(void * entry){
	/*
	 * Initialization
	 */

	struct thread_info * tinfo2 = (struct thread_info *) entry;
	off_t length, start;
	start = (*tinfo2).start;
	length = (*tinfo2).length;

	int cur, next; // Cur holds current character in question and compares it to the next character gotten from stream. fgetc returns ints, hence why cur and next are int type.
	int count, s; // Holds how many time the current character has been found, s is for error checking

	/*
	 * Update name of file to write to, and create that file
	 */
	char writetemp[ strlen((*tinfo2).modified) + (*tinfo2).threadnum/10 + 2]; // writeto is now as large as the originally modified filename + # of digits + null byte
	char * writeto = (char *) &writetemp;
	if((*tinfo2).pieces != 1){
		char ending[(*tinfo2).threadnum/10 +2]; // the math inside the brackets is done to figure out how many digits threadnum takes + added null byte
		sprintf(ending, "%d", (*tinfo2).threadnum); // ending contains the digit to be tacked onto writeto

		writeto = strcpy(writeto, (*tinfo2).modified); // writeto now contains the filename to write to without the ending digits
		writeto = strcat(writeto, (char *) &ending); // writeto now contains the full filename

	}else
		writeto = strcpy(writeto, (*tinfo2).modified); // writeto now contains the filename to write to without the ending digits


	FILE * fs = fopen(writeto, "w"); // fopen creates a new file with writing capabilities

	if(fs == NULL)
		handle_error("fopen");

	/*
	 * File created. Now to start counting and writing to that file
	 */

	FILE * fso = fopen((*tinfo2).filename, "r"); // Probably redundant to keep opening the file this way, but I feel it is safer for some odd reason

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

	return NULL;
}

void launch(int argc, char ** argv){

	if(argc == 1){
		fprintf(stderr, "Usage: %s [file to compress] [number of parts (non-zero)]\n",argv[0]);
		exit(EXIT_FAILURE);
	}else if(argc != 3)
		handle_error_en(22, "Invalid number of arguments");

	char ** endptr = (char **)malloc(sizeof(char *));
	long int pieces = strtol(argv[2],endptr,10); // # of pieces to split file

	if(pieces == 0)
		handle_error_en(22, "Cannot have 0 parts. Minimum is 1");

	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for number of parts");

	/*
	 * At this stage, preliminary error checks are finished, pieces holds # of parts to split into and argv[1] holds the directory with filename
	 * Below: filename gets updated
	 */

	char * filename = (char *)malloc((strlen(argv[1])+1) + 5); // length of original filename + 1 for null byte + 4 to be able to add _LOLS at end

	filename = strcpy(filename, argv[1]); // Acquire filename + directory?

	if(filename[strlen(argv[1])-1] == '/') // replace possible '/' ending with null
		filename[strlen(argv[1])-1] = '\0';

	filename = strcat(filename, "_LOLS"); // add _LOLS to end

	int i = strlen(filename);

	// Start replacing '.' with '_' for filename itself, not touching directory
	while(i > 0){

		if(filename[i] == '/')
			break;
		if(filename[i] == '.')
			filename[i] = '_';

		i--;
	}

	/*
	 * At this stage, filename now holds the updated filename with the same directory. NOTE: number may still have to be concatenated to end
	 * Below: file is found, opened and # of bytes each thread will work on is assigned
	 */

	FILE * fs = fopen(argv[1], "r"); //fopen returns file stream

	if(fs == NULL)
		handle_error("fopen");

	int fd = fileno(fs); // fileno returns file descriptor from file stream

	struct stat st; // Construct stat struct to be able to access the # of total bytes that the file has
	fstat(fd, &st); // st is filled out
	off_t size = st.st_size; // size now holds the number of bytes from the file passed in from user

	if(size == 0)
		handle_error_en(22, "Empty file");


	off_t first, rest; // Will hold # of bytes for the first thread in first, and the # of bytes for all others. This is to resolve uneven distribution of bytes.

	rest = size/pieces;
	first = rest + size%pieces;

	/*
	 * At this point, fs is the stream for the file, first and rest hold the # of bytes that each thread will be accountable for.
	 * Below: start assigning threads to work on encoding and compressing
	 */

	struct thread_info *tinfo = (struct thread_info *)calloc(pieces,sizeof(struct thread_info)); // Struct that will be passed into threads
	if (tinfo == NULL)
		handle_error("Calloc");

	tinfo[0].threadnum = 0;
	tinfo[0].start = 0;
	tinfo[0].length = first;
	tinfo[0].filename = argv[1];
	tinfo[0].modified = filename;
	tinfo[0].pieces = pieces;
	int s = pthread_create(&tinfo[0].thread_id,0,&encoder,&tinfo[0]); // create the first thread

	if(s != 0)
		handle_error_en(s,"pthread_create");

	if(rest != 0 && first != size){ // If rest = 0 or first takes all the bytes, skip making other threads. They would be useless. Not creating excess empty files.
		for(i = 1; i < pieces; i++){
			tinfo[i].threadnum = i;
			tinfo[i].length = rest;
			tinfo[i].start = tinfo[i-1].start + tinfo[i-1].length; //add 1?
			tinfo[i].filename = argv[1];
			tinfo[i].modified = filename;
			tinfo[i].pieces = pieces;
			s = pthread_create(&tinfo[i].thread_id,0,&encoder,&tinfo[i]);
			if(s != 0)
				handle_error("pthread_create");
		}
	}

	void * res;
	for (i = 0; i < pieces; i++) {
		s = pthread_join(tinfo[i].thread_id, &res);
		if (s != 0)
			handle_error_en(s,"pthread_join");

		free(res);      /* Free memory allocated by thread */
		if(rest == 0)
			break; // If rest is 0, the other threads were never made, even if asked for.
	}

	free(tinfo);

	return;
}
int main(int argc, char ** argv){
	launch(argc, argv);
	exit(EXIT_SUCCESS);
}

