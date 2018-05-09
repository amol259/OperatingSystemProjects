///////////////////////////////////////////////////////////////////////////////
// threads.c
// Author: Amol Sidhu (ass140230)
// Date: March 11th, 2018

// Procedures:
//
// main	creates the process and consumer threads and intializes semaphores.
//produce_item_in_buffer will produce an item to the buffer
//consume_item_from_buffer will consume an item to the buffer
//consumer calls consume_item_from_buffer and outputs the time it took to consume an item and the amount of items in buffer before removal
//producer calls produce_item_from_buffer and outputs the time it took to produce an item and the amount of items in buffer after insertion
//
//Compile code: ~$ gcc threads.c -o threads
// Run code: ./threads

///////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include <string.h>		//String library
#include <unistd.h>		//Gives access to POSIX api
#include <pthread.h>	//pthread library
#include <semaphore.h>	//semaphore library
#include <time.h>		//Time library for clock


typedef int buffer_object;
#define buffer_size 10

//Global Variables

buffer_object buffer[buffer_size]; //The buffer
pthread_mutex_t mutex; //The mutex lock
sem_t full, empty; //semaphores
int count, in, out; 
int bufferCount;
int bufferSize=10;
int outcome;
unsigned long micros = 0;
float timeTaken = 0.0;
clock_t start, end;



//Functions prototypes 
int produce_item(buffer_object buffer_object);
int consume_item(buffer_object *item);
void *consumer(void *param);
void *producer(void *param);



/*
 * main
 * Initalzies semaphores and mutex lock and some global varibales. It also creates the producer and consumer thread and then it sleeps for 
 * 			  a specified amount of time before exiting the program.
 *
 * I/P:
 * ar
 *
 * O/P:
 * int Status code
 */

int main(){

srand(time(NULL));					//seed for number generator
pthread_mutex_init(&mutex,NULL); 	//creates the mutex lock
sem_init(&empty, 0, buffer_size); 	//creates empty semaphore and sets its size to buffer_sizr
sem_init(&full, 0,0);				// creates full semaphore and sets its size to 0

bufferCount=in=out=count=0; 

//Create producer and consumer thread
pthread_t producers; 
pthread_create(&producers,NULL, producer,NULL);
printf("Producer Thread Created\n");

pthread_t consumers; 
pthread_create(&consumers,NULL, consumer,NULL);
printf("Consumer Thread Created\n");


//sleep 2 seconds before termination
sleep(2);
printf("Exiting Program\n");
	return 0;

}//END MAIN

/*
 * produce_item_in_buffer()
 * Produces an item in the buffer, update the count of the items in buffer, and measures the time taken to produce 1 item
 * I/P: buffer_object insert_item, this is a reference to a type of buffer_object and the item that needs to be inserted(
 * 
 *
 * O/P:
 * outcome: 0 = sucess, 1 = failure
 */

int produce_item_in_buffer(buffer_object insert_item){
	
	start=clock();
	sem_wait(&empty);//locks empty
	pthread_mutex_lock(&mutex); //Locks mutex

	//Add item to buffer if buffer is not full
	if(count !=buffer_size)
	{
		
		buffer[in] = insert_item;
		in=(in+1) %buffer_size;
		count++;

		if(bufferCount < bufferSize){
			bufferCount++;
			printf("Items in buffer after Insertion: %d\n", bufferCount);
		}

		outcome = 0;//success
	} else
	outcome = -1; //Failure


	pthread_mutex_unlock(&mutex); //unlocks mutex
	sem_post(&full);	//unlocks full

	end=clock();
	micros= (double)end-start;
	timeTaken=(double) (micros)/CLOCKS_PER_SEC;		//total time between first and second clock() command
    printf("Producer TIME: %f\n",timeTaken);

    return outcome;

}

/*
 * consume_item_from_buffer
 * Consumes an item from the buffer, updated the count of items in buffer, and measures the time taken to consume 1 item
 * I/P:	
 * buffer_object *remove_item this is a reference to a type of buffer_object and the item it's to remove(remove_item)
 *
 * O/P:
 * outcome: 0 = success 1 = failure
 */


int consume_item_from_buffer(buffer_object *remove_item){

	start=clock();	//start clock time
	sem_wait(&full);
	pthread_mutex_lock(&mutex);

//Remove item from buffer to item if buffer not empty
	if(count!=0){
		*remove_item = buffer[out];
		out = (out - 1 ) % buffer_size;
		count--;

		if(bufferCount>= 0)
		{
			printf("Items in buffer before removal:%d\n", bufferCount);
			bufferCount--;
		}
			outcome=0;	//success
		
	} else
	
	outcome = -1;	//Failure	

	pthread_mutex_unlock(&mutex);
	sem_post(&empty);

	end=clock();	//stop clock time
	micros= (double)end-start;	//find difference between clocks
	timeTaken=(double) (micros)/CLOCKS_PER_SEC;		//total time between first and second clock() command
    printf("Consumer TIME: %f\n",timeTaken);
	return outcome;

	}

/*
 * *producer(void *param)
 * Producer which creates an array to store the time taken for producers inserting and calls the produce_item_from_buffer the desired amount of time. It also writes the data from the array
 *           to a text file which was then used to make the histogram.
 * I/P:
 * void *param can be used to pass any type of data type
 *
 * O/P:
 * None.
 */
 void *producer(void *param){

buffer_object number;
int n =10500;
float timeArrayProducer[n];
int   itemsInBufferBeforeInsert[n];
number = 0xFF;

for(int i=0; i<10500;i++){
    
    produce_item_in_buffer(number);
  	timeArrayProducer[i] = timeTaken;
  	itemsInBufferBeforeInsert[i] = bufferCount;
  }
}
/*
 * *consumer()
 * Consumer creates an array for the time taken for consumers and calls the consume_item_from_buffer function the desired amount of times. It also 
 * 			writes that data to a ouptut text file, which is used to make the corresponding histogram. The consumer function also creates an array 
 *			for the items in the buffer after conusmption. This data is written to a text file, which is used to make the corresponding histogram.
 * 
 * I/P:
 * void *param can be used to pass any type of data type
 *
 * O/P:
 * None.
 */

void *consumer(void *param){
buffer_object number;
int n = 10500;
float timeArrayConsumer[n];
int itemsInBufferAfterConsumption[n];


	for(int i=0;i<10500;i++)
  {
    consume_item_from_buffer(&number);
  	timeArrayConsumer[i]=timeTaken;
  	itemsInBufferAfterConsumption[i] = bufferCount;
  }

   
}



















