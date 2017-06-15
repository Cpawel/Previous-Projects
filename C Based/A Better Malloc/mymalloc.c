/*
 * mymalloc.c
 *
 *  Created on: Oct 2, 2016
 *      Author: pd304
 */

#include "mymalloc.h"


static char mem[5000];

unsigned char init = 0; // Variable to make sure to place initial size in array
unsigned int unallocated = 4996; // Amount of unallocated bytes from total array

unsigned int cur, next; // Ints that hold the addresses while going through the array (Index of array)


typedef union Reader{
	int integer;
	char chars[4];
}Reader;

void errprint(char*fname, size_t lnum, char*msg){
  printf("File:%s Line:%zd, %s", fname,lnum,msg);

}

unsigned int getunalloc(){
	return unallocated;
}

unsigned int bestfit(unsigned int size){

	if(size % 2 != 0)
		size++;

	union Reader reader;
	unsigned int curdiff = 5000; // Holds the smallest difference
	int diff = -1;
	cur = 0;
	next = 0;


	while(next < 4996 && diff != 0){ // Stops upon reaching end of memory or if the difference between the wanted size and slot found is 0

		reader.chars[0] = mem[next];
		reader.chars[1] = mem[next+1];
		reader.chars[2] = mem[next+2];
		reader.chars[3] = mem[next+3];

		if(reader.integer % 2 == 0){
			diff = reader.integer - size;
			if(diff >= 0 && curdiff > diff){
				curdiff = diff;
				cur = next;
			}
			next += reader.integer + 4;
		}else
			next += reader.integer + 3; //Slot was allocated, therefore true size is size-1
	}

	if(curdiff == 5000){ // No suitable chunk slot found, return 5000 for special case in malloc

		return curdiff;
	}else if(curdiff != 0 && curdiff < 5){ // Slot found is suitable, but not enough space is left over for metadata. Tacking on the extra to output
		reader.integer = size+curdiff+1; // reader now holds the size of the new chunk, along with a 1 for allocated - chunk size should be the same as before but +1

		mem[cur] = reader.chars[0];
		mem[cur+1] = reader.chars[1];
		mem[cur+2] = reader.chars[2];
		mem[cur+3] = reader.chars[3];

		if(reader.integer -1 > unallocated) // unallocated space is not large enough
			return 5000;

		int tmp = reader.integer - 1;
		unallocated -= tmp; // Only -1 due to allocated's +1 - No new chunk was made


		return cur;

	}else{ // Slot was found, excess space is enough to make a new chunk with metadata, creating slot

		reader.integer = size+1;

		mem[cur] = reader.chars[0]; // Read in allocated size
		mem[cur+1] = reader.chars[1];
		mem[cur+2] = reader.chars[2];
		mem[cur+3] = reader.chars[3];

		next = cur + size +4; // Go to start of new metadata location

		reader.integer = curdiff - 4; // Reader now holds the size of the new free space

		mem[next] = reader.chars[0]; // Read in new size of free memory
		mem[next+1] = reader.chars[1];
		mem[next+2] = reader.chars[2];
		mem[next+3] = reader.chars[3];

		if(size +4 > unallocated) // unallocated space is not large enough
					return 5000;

		int tmp = size+4;
		unallocated -= tmp; // chunk size of size has been allocated, along with 4 bytes for new metadata

		return cur;

	}
}


void * mymalloc(size_t size){

	unsigned int addr = 0;
	union Reader reader;

	if(init == 0){
		init = 1;
		reader.integer = 4996;
		mem[0] = reader.chars[0]; // Didn't want to create a variable just to create a small loop
		mem[1] = reader.chars[1];
		mem[2] = reader.chars[2];
		mem[3] = reader.chars[3];
	}

	if(size == 0){

		return ((void*)0); // Return null for malloc(0)
	}

	if(size > unallocated && unallocated > 0){

		errmsg("Memory chunk size asked for is too large to accommodate. Returning null.\n");

		// printf("Memory chunk size asked for is too large to accommodate, returned largest possible chunk.\n");

		return ((void *) 0);

	}else if(unallocated == 0 ){
		// printf("Out of memory!\n");

		errmsg("Out of memory!\n");
		return ((void *) 0);
	}


	addr = bestfit(size);



	if(addr >= 5000){
		errmsg("No large enough memory space.\n");
		return ((void*)0); //returns null, no large enough chunk to fit designation
	}

	return &mem[addr+4];
}

