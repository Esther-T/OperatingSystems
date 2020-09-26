/**
 * Author: Esther Tan
   Title: Producer-Consumer Problem Program
   Description: This program simulates a multi-process synchronication problem that
				contains two processes: producer and consumer that share a fixed-size
				buffer
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TRUE 1

	buffer_item buffer[BUFFER_SIZE];
	/*define semaphores and mutex*/
	sem_t empty;
	sem_t full;
	pthread_mutex_t mutex;


int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

//helper function to insert the item into buffer
//pre  	item exists and is assigned
//post 	item is inserted into buffer
//		returns 0 if successful
//usage int status = insert_item(item);
int insert_item(buffer_item item)
{
	/* Acquire Empty Semaphore */
	sem_wait(&empty);

	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&mutex);
	
   buffer[insertPointer] = item;
   if(++insertPointer >= BUFFER_SIZE)
   {
		insertPointer = 0;
   }
		
	/* Release mutex lock and full semaphore */
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	
	return 0;
}

//helper function to remove the item from buffer
//pre  	item is a pointer and it is assigned
//post 	item is consumed by consumer
//		item points to the value of item consumed
//		if successful returns 0
//usage int status = remove_item(&item);
int remove_item(buffer_item* item)
{
	int temp;
	/* Acquire Full Semaphore */
	 sem_wait(&full);
	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&mutex);
	
	//gets the item out from the buffer
	temp = buffer[removePointer];
	*item = temp;
	
	if(++removePointer >= BUFFER_SIZE)
   {
		removePointer = 0;
   }
	//Release mutex lock
	pthread_mutex_unlock(&mutex);
	//empty semaphore
	sem_post(&empty);

	return 0;
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;
	if(argc != 4)
	{
		fprintf(stderr, "Usage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	//converts string into integer
	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);
	
	/*Initialize semaphores */
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	
	//Initialize mutex
	pthread_mutex_init(&mutex, NULL);
	
	/* Create the producer and consumer threads */
	pthread_t producer_t;
	pthread_t consumer_t;

	for(int j = 0; j < consumerThreads; j++)
	{
		pthread_create(&consumer_t, 0, consumer, NULL); 
	}
	for(int j = 0; j < producerThreads; j++)
	{
		pthread_create(&producer_t, 0, producer, NULL); 
	}


	/* Sleep for user specified time */
	sleep(sleepTime);
	
	//terminate the threads
	for(int i=0;i<producerThreads;i++)
    {
        pthread_join(producer_t,NULL);
    }
    for(int i=0;i<consumerThreads;i++)
    {
        pthread_join(consumer_t,NULL);
    }
	
	//destroy the mutex and semaphores
	pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
	
	return 0;
}

//producer thread
//produces item and placed them in buffer
void *producer(void *param)
{
	buffer_item item;
	int randomTime;
	while(TRUE)
	{
		randomTime = rand() % 5;
		sleep(randomTime);
		item = rand() % RAND_MAX;
		if (insert_item(item))
		{
			fprintf(stderr, "Producer error condition\n");
			break;
		}
		else
		{
			printf("Producer produced %d\n",item);
		}
	}
}

//consumer thread
//consumes the item from buffer
void *consumer(void *param)
{
	buffer_item item;
	int randomTime;
	while(TRUE)
	{
		randomTime = rand() % 5;
		sleep(randomTime);
		if (remove_item(&item))
		{
			fprintf(stderr, "Consumer error condition\n");
			break;
		}
		else
		{
			printf("Consumer consumed %d\n",item);
		}
	
	}
}
