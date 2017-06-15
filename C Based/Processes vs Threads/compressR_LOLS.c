/*
 * compressR_LOLS.c
 *
 *  Created on: Nov 15, 2016
 *      Author: pd304
 */

/*
 * Source code for PARENT of multiprocessing LOL compression
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define _FILE_OFFSET_BITS 64 // Allows for large file sources

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct process_info{
	int start, length, procnum, pieces; // Starting location for process, # of bytes that the process should read, its number, and # of pieces
	char * filename, * modified; // filename is the original filename given by argv, and modified is the original with the modifications in launch
};

void launch(int argc, char ** argv){

	if(argc == 1){
		fprintf(stderr, "Usage: %s [file to compress] [number of parts (non-zero)]\n",argv[0]);
		exit(EXIT_FAILURE);
	}else if(argc != 3)
		handle_error_en(22, "Invalid number of arguments");

	char ** endptr = (char **)malloc(sizeof(char *));
	long int pieces = strtol(argv[2],endptr,10); // # of pieces to split file -> # of times to fork

	if(pieces == 0)
		handle_error_en(22, "Cannot have 0 parts. Minimum is 1");

	if(**endptr != '\0')
		handle_error_en(22, "Something other than a number was entered for number of parts");

	/*
	 * At this stage, preliminary error checks are finished, pieces holds # of parts to split into and argv[1] holds the directory with filename
	 * Below: filename gets updated
	 */

	char * filename = (char *)malloc((strlen(argv[1])+1) + 5); // length of original filename + 1 for null byte + 4 to be able to add _LOLS at end

	filename = strcpy(filename, argv[1]); // Acquire filename + directory

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
	 * Below: file is found, opened and # of bytes each process will work on is assigned
	 */

	FILE * fs = fopen(argv[1], "r"); //fopen returns file stream

	if(fs == NULL)
		handle_error("fopen");

	int fd = fileno(fs); // fileno returns file descriptor from file stream

	struct stat st; // Construct stat struct to be able to access the # of total bytes that the file has
	fstat(fd, &st); // st is filled out
	off_t size = st.st_size; // size now holds the number of bytes from the file passed in from user

	fclose(fs);

	if(size == 0)
		handle_error_en(22, "Empty file");


	off_t first, rest; // Will hold # of bytes for the first process in first, and the # of bytes for all others. This is to resolve uneven distribution of bytes.

	rest = size/pieces;
	first = rest + size%pieces;

	/*
	 * At this point, first and rest hold the # of bytes that each process will be accountable for.
	 * Below: Start filling out a struct that holds all the arguments necessary for the worker to work
	 */

	struct process_info *pinfo = (struct process_info *)calloc(pieces,sizeof(struct process_info)); // Struct that will be passed into processes - will be recast as char * for args.
	if (pinfo == NULL)
		handle_error("Calloc");

	if(first == size){ // This ensures that a 0 is tacked onto the first file, even though only that file would be made.
		rest = 0;
		pieces = 1;
		pinfo[0].pieces = 2;
	}else
		pinfo[0].pieces = pieces;

	pinfo[0].procnum = 0;
	pinfo[0].start = 0;
	pinfo[0].length = first;
	pinfo[0].filename = argv[1];
	pinfo[0].modified = filename;

	for(i = 1; i < pieces; i++){
		pinfo[i].procnum = i;
		pinfo[i].start = pinfo[i-1].start + pinfo[i-1].length;
		pinfo[i].length = rest;
		pinfo[i].pieces = pieces;
	}

	/*
	 * At this point, pinfo has been fully filled out, and ready to be cast into a char *, so that exec will accept it as an argument
	 * Below: forking and exec-ing for child processes, after which the parent waits on its children
	 */

	pid_t pids[pieces];
	for(i = 0; i < pieces; i++){
		pids[i] = fork();
		if(pids[i] < 0)
			handle_error("fork");
		else if(pids[i] == 0){
			// Child process
			char * procnumC = (char *)malloc(pinfo[i].procnum/10 +2);
			char * startC = (char *) malloc(pinfo[i].start/10 +2);
			char * lengthC = (char *) malloc(pinfo[i].length/10 +2);
			char * piecesC = (char *) malloc(pinfo[i].pieces/10 +2); // Used to convert integer counterpart to char *

			sprintf(procnumC, "%d", pinfo[i].procnum);
			sprintf(startC, "%d", pinfo[i].start);
			sprintf(lengthC, "%d", pinfo[i].length);
			sprintf(piecesC, "%d", pinfo[i].pieces);

			char * args[] = {"./compressR_worker_LOLS", procnumC, startC, lengthC, piecesC, pinfo[0].modified, pinfo[0].filename, NULL};
			int s = execv(args[0], args);

			free(procnumC);
			free(startC);
			free(lengthC);
			free(piecesC);

			if(s == -1)
				handle_error("execv");
		}
	}

	// Parent process

	for(i = 0; i < pieces; i++){
		wait(NULL);
	}


	return;
}

int main (int argc, char ** argv){
	launch(argc, argv);
	exit(EXIT_SUCCESS);
}

