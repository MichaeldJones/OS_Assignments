#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024

int total = 0;
int n1,n2;
char *s1,*s2;
FILE *fp;
pthread_t thread1, thread2;
pthread_mutex_t waitForFile;

int *readf(FILE *fp)
{
	// Open file for read only
	if((fp=fopen("strings.txt", "r")) == NULL)
	{
		printf("ERROR: can't open string.txt!\n");
		pthread_exit((void *)0);
	}
	
	// Allocate new character pointer s1
	s1 = (char *) malloc(sizeof(char)*MAX);
	if(s1==NULL)
	{
		printf("ERROR: Out of memory!\n");
		pthread_exit((void *) -1);
	}
	
	// Allocate new character pointer s2
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s2==NULL)
	{
		printf("ERROR: Out of memory\n");
		pthread_exit((void *)(-1));
	}
	
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1)-1;  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/
	pthread_mutex_unlock(&waitForFile);
	
	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		pthread_exit((void *)-1);
}

void *num_substring(void)
{
	int i,j,k;
	int count; 
	pthread_mutex_lock(&waitForFile);
	for (i = 0; i <= (n1-n2); i++)
	{   
		count=0;
		for(j = i,k = 0; k < n2; j++,k++)
		{  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k))
				break;

			count++;
			
			if(count==n2)    
				total++;		/*find a substring in this step*/                          
		}
	}
	pthread_mutex_unlock(&waitForFile);
	printf("The number of substrings is: %d\n", total);
	pthread_exit((void *) 0);
}

int main(int argc, char *argv[])
{
	// Initialize mutex
 	pthread_mutex_init(&waitForFile, NULL);
 	pthread_mutex_lock(&waitForFile);
 	
	// Create thread to read the file
 	pthread_create(&thread1, NULL, readf,(void *) fp);
	
	// Create thread to calculate number of substrings
 	pthread_create(&thread2, NULL, num_substring, NULL);
 	
 	pthread_exit((void *)1);
	
	return 1;
}











