////////////////////////////////////////
// Matthew Buchanan
// CSC 484
// Spring 2024
// Program 2 Multi-threaded Factorial
////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Function definitions
void* Threaded_Multiply(void* n); // Multithreaded
void get_input(); // Only called once


// Global variables
long final_result;
int thread_count;
long number;


// Mutex for access to variable 'final_result'
pthread_mutex_t mutex;


// Main
int main(int argc, char* argv[]) 
{	
	// Initialize global variables and mutex
	thread_count = 0;
	number = -1;
	final_result = 1;	
	pthread_mutex_init(&mutex, NULL);
	
	// Call function to fetch user input
	get_input();

	// Skip if the number to factorialize is 0 or 1
   	if(number > 1)
   	{	
   		// Distribute the work evenly to the available threads
   		pthread_t* thread_handles;
   		thread_handles = malloc(thread_count*sizeof(pthread_t));
   		int i;
   		long n = number;
   		for (i = thread_count - 1; i >= 0; i--)
   		{
      		pthread_create(&thread_handles[i], NULL,
          	Threaded_Multiply, (void*)n);
        	n -= number / thread_count;
        }				
		
		// Wait for all threads to finish
   		for (i = thread_count - 1; i >= 0; i--) 
      		pthread_join(thread_handles[i], NULL); 
      			
      	// Compute left over values
      	while(n > 1)
      	{
			final_result *= n;
   			n--;
      	}
      		
      	free(thread_handles);    	  	
    }  	
      	
    // Display final result
    printf("Final Result: %ld\n", final_result);
      	
    // Clean up and exit
    pthread_mutex_destroy(&mutex);
    return 0;   
}


// Multi-Threaded function to multiply a portion of the final factorial
void* Threaded_Multiply(void* n)
{
	// Execute this threads portion of the final factorial
	long partial_result = (long)n;
	long i;
	for(i = (long)n - 1; i > (long)n - (number / thread_count); i--)
		partial_result *= i;
	
	// Critical section to multiply this thread's partial result into final_result
	pthread_mutex_lock(&mutex);
   	final_result *= partial_result;   	
   	pthread_mutex_unlock(&mutex);
}  


// Function to get number to factorialize and number of threads to use
void get_input()
{
	while(thread_count > number && number != 0) 
	{
		printf("Enter the number of threads to use from 1 to 1024\n");
		scanf("%d", &thread_count);
		while(thread_count < 1 || thread_count > 1024)
		{
			printf("Enter the number of threads to use from 1 to 1024\n");
			scanf("%d", &thread_count);
		}
	
		printf("Enter the number to factorialize, must be at least 0\n");
		scanf("%ld", &number);
		while(number < 0)
		{
			printf("Enter the number to factorialize, must be >= 0\n");
			scanf("%ld", &number);
		}
		
		if(thread_count > number)
			printf("Number of threads may not be greater than the number to factorialize\n");
	}
} 
