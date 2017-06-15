/*
 * part4.c
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

void worker(int start, int length, long * cur, FILE * fso, int pipefd[]){
	/*
	 * start is the starting index for cur - the array of longs
	 * num is the number of processes to still be created
	 */

	close(pipefd[0]); // close reading portion of initial pipe - communicates to parent from this child

	int extra = 0;
	int pid = -2; // indicator of if this child is the last child needed
	int pipefd2[2], pipefd3[2];

	if(length > 50){ // length is large - split it up into 3 parts, 2 parts for 2 children and 1 for parent
		extra = length % 3;
		length /= 3;

		if(pipe(pipefd2) == -1)
			handle_error("pipe");
		pid = fork();

		if(pid == 0){
			worker(start + length + extra, length, cur, fso, pipefd2);
			exit(EXIT_SUCCESS);
		}else if(pid == -1)
			handle_error("fork");

		if(pipe(pipefd3) == -1)
			handle_error("pipe");
		pid = fork();

		if(pid == 0){
			worker(start + 2*length + extra, length, cur, fso, pipefd3);
			exit(EXIT_SUCCESS);
		}else if(pid == -1)
			handle_error("fork");
	}

	/*
	 * 2 children could have been spawned off from the above code.
	 * Now, start of finding local information while children initialize
	 */

	long sum, min, max;

	sum = cur[start];
	min = sum;
	max = min;

	int i;
	for(i = 1; i < length; i++){
		sum += cur[start+i];
		if(max < cur[start+i])
			max = cur[start+i];
		else if (min > cur[start+i])
			min = cur[start+i];
	}

	/*
	 * Local data found, now to retrieve data from children and then send own data up to parent
	 */

	if(pid != -2){
		close(pipefd2[1]); // close writing portion of pipe
		close(pipefd3[1]);
		long buf; // buffer to hold incoming longs

		extra = read(pipefd2[0], &buf, sizeof(long)); // extra is an error catch - read will first read the sum found
		if(extra == -1)
			handle_error("read01");
		sum += buf;

		extra = read(pipefd2[0], &buf, sizeof(long)); // then it reads the min found
		if(extra == -1)
			handle_error("read02");
		if(buf < min)
			min = buf;

		extra = read(pipefd2[0], &buf, sizeof(long)); // and finally the max that was found is read
		if(extra == -1)
			handle_error("read03");
		if(max < buf)
			max = buf;

		extra = read(pipefd3[0], &buf, sizeof(long)); // extra is an error catch - read will first read the sum found
		if(extra == -1)
			handle_error("read04");
		sum += buf;

		extra = read(pipefd3[0], &buf, sizeof(long)); // then it reads the min found
		if(extra == -1)
			handle_error("read05");
		if(buf < min)
			min = buf;

		extra = read(pipefd3[0], &buf, sizeof(long)); // and finally the max that was found is read
		if(extra == -1)
			handle_error("read06");
		if(max < buf)
			max = buf;

		close(pipefd2[0]);
		close(pipefd3[0]);
		wait(NULL);
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

	char * filename = (char *)malloc((strlen(argv[1])+1) + 12); // length of original filename + 1 for null byte + 12 to be able to add '_outputD.txt' at end

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

	filename = strcat(filename, "_outputD.txt"); // add _output.txt to end

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

	int num = i / 2; // num splits the numbers in two to start the chain of splitting the array
	int extra = i % 2; // number of extra inputs that need to processed by either starting process

	long sum, max, min;

	int pid; // process id

	int pipefd[2][2];

	if(pipe(pipefd[0]) == -1)
		handle_error("pipe");
	pid = fork();

	if(pid == 0){
		worker(0, num + extra, cur, fso, pipefd[0]);
		exit(EXIT_SUCCESS);
	}else if(pid == -1)
		handle_error("fork");

	if(pipe(pipefd[1]) == -1)
		handle_error("pipe");
	pid = fork();

	if(pid == 0){
		worker(num+extra, num, cur, fso, pipefd[1]);
		exit(EXIT_SUCCESS);
	}else if(pid == -1)
		handle_error("fork");

	/*
	 * At this point the main process has forked off the children, which will start doing the things in worker.
	 * The parent now has to wait for the values to come back from its two children
	 */

	sum = 0;
	i = 0;
	while(i < 2){
		close(pipefd[i][1]); // close writing portion of pipe
		long buf; // buffer to hold incoming longs

		j = read(pipefd[i][0], &buf, sizeof(long)); // j is an error catch - read will first read the sum found
		if(j == -1)
			handle_error("read07");
		sum += buf;

		j = read(pipefd[i][0], &buf, sizeof(long)); // then it reads the min found
		if(j == -1)
			handle_error("read08");
		if(buf < min)
			min = buf;

		j = read(pipefd[i][0], &buf, sizeof(long)); // and finally the max that was found is read
		if(j == -1)
			handle_error("read09");
		if(max < buf)
			max = buf;

		close(pipefd[i][0]);
		i++;
	}
	wait(NULL);


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

