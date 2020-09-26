/**
Name: Esther Tan
Multithreaded Sorting Application
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	//output original list
	printf("Original List:\n");
	for(int i = 0; i < SIZE; i++)
	{
		printf("%d", list[i]);
		printf(" ");
	}
	printf("\n\n");
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a “parameters”
	//2. use “parameters” to specify the first half of the array
    //3. create the first thread
	parameters *data = (parameters *) malloc(sizeof(parameters));
	data -> from_index = 0;
	data -> to_index = (SIZE/2) - 1;
	pthread_create(&workers[0], 0, sorter, data);
	
	/* establish the second sorting thread */
	//1. call malloc to allocate a “parameters”
	//2. use “parameters” to specify the first half of the array
    //3. create the second thread
	data = (parameters *) malloc(sizeof(parameters));
	data -> from_index = SIZE/2;
	data -> to_index = SIZE - 1;
	pthread_create(&workers[1], 0, sorter, data);
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 
	for(i = 0; i < NUMBER_OF_THREADS -1; i++)
	{
		pthread_join(workers[i], NULL);
	}

	/* establish the merge thread */
	//reuse “parameters” to hold the beginning index in each half
	//create the third thread: merge 
	data = (parameters *) malloc(sizeof(parameters));
	data -> from_index = 0; //beginning of first half
	data -> to_index = SIZE/2; //beginning of second half
	pthread_create(&workers[2], 0, merger, data); 
	
	/* wait for the merge thread to finish */
	pthread_join(workers[2], NULL);

	/* output the sorted array */
	
	printf("Merged List:\n");
	for(int i = 0; i < SIZE; i++)
	{
		printf("%d", result[i]);
		printf(" ");
	}
	printf("\n\n");
	
    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */
//pre: the parameters for the list exist and are assigned
//post: the list of index within the parameters are sorted
//		in ascending order
void *sorter(void *params)
{
	parameters* p = (parameters *)params;
	
	int start = p -> from_index;
	int end = p -> to_index +1;
	int larger;
	
	//prints the list received
	printf("Sub-List received:\n");
	for(int i = start; i < end; i++)
	{
		printf("%d", list[i]);
		printf(" ");
	}
	printf("\n\n");
	
	//sort the list of index start to end
	for(int i = start; i < end; i++)
	{
		for(int j = i+1; j < end ; j++)
		{
			if(list[i] > list[j])
			{
				larger = list[i];
				list[i] = list[j];
				list[j] = larger;
			}
		}
	}
	
	//prints the sorted list
	printf("Sub-List sorted:\n");
	for(int i = start; i < end; i++)
	{
		printf("%d", list[i]);
		printf(" ");
	}
	printf("\n\n");
	
	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */
//pre: parameters for the list exist and are assigned
//post:merge and sort the two sublists
void *merger(void *params)
{
	//reuse “parameters” to hold the first index in each half
	//merge two sorted sublist to the array result
	
	parameters* p = (parameters *)params;
	
	int start = p -> from_index;
	int end = p -> to_index +1;
	int larger;
	
	for(int i = start; i < end; i++)
	{
		for(int j = start; j < (SIZE-1)-i; j++)
		{
			if(list[j] > list[j+1])
			{
				larger = list[j];
				list[j] = list[j+1];
				list[j+1] = larger;
			}
		}
	}
	
	//feeds back to result
	for(int i = 0 ; i < SIZE; i++)
	{
		result[i] = list[i];
	}
	
	
	pthread_exit(0);
}


