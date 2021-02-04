#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAX 6

FILE *fp;
char buff[6];
int front = 0;
int end = -1;
int numItem = 0;
pthread_t producer, consumer;
pthread_mutex_t buffMutex, condMutex;
pthread_cond_t con, prod;

// Returns false if the queue is full
bool isFull()
{
	return numItem == MAX;
}

 // Returns true if the queue is empty
bool isEmpty()
{
	return numItem == 0;
}

// Places character into queue or returns 1 if the queue is full
int insert(char c)
{
	if(!isFull())
	{
		if(end == MAX - 1)
		{
			end = 0;
		}
		else
		{
			end++;
		}
		
		buff[end] = c;
		
		if(numItem <= MAX)
		{
			numItem++;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

// Returns the ASCII character number associated with the next in queue or 0 if queue is empty
int removeChar(void)
{
	if(!isEmpty())
	{
		char datum = buff[front];
		if(front == MAX - 1)
		{
			front = 0;
		}
		else
		{
			front++;
		}
		if(numItem > 0)
		{
			numItem--;
		}
		
		return (int) datum;
	}
	else
	{
		return 0;
	}
}


void *producerThread(FILE *file)
{
	char c;
	c = (char)fgetc(file);
	while (1)
	{
		// Check to see if the queue is empty
		if(numItem == 1)
		{
			printf("Waking up consumer...\n");
			pthread_cond_signal(&con);
		}
		
		
		pthread_mutex_lock(&buffMutex);
		// Insert a character to the queue and read the next one
		insert(c);
		pthread_mutex_unlock(&buffMutex);
		c = (char)fgetc(file);
		
		
		pthread_mutex_lock(&condMutex);
		// Check to see if the buffer is full
		if(isFull() && c != EOF) 
		{
			printf("Buffer full, producer sleeping...\n");
			pthread_cond_wait(&prod, &condMutex);
			printf("Producer signalled, waking up...\n");
		}
		
		pthread_mutex_unlock(&condMutex);
		
	} 
}


void *consumerThread(void)
{
	char datum;
	do
	{
		// Check to see if the buffer is full 
		if(numItem == MAX - 1)
		{
			printf("Waking producer up...\n");
			pthread_cond_signal(&prod);
		}
		
		
		pthread_mutex_lock(&buffMutex);
		
		// Remove a character and print a character to the string
		datum = (char)removeChar();
		pthread_mutex_unlock(&buffMutex);
		if(datum != '\n')
		{
			printf("%c\n", datum);
		}
		
		
		pthread_mutex_lock(&condMutex);
		
		// Check to see if the queue is full
		if(isEmpty() && datum != '\n')
		{
			printf("buff is empty, consumer sleeping...\n");
			pthread_cond_wait(&con, &condMutex);
			printf("Consumer signalled, waking up...\n");
		}
		
		
		pthread_mutex_unlock(&condMutex);
		
	} while(datum != '\n');
	
	printf("Delimeter reached, killing consumer and producer...\n");
	pthread_cancel(producer);
	pthread_exit(NULL);
}

int main()
{
	// Open mesage.txt for reading
	fp = fopen("message.txt", "r");
	
	// Initialize mutexes and conditions
	pthread_mutex_init(&condMutex, NULL);
	pthread_mutex_init(&buffMutex, NULL);
	pthread_cond_init(&con, NULL);
	pthread_cond_init(&prod, NULL);
	
	// Create threads for producer and consumer
	pthread_create(&producer, NULL, producerThread,(void *) fp);
	pthread_create(&consumer, NULL, consumerThread, NULL);
	
	// Wait for threads to exit
	pthread_exit(NULL);
	
	// Cleanup  mutexes and conditions
	pthread_mutex_destroy(&condMutex);
	pthread_mutex_destroy(&buffMutex);
	pthread_cond_destroy(&con);
	pthread_cond_destroy(&prod);
	
	// Close messages.txt
	fclose(fp);
	
	return 1;
}
