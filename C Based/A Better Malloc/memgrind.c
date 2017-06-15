
#include <time.h>
#include "mymalloc.h"

/*
 * Test A: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one
 */

void TestA(){

	void * tester[3000];
	int i;
	for(i = 0; i < 3000; i++){
		tester[i] = mymalloc(1);
	}

	for(i = 0; i < 3000; i++){
		myfree(tester[i]);
	}
	return;
}

/*
 * Test B: malloc() 1 byte and immediately free it 3000 times in a row
 */

void TestB(){
	int i;

	for(i = 0; i < 3000; i++){
		void * test = mymalloc(1);
		myfree(test);
	}

	return;
}

/*
 * Test C: Randomly choose between a 1 byte malloc() or free() 6000 times
 *   - Keep track of each operation so that you eventually malloc() 3000 bytes, in total
 *   - Keep track of each operation so that you eventually free() all pointers
 */

void TestC(){

	void * tester[6000];
	int freed = 0;
	int malloced = 0;
	int current = 0;
	int total = 0;

	tester[malloced] = mymalloc(1); // This is to avoid trying to access tester[-1]
	malloced++;
	total++;

	while(total < 6000){
		if(rand() % 2 == 0){ // Malloc
			tester[malloced] = mymalloc(1);
			malloced++;
		}else{ // Free
			myfree(tester[malloced-1]);
			freed++;
		}
		total++;
	}

	while(malloced < 3000){
		tester[malloced] = mymalloc(1);
		total++;
		malloced++;
	}

	while(4996 > getunalloc()){
		myfree(tester[current]);
		current++;
		freed++;
		total++;
	}

}

/*
 * Test D: Randomly choose between a randomly-sized malloc() or free 6000 times
 *    - Keep track of each malloc so that your mallocs do not exceed your memory capacity
 *    - Keep track of each operation so that you eventually malloc() 3000 times
 *    - Keep track of each operation so that you eventually free() all pointers
 */

void TestD(){

	void * tester[6000];
	int freed = 0;
	int malloced = 0;
	int current = 0;
	int total = 0;

	tester[malloced] = mymalloc(rand() % 10); // This is to avoid trying to access tester[-1]
	malloced++;
	total++;

	while(total < 6000){
		if(rand() % 2 == 0){ // Malloc
			tester[malloced] = mymalloc(rand() % 10);
			malloced++;
		}else{ // Free
			myfree(tester[malloced-1]);
			freed++;
		}
		total++;
	}

	while(malloced < 3000){
		tester[malloced] = mymalloc(rand() % 10);
		total++;
		malloced++;
	}

	while(4996 > getunalloc()){
		myfree(tester[current]);
		current++;
		freed++;
		total++;
	}

}


int main(){

	/*
	 * Implementing time calculations below
	 * Contains both execution time and CPU time
	 */
	time_t start_t, end_t;
	double total_t;
	// Above segment is to calculate execution time of the program. (Not CPU time)

	clock_t time1, time2, c_total;
	// Above segment is to calculate how many clock cycles the program used.

	char ch; // Used to wait on user input
	int count;

	for(count = 0; count < 100; count++){
		time(&start_t);
		time1 = clock();
		TestA();
		time(&end_t);
		time2 = clock();
		total_t += difftime(end_t, start_t);
		c_total += (double)(time2 - time1) / CLOCKS_PER_SEC;
	}
	printf("---------------\n");
	printf("Avg. execution time of test A is: %f\n", total_t/100);
	printf("Avg. CPU time of the test A is: %f\n", (double)c_total / 100);
	printf("---------------\n");

	printf("Press ENTER key to Continue\n");
	scanf("%c",&ch);

	for(count = 0; count < 100; count++){
		time(&start_t);
		time1 = clock();
		TestB();
		time(&end_t);
		time2 = clock();
		total_t += difftime(end_t, start_t);
		c_total += (double)(time2 - time1) / CLOCKS_PER_SEC;
	}
	printf("---------------\n");
	printf("Avg. execution time of test B is: %f\n", total_t/100);
	printf("Avg. CPU time of the test B is: %f\n", (double)c_total / 100);
	printf("---------------\n");

	printf("Press ENTER key to Continue\n");
	scanf("%c",&ch);

	total_t = 0;
	c_total = 0;
	for(count = 0; count < 100; count++){
		time(&start_t);
		time1 = clock();
		TestC();
		time(&end_t);
		time2 = clock();
		total_t += difftime(end_t, start_t);
		c_total += (double)(time2 - time1) / CLOCKS_PER_SEC;
	}
	printf("---------------\n");
	printf("Avg. execution time of test C is: %f\n", total_t/100);
	printf("Avg. CPU time of the test C is: %f\n", (double)c_total / 100);
	printf("---------------\n");

	printf("Press ENTER key to Continue\n");
	scanf("%c",&ch);

	for(count = 0; count < 100; count++){
		time(&start_t);
		time1 = clock();
		TestD();
		time(&end_t);
		time2 = clock();
		total_t += difftime(end_t, start_t);
		c_total += (double)(time2 - time1) / CLOCKS_PER_SEC;
	}
	printf("---------------\n");
	printf("Avg. execution time of test D is: %f\n", total_t/100);
	printf("Avg. CPU time of the test D is: %f\n", (double)c_total / 100);
	printf("---------------\n");

	printf("Press ENTER key to Continue\n");
	scanf("%c",&ch);

	// TestE();
	// TestF();

	return 0;
}