void myfree(void * addr){

	addr-=4;

	union Reader reader;
	unsigned int size;

	if(addr < (void *)&mem[0] || addr > (void *)&mem[4996]){ // Pointer passed in is out of bounds of the array. Cannot point to mem[4996] due to metadata
		errmsg("Not Allocated!\n");
		// printf("SegFault: Not Allocated\n");

		return;
	}
	unsigned int found = 0;
	unsigned int allocated;
	cur = 0; // Acts as previous location
	next = 0; // Acts as current location



	while(!(addr > (void *)&mem[cur] && addr < (void *)&mem[next])){ // Loops as long as addr is not in between the current address and next address

		reader.chars[0] = mem[next]; // Retrieve size of current chunk
		reader.chars[1] = mem[next+1];
		reader.chars[2] = mem[next+2];
		reader.chars[3] = mem[next+3];

		size = reader.integer;


		if(size % 2 == 1){ // Remember, odd means allocated, even means unallocated

			reader.integer--;
			allocated = 1;
		}else
			allocated = 0;

		if(addr == (void *)&mem[next]){ // Break upon finding correct address

			found = 1;
			break;
		}

		cur = next; // The current location is saved in cur

		next = cur + reader.integer + 4; // Go to the next location

	}

	if(found == 0){ // Address passed in was not valid
		errmsg("Not a valid address\n");
		//	  printf("Not a valid address\n");

		return;

	}else if(allocated == 0){ // Trying to free a free chunk - not allowed
		errmsg("Already Free\n");
		// printf("Already free\n");

		return;
	}


	/*
	 *  At this point, next is the index that holds the correct address, reader.integer holds the size of the chunk
	 *  cur holds the index to the start of the previous chunk + metadata -> important for memory combining
	 */


	mem[next] = reader.chars[0]; // Update the chunk as free
	mem[next+1] = reader.chars[1];
	mem[next+2] = reader.chars[2];
	mem[next+3] = reader.chars[3];



	size = reader.integer; // Store the size of the freed chunk

	unallocated += size; // Update how many free bytes exist

	if(cur != next){
		reader.chars[0] = mem[cur]; // Retrieve size of previous chunk
		reader.chars[1] = mem[cur+1];
		reader.chars[2] = mem[cur+2];
		reader.chars[3] = mem[cur+3];


		if(reader.integer % 2 == 0){ // Previous chunk is free - time to combine the newly freed chunk with the free chunk before it

			found = 0; // Update found to zero in the future case that the chunk after the freed chunk is also free
			reader.integer += size + 4; // Reader now holds the size of the newly freed chunk + the meta data of that chunk + the size of free data before new chunk

			mem[cur] = reader.chars[0]; // Update the new free chunk size
			mem[cur+1] = reader.chars[1];
			mem[cur+2] = reader.chars[2];
			mem[cur+3] = reader.chars[3];

			unallocated += 4; // Already updated to account for the freeing of user data, but combination frees the meta data, so +4.


		}
	}
	/*
	 * At this point, the pointer passed in has had its data freed, and then combined with the free chunk before it (if it existed)
	 * Now to check the chunk that was in front of the pointer data, and combine it if needs be
	 */

	next += size + 4; // Next goes to the chunk forward

	if(next > 4995) // Reached the end of the array
		return;

	reader.chars[0] = mem[next]; // Retrieve size of forward chunk
	reader.chars[1] = mem[next+1];
	reader.chars[2] = mem[next+2];
	reader.chars[3] = mem[next+3];



	if(reader.integer % 2 == 0){ // Forward chunk is free - time to combine the newly freed chunk with the free chunk after it
		unallocated += 4; // One way or another, meta data is being freed. +4 to Gryffindor! .. wait, no.. +4 to unallocated!

		if(found != 0){ // This means that the previous chunk to the freed one was not free - cur can be used and dont have to worry about double the trouble.. for now

			cur = next - size - 4; // Cur now holds the index that will hold the updated free space

			reader.integer += size + 4; // Reader now holds the sizes of the of the chunks pointed to by cur and next, + 4 for metadata in between

			mem[cur] = reader.chars[0]; // Update the new free chunk size
			mem[cur+1] = reader.chars[1];
			mem[cur+2] = reader.chars[2];
			mem[cur+3] = reader.chars[3];

			return; // Our work is done. Beam me up, Scotty.


		}

		/*
		 * How grand! The slot in front AND behind the newly freed chunk were also free... lets get this party started!
		 *
		 * Situation: Cur holds the start to the space that is the nice and big free chunk (I assume big, since its eating 2 of its brothers.. irrelevant)
		 *  Next is now pointing to the start of the chunk after the chunk that was passed in
		 *  Reader is holding the size of next's chunk
		 *  Now to update cur's size.. again..
		 */

		size = reader.integer + 4; // Size will now hold next's chunk size + metadata

		reader.chars[0] = mem[cur]; // Retrieve size of cur's chunk after eating one of its brothers
		reader.chars[1] = mem[cur+1];
		reader.chars[2] = mem[cur+2];
		reader.chars[3] = mem[cur+3];

		reader.integer += size; // Tack on the size of next's chunk to cur's chunk

		mem[cur] = reader.chars[0]; // Re-update cur with the size of itself and its two brothers now in its belly
		mem[cur+1] = reader.chars[1];
		mem[cur+2] = reader.chars[2];
		mem[cur+3] = reader.chars[3];

		return; // Task complete. A chunk just ate its neighbor, and its neighbor's neighbor. Hot.

	}
	return;
}

