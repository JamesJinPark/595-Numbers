/* James Park
 * CIT 595 HW #2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//global variables
double max;
double min;
double average;
double total;
double num;
double input_start[5];
double* input_end = input_start;
int quit;
pthread_mutex_t lock1;

/*
 * helper function
 * does calculations related to max, min, average
 */
void calculate(double f){
	if (f > max){
		max = f;
	}
	if (f < min){
		min = f;
	}
	total = f + total;
	num++;
}

/*
 * helper function
 * stores the last five inputs into array
 */
void store_input(double f){
	for (int i = 3; i >= 0; i--){
		input_start[i + 1] = input_start[i];
	}
	input_start[0] = f;
}

/*
 * thread function
 *
 * continuously loops, prompting the user to input a (floating point) number via the keyboard
 * scans user input the first time to initialize global variables and then goes into do while loop
 */
void* func1(void* p){
	char input[256];
	scanf("%s", input);
	double f;
	if (strcmp(input, "0")!= 0) {
		if (strcmp(input, "q") == 0){
			pthread_mutex_lock(&lock1);
			quit = 0;
			pthread_mutex_unlock(&lock1);
			pthread_exit(NULL);//exits and closes thread
		}
		f = atof(input);//returns 0 if input is not a float
		if (f != 0) {
			// tests if input is a float; if float then adds the number onto store_input
			pthread_mutex_lock(&lock1);
			max = f;
			min = f;
			total = f + total;
			num++;
			store_input(f);
			pthread_mutex_unlock(&lock1);
	 	}
	}
	else{
		pthread_mutex_lock(&lock1);
		calculate(0);
		store_input(0);
		pthread_mutex_unlock(&lock1);
	}
	do{
		scanf("%s", input);
		double f;
		if (strcmp(input, "0")!= 0) {
			if (strcmp(input, "q") == 0){
				pthread_mutex_lock(&lock1);
				quit = 0;
				pthread_mutex_unlock(&lock1);
				pthread_exit(NULL);//exits and closes thread
			}
			f = atof(input);//returns 0 if input is not a float
			if (f != 0) {
				// tests if input is a float; if float then adds the number onto store_input
				pthread_mutex_lock(&lock1);
				calculate(f);
				store_input(f);
				pthread_mutex_unlock(&lock1);
	 		}
		}
		else{
			pthread_mutex_lock(&lock1);
			calculate(0);
			store_input(0);
			pthread_mutex_unlock(&lock1);
		}
	} while(1);
	return p; //returning p doesn't mean anything here.  thread will exit the do while loop using pthread_exit
}

/*
 * main function
 * pauses for ten seconds, and then displays:
 * the maximum value input so far;
 * the minimum value input so far;
 * the average of all values input so far;
 * and, a listing of the last five values that were input, in order, starting with the most recent.
 *
 * Pauses again and repeats.
 *
 */
int main(){
	quit = 1;
	total = 0;
	num = 0;
	pthread_t t1;
	pthread_mutex_init(&lock1, NULL);
	pthread_create(&t1, NULL, &func1, NULL);
	do {
		pthread_mutex_lock(&lock1);
		if (quit == 0) {
			pthread_mutex_unlock(&lock1);
			break;
		}
		pthread_mutex_unlock(&lock1);
		sleep(10);
		pthread_mutex_lock(&lock1);
		if (num != 0) {
			average = total / num;
			pthread_mutex_unlock(&lock1);
		}
		else {
			average = 0;
			pthread_mutex_unlock(&lock1);
		}
		printf("\nmax = %f\nmin = %f\naverage = %f\n", max, min, average);
		for (int j = 0; j < 5; j++){
			printf("%f\n", input_start[j]);
		}
		printf("\n");
	} while(1);
	pthread_join(t1, NULL);
	return 1;
}