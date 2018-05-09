/*
 * filename: prgm1_sidhu
 * Author: AMOL SIDHU
 * Class: Operating Systems CE 4348.001
 * main - Function that initalizes the semaphores and creates producer and consumer processes.
 * producer - Function that inserts an item into the buffer, updates count, finds the time taken to insert each item and outputs that data to a text file.
 * consumer - Function that removes an item from the buffer, updates count, and finds the time taken to remove each item and outputs that data to text file.
 *
 *    NOTE: File operations were written on my machines path(mac os), so the write operations will not work unless the path is modified.
  *         All the data is available in the excel file which contains 4 different sheets and their corresponding histogram and data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/mman.h>

typedef int buffer_type;
#define buffer_size 10

/*Global Variables*/
buffer_type buffer[buffer_size];
sem_t full, empty,mutex; //semaphores
int in;
int out;
int count;
unsigned long micros = 0;
float timeTaken = 0.0;
clock_t start, end;


/* prototypes*/
void producer();
void consumer();


/*
 * producer()
 * Creates 2 arrays, and proceeds to insert 10000 items into the buffer and finds the time for each insert operation. The time is stored in one array and the elements
 * in the buffer before the insert is stored in another. Semaphores are used for mutual exclusion and to signal when the buffer is full.
 * I/P:
 * None.
 *
 * O/P:
 * None.
 */
void producer()
{
	//Create arrays
  int n =10000;
  float timeArrayProducer[n];
  int elementsInBufferforinsert[n];

  //File Operation
  FILE * fp;
//  FILE * fa;
  fp = fopen("producerTimeArray.txt", "w");
 // fa = fopen("producerItemsInSharedRegion.txt", "w");

  buffer_type item;

 //insert 10,000 items
 for(int i=0; i<10000;i++)
 {
  	start=clock();

  	//Decrement semaphores
	sem_wait(&empty);//locks empty
  	sem_wait(&mutex);

  	count =0;

  	if(count!=buffer_size)
  	{
    buffer[in] = 0xFF;
    in=(in+1) %buffer_size;
    elementsInBufferforinsert[i] = in;
    count++;
	}

	//Increments semaphores
  	sem_post(&full);	//unlocks full
	sem_post(&mutex);	//unlocks mutex

  	end=clock();
	micros= (double)end-start;
	timeTaken=(double) micros/CLOCKS_PER_SEC;		//total time between first and second clock() command
	timeArrayProducer[i] = timeTaken; 				//store time in array

  }   //END LOOP


  //Output time array TO FILE
  for(int i=0; i<10000; i++)
  {
      fprintf (fp, "%f\n",timeArrayProducer[i]);
  }


  //ADDS items in buffer before insert to file
   // for(int i=0; i<10000; i++)
   // {
   //    fprintf (fa, "%d\n",elementsInBufferforinsert[i]);
   // }


  fclose(fp);
  //fclose(fa);
  printf("Producer process is done\n");

}


/*
 * conusmer()
 * Creates 2 arrays, and proceeds to remove 10000 items from the buffer and finds the time for each insert operation. The time is stored in one array and the elements
 * in the buffer before the insert is stored in another. Seamphores are used for mutual exclusion and to indicate when the buffer is empty.
 * I/P:
 * None.
 *
 * O/P:
 * None.
 *
 *
 */

void consumer()
{
	//Create arrays
  int n =10000;
  float timeArrayConsumer[n];
  int elementsInBufferforRemove[n];

  //File operations
  FILE * fp;
  // FILE * fa;

  fp = fopen("consumerTimeArray.txt", "w");
  //fa = fopen("consumerItemsInSharedRegion.txt", "w");


  buffer_type item;

  for(int i=0;i<10000;i++)
 {
 	start=clock();
 	//Decrement semaphores
 	sem_wait(&full);
 	sem_wait(&mutex);

 	count =buffer_size;
  //fails if buffer is empty
  if(count!=0)
  {
    item = buffer[out];
    out = (out +1 ) % buffer_size;
    elementsInBufferforRemove[i] = out;
    count--;

  }

  //Increment semaphores
  sem_post(&mutex);
  sem_post(&empty);

  end=clock();
  micros= (double)end-start;

  timeTaken=(double) micros/CLOCKS_PER_SEC;		//total time between first and second clock() command
  timeArrayConsumer[i]= timeTaken;

  } //END LOOP


 // Add time array to output text file
  for(int i=0; i<10000; i++)
  {
    fprintf (fp, "%f\n",timeArrayConsumer[i]);
  }

 //Add items in buffer before removal to output text file
  // for(int i=0; i<10000; i++)
  // {
  //   fprintf (fa, "%f\n",elementsInBufferforRemove[i]);
  // }

    fclose(fp);
    // fclose(fa);
    printf("Conusmer process is done\n");

}



/*
 * main()
 * Initlaizes 3 sempahores that are used in both processes(full,empty,mutex). Creates a consumer and producer address using the fork() command and calls the
 * correspnding function.
 *
 * I/P: None.
 *
 *
 * O/P: main {int} - returns a status/exit code to the command line.
 */

int main(){

  //initalize semaphores
  sem_init(&empty, 0, buffer_size);
  sem_init(&full, 0,0);
  sem_init(&mutex,0,1);

  //create 1 producer and 1 consumer process
  int pid;
  pid=fork();

  if(pid<0)
  {
    printf("ERROR:FORK\n");
  } else if(pid == 0)
  {
     consumer();
  }else
    {
      producer();
    }

    return 0;
}
