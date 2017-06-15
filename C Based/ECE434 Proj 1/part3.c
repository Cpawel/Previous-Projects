/*
 * part3.c
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
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define _FILE_OFFSET_BITS 64 // Allows for large file sources

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

void worker(int start, long * cur, FILE * fso, int pipefd[]){
	/*
	 * start is the starting index for cur - the array of longs
	 * num is the number of processes to still be created
	 */

	close(pipefd[0]); // close reading portion of initial pipe - communicates to parent from this child

	/*
	 * Start of finding local information
	 */

	long sum, min, max;

	sum = cur[start];
	min = sum;
	max = min;

	int i;
	for(i = 1; i < 50; i++){
		sum += cur[start+i];
		if(max < cur[start+i])
			max = cur[start+i];
		else if (min > cur[start+i])
			min = cur[start+i];
	}

	int err;
	err = fprintf(fso, "Hi I'm process %d and my parent is %d.\n", (int) getpid(), (int) getppid());

	if(err < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	err = write(pipefd[1], &sum, sizeof(long));
	if(err == -1)
		handle_error("write");

	err = write(pipefd[1], &min, sizeof(long));
	if(err == -1)
		handle_error("write");

	err = write(pipefd[1], &max, sizeof(long));
	if(err == -1)
		handle_error("write");

	close(pipefd[1]);
	return;
}

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

	char * filename = (char *)malloc((strlen(argv[1])+1) + 12); // length of original filename + 1 for null byte + 12 to be able to add '_outputC.txt' at end

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

	filename = strcat(filename, "_outputC.txt"); // add _output.txt to end

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

	// File opened, attempting read

	long * cur = (long *)calloc(10, sizeof(long)); // Starting with only space for 10 numbers - will grow as an arraylist does
	i = 0; // i will now act as the index for cur
	int j = 1; // j will act as the recipient of fscanf
	int k = 10; // k acts as the current length of cur
	errno = 0;

	while(j > 0){

		j = fscanf(fsi, "%ld", &cur[i]);

		if(errno != 0)
			handle_error("fscanf");

		i++;

		if(i == k ){ // should only come here when i = 10, 20, 40, etc.

			k = i*2;
			cur = (long *)realloc(cur, k*sizeof(long));

			if(cur == NULL)
				handle_error("realloc");
		}

	}
	i--;
	if(j == 0 && i == 0)
		handle_error_en(22, "Input file is not properly formatted. Have 1 number per line.");

	j = fclose(fsi); // j acts as an error catch

	if(j == EOF)
		handle_error("fclose");

	/*
	 * At this point, cur should now be populated with all the numbers obtainable from the input file.
	 * Note: if the input file has a line that is improperly formatted between 2 properly formatted lines, the line after the improper one will not be read.
	 * Now, the array is to be diced up in order to be sent out amongst multiple processes to find the necessary information
	 */

	int num = i / 50; // number of processes to be spawned - each process gets at least 50 numbers to process
	int extra = i % 50; // number of extra inputs that need to processed by the first child

	if(num != 0)
		extra += 50;

	long sum, max, min;

	int pid = -2; // process id - started at -2 in the case that there is no need to fork

	int pipefd[num-1][2];

	i = 0;

	while(i < num){ // only fork if there are more than 9 numbers
		if(pipe(pipefd[i]) == -1)
			handle_error("pipe");
		pid = fork();

		if(pid == 0){
			worker(extra + 50*i, cur, fso, pipefd[i]);
			exit(EXIT_SUCCESS);
		}else if(pid == -1)
			handle_error("fork");
		i++;
	}

	/*
	 * At this point the main process has forked off the children, which will start doing the things in worker.
	 * The parent now has to find its own values for sum, max and min, and then wait for the child.
	 * The parent processes the amount of numbers as set by extra
	 */

	extra--;
	sum = cur[extra];
	max = sum;
	min = max;

	while(extra > 0){
		sum += cur[extra - 1];
		if(max < cur[extra - 1])
			max = cur[extra - 1];
		else if (min > cur[extra - 1])
			min = cur[extra - 1];
		extra--;
	}
	// local values found, now have to receive children's numbers
	i = 0;
	if(pid != -2){
		while(i < num-1){
			close(pipefd[i][1]); // close writing portion of pipe
			long buf; // buffer to hold incoming longs

			j = read(pipefd[i][0], &buf, sizeof(long)); // j is an error catch - read will first read the sum found
			if(j == -1)
				handle_error("read01");
			sum += buf;

			j = read(pipefd[i][0], &buf, sizeof(long)); // then it reads the min found
			if(j == -1)
				handle_error("read02");
			if(buf < min)
				min = buf;

			j = read(pipefd[i][0], &buf, sizeof(long)); // and finally the max that was found is read
			if(j == -1)
				handle_error("read03");
			if(max < buf)
				max = buf;

			close(pipefd[i][0]);
			i++;
		}
		wait(NULL);
	}


	/*
	 * Now to write the information found into the output file and close it
	 */

	j = fprintf(fso, "Hi I'm process %d and my parent is %d.\n", (int) getpid(), (int) getppid());

	if(j < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	j = fprintf(fso, "Max= %ld\n", max);

	if(j < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	j = fprintf(fso, "Min= %ld\n", min);

	if(j < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	j = fprintf(fso, "Sum= %ld\n", sum);

	if(j < 0)
		handle_error_en(5, "fprintf failed unexpectedly");

	j = fclose(fso);

	if(j == EOF)
		handle_error("fclose");

	return;

}

int main(int argc, char ** argv){
	launch(argc, argv);
	exit(EXIT_SUCCESS);
}

