/*
 * part1.c
 *
 *  Created on: Feb 23, 2017
 *      Author: pawel
 */

/*
 * Source code for part 1 of the Process Communication project
 * This section entails the student to do the following in a single process:
 * 1) Read and copy information from a text
 * 2) Find the min, max and sum of the integers supposedly found in the text document
 * 3) Output the values found into a new text document
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

void launch(int argc, char ** argv){

	if(argc == 1){
		fprintf(stderr, "Usage: %s [file to read]\n",argv[0]);
		exit(EXIT_FAILURE);
	}else if(argc != 2)
		handle_error_en(22, "Invalid number of arguments");

	/*
	 * Initialization checks finished.
	 * Below is the start of acquiring and modifying the file and directory names for output
	 */

	char * filename = (char *)malloc((strlen(argv[1])+1) + 12); // length of original filename + 1 for null byte + 12 to be able to add '_outputA.txt' at end

	filename = strcpy(filename, argv[1]); // Acquire filename + directory

	if(filename[strlen(argv[1])-1] == '/') // replace possible '/' ending with null
		filename[strlen(argv[1])-1] = '\0';

	int i = strlen(filename);

	// Start replacing '.' with '_' for filename itself, not touching directory - this is to remove any extra extensions that could result in not being to open the file
	while(i > 0){

		if(filename[i] == '/')
			break;
		if(filename[i] == '.')
			filename[i] = '_';

		i--;
	}

	filename = strcat(filename, "_outputA.txt"); // add _output.txt to end

	FILE * fso = fopen(filename, "w"); // fopen attempts to create a new file (the output file) with writing capabilities

	if(fso == NULL)
		handle_error("fopen");

	/*
	 * Output filename has now been created.
	 * Now to actually attempt to open input file and attempt to read integers from it.
	 */

	FILE * fsi = fopen(argv[1], "r"); //fopen returns file stream (for input file)

	if(fsi == NULL)
		handle_error("fopen");

	int fd = fileno(fsi); // fileno returns file descriptor from file stream

	struct stat st; // Construct stat struct to be able to access the # of total bytes that the file has
	fstat(fd, &st); // st is filled out
	off_t size = st.st_size; // size now holds the number of bytes from the file passed in from user

	if(size == 0)
		handle_error_en(22, "Empty file");

	/*
	 * File has been opened, now to read from the file and compare the incoming numbers, along with summing them
	 */

	long min, max, sum, cur; // cur is the current number
	errno = 0;

	i = fscanf(fsi, "%ld", &cur); // i will now act as the error check for fscanf

	if(errno != 0)
		handle_error("fscanf");

	min = cur;
	max = cur; // All are equal to cur since cur is currently the first number
	sum = cur;

	while(i > 0){

		i = fscanf(fsi, "%ld", &cur);

		if(errno != 0)
			handle_error("fscanf");

		sum += cur;

		if(cur > max)
			max = cur;
		else if( cur < min)
			min = cur;

	}

	if(i == 0)
		handle_error_en(22, "Input file is not properly formatted. Have 1 number per line.");

	sum-=cur; // cur is added 1 too many times. this does not affect the min or max

	i = fclose(fsi); // j acts as an error catch

	if(i == EOF)
		handle_error("fclose");

	/*
	 * At this point, the max, min and sum should have been found
	 * Note: if the input file has a line that is improperly formatted between 2 properly formatted lines, the line after the improper one will not be read.
	 * Now to write the information found into the output file and close it
	 */

	i = fprintf(fso, "Hi I'm process %d and my parent is %d.\n", (int) getpid(), (int) getppid());

	if(i < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	i = fprintf(fso, "Max= %ld\n", max);

	if(i < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	i = fprintf(fso, "Min= %ld\n", min);

	if(i < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	i = fprintf(fso, "Sum= %ld\n", sum);

	if(i < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	i = fclose(fso);

	if(i == EOF)
		handle_error("fclose");

	return;

}

int main(int argc, char ** argv){
	launch(argc, argv);
	exit(EXIT_SUCCESS);
}

