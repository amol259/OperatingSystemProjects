///////////////////////////////////////////////////////////////////////////////
// Prgm4_Sidhu
// Author: Amol Sidhu (ass140230)
// Date: May 3rd, 2018

// Procedures:
//
// main:          	  Creates the random process execution times and sets the arrival time. Calls each corresponding function.
// fcfs: 		  	      First Come First Serve Scheduling 
//shortestTimeFirst:  Shortest Time First Scheduling
//hrrn:   			      Highest Response Ratio Next Scheduling
//randomNumGenerator: Generates a random number between 1-50


///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>


 void fcfs(int,int [],int []);				
 void turn_around(int, int [], int []);		
 void shortestTimeFirst(int,int[], int[]);
 void hrrn(int, int[], int[]);
 void roundrobin(int, int[], int[]);
/*
 * main
 * Creates the random process execution times and sets the arrival time. Then it calls
 *		each of the algorithims and runs 1000 simulations.
 *
 * I/P:None
 * 
 * O/P:
 * status code 
 */
int main()
{
	int processExecutionTime[1000],arrivalTime[1000];
	int i,value,size;
	size=1000; // Number of processes

	//populates the different execution Time or ser
	for(i=0;i<1000;i++){
		value = randomNumGenerator();
		processExecutionTime[i] =value;
	}

	//sets arrival time array from 0-999
	for(i=0; i<1000;i++){
		arrivalTime[i] = i;
	}

	for(i=0;i<1000;i++){
  fcfs(size,arrivalTime,processExecutionTime);
	roundRobin(size,arrivalTime,processExecutionTime);
	shortestTimeFirst(size,arrivalTime,processExecutionTime);
  hrrn(size,arrivalTime,processExecutionTime);
	}

	printf("Program Complete\n");
}
/*
 * fcfs
 * First Come First Serve Scheduling Algorithim. 
 *
 * I/P: Size of the processes, the arrival time array, and the execution time array.
 * 
 * O/P:
 * None
 */
void fcfs(int size,int arrivalTime[],int processExecutionTime[])
{
	int i, j, result;
	double turnAroundTimeSum=0, relativeTurnAroundTimeSum=0;
	double  waitTime[size],turn_around_time[size],relative_turn_around_time[size];
	double avg_turn_around_time, avg_relative_turn_around_time;

	//find waiting time
	for(i=1; i<size; ++i) {
		result=0;
		for(j=0; j<i; ++j)
		{	
			result= result + processExecutionTime[j];
		}
		waitTime[i]=result - arrivalTime[i];	
	}

	//Calculate turnaround time 

	for(i=0; i<size; ++i){	
		turn_around_time[i]= processExecutionTime[i] + waitTime[i];	
	}

	//calculate relative turnaround time

	for(i=0; i<size;i++){
		relative_turn_around_time[i] = turn_around_time[i]/processExecutionTime[i];
		//printf("%f\n", relative_turn_around_time[i] );
	}

	//Get the sum of the turnaround times and the relative turnaround time
	for(i=0; i<size;i++){
		turnAroundTimeSum = turnAroundTimeSum + turn_around_time[i];
	}
	
		for(i=0; i<size;i++){
		relativeTurnAroundTimeSum = relativeTurnAroundTimeSum + relative_turn_around_time[i];
	}
	//Calculate the average turnaround time
	avg_turn_around_time = turnAroundTimeSum / (double) size;

	avg_relative_turn_around_time = relativeTurnAroundTimeSum/ (double) size;
	
	//Calculate the average relative turnaround time	
  //	avg_relative_turn_around_time = (double) relativeTurnAroundTimeSum/ (double) size;
      printf("FCFS Completed\n");

}

/*
 * roundrobin
 * Round Robin Scheduling Algorithim. 
 *
 * I/P: Size of the processes, the arrival time array, and the execution time array.
 * 
 * O/P:
 * None
 */

int roundRobin(int size, int arrivalTime[], int processExecutionTime[]){
  int remainingTime[size], waitTime[size],turnAroundTime[size],relativeTurnAroundTime[size];
  int turnAroundTimeSum,relativeTurnAroundTimeSum,serviceTime =0;
  double averageTurnaroundTime, averageRelativeTurnaroundTime;
  bool completed = false;


  //Make a copy of the execution time array to store the remainaing time

  for(int i=0;i<size;i++){
    remainingTime[i]=processExecutionTime[i];
  }

  //loop through all processes
  while(!completed){
    for(int i=0;i<size;i++){
      if(remainingTime[i]>0){
        completed = false; //pending processes

        if(remainingTime[i]>1){
          serviceTime++;
          remainingTime[i]--;
        }
        // last cycle 
        else{
          serviceTime = serviceTime + remainingTime[i];

          //wait time calculation
          waitTime[i] = serviceTime - processExecutionTime[i];
          remainingTime[i]=0;
        }
      }
      completed = true;
     
    }
  }

   
  for(int i=0;i<size;i++){
    turnAroundTime[i]= waitTime[i] + processExecutionTime[i];
  }

  for(int i=0;i<size;i++){
    relativeTurnAroundTime[i] = turnAroundTime[i]/ processExecutionTime[i];
  }

  turnAroundTimeSum=0;
  relativeTurnAroundTimeSum=0;

  //Sum of turnaround times and relative turnaround time
  for (int i=0;i<size;i++){
    turnAroundTimeSum = turnAroundTimeSum + turnAroundTime[i];
    relativeTurnAroundTimeSum = relativeTurnAroundTimeSum + relativeTurnAroundTime[i];
  }
  //Average turnaround time
  averageTurnaroundTime = (double)turnAroundTimeSum/(double)size;

  //Average relative turnaround time
  averageRelativeTurnaroundTime = (double)relativeTurnAroundTimeSum/(double)size;
    printf("Round Robin Completed\n");

  return 0;
}


/*
 * shortestTimeFirst
 * Shortest Time First Scheduling Algorithim. 
 *
 * I/P: Size of the processes, the arrival time array, and the execution time array.
 * 
 * O/P:
 * None
 */
void shortestTimeFirst(int size, int arrivalTime[],int processExecutionTime[]) 
{
	int waitTime[size], turnaroundTime[size],relativeTurnaroundTime[size], totalTurnaroundTime = 0;
	int remainingTime[size],totalRelativeTurnaroundTime =0;
	int processCompleted = 0, time = 0, minNumber = 5000;
	int place = 0, totalServiceTime;
  double averageTurnaroundTime,averageRelativeTurnaroundTime;

	for (int i = 0; i < size; i++){
		remainingTime[i] = processExecutionTime[i];
  }

	while (processCompleted != size) {

		// sort by shortest remaining time
		for (int z = 0; z < size; z++) {
			if (processExecutionTime[z] <= time){
        if(remainingTime[z]>0){
            if (remainingTime[z] < minNumber) {
    				  minNumber = remainingTime[z]; 
    				  place = z;	// store location 
    			  }
        }
      }
		}

		remainingTime[place] = remainingTime[place] - 1 ;

		minNumber = remainingTime[place];
		if (minNumber == 0)
			minNumber = 5000;//set to an arbitraliy large value

		//After a process is executed 
		if (remainingTime[place] == 0) {
			processCompleted++;
			totalServiceTime = time + 1;
			// Calculate waiting time 
			waitTime[place] =  totalServiceTime - processExecutionTime[place] - arrivalTime[place];
			
		}
		// Increment time
		time++;

	}

		//calculate turnaround time
		for (int i = 0; i < size; i++){
		turnaroundTime[i] = processExecutionTime[i] + waitTime[i];
		relativeTurnaroundTime[i] = turnaroundTime[i]/processExecutionTime[i];
		}

	// Calculate total turnaround and relative turnaround times
	for (int i = 0; i < size; i++) {
		totalTurnaroundTime = totalTurnaroundTime + turnaroundTime[i];
		totalRelativeTurnaroundTime= totalRelativeTurnaroundTime + relativeTurnaroundTime[i];
	}

	//Average turnaround time
  averageTurnaroundTime = (double)totalTurnaroundTime / (double)size;
	//printf(" %f\n",(double)totalTurnaroundTime / (double)size );

	//Average relative turnaround time
  averageRelativeTurnaroundTime = (double)totalRelativeTurnaroundTime / (double)size ;
	//printf("%f\n",(double)totalRelativeTurnaroundTime / (double)size );
    printf("SRT Completed\n");

}

/*
 * hrrn
 * Highest Response Ratio Next Scheduling Algorithim. 
 *
 * I/P: Size of the processes, the arrival time array, and the execution time array.
 * 
 * O/P:
 * None
 */

void hrrn(int size, int arrivalTime[],int processExecutionTime[] )
{
  int i, j, z, time, expectedServiceTime, executionTime_Sum = 0,place;
  int waitTime[size], turnAroundTime[size],relativeTurnAroundTime[size];
  double turnAroundTimeSum = 0, relativeTurnAroundTimeSum = 0, averageTurnaroundTime, averageRelativeTurnaroundTime;
  double responseRatio,hrrn;

  //Get the sum of all execution times
  for (i = 0; i < size; i++) {
      executionTime_Sum += processExecutionTime[i];
  }

  // Sort by arrival times
   for (i = 0; i < size - 1; i++) {
    for (j = i + 1; j < size; j++) {
      if (arrivalTime[i] > arrivalTime[j]) {

        // Swap earlier arrival time processes with the later one
        responseRatio = arrivalTime[i];
        arrivalTime[i] = arrivalTime[j];
        arrivalTime[j] = responseRatio;
      }
    }
  }

  time = arrivalTime[0]; // set time to equal arrival time of first process

  while(time < executionTime_Sum) {

    // Set some lower limit 
    hrrn = -9999;
    for (i = 0; i < size; i++) {

      if (arrivalTime[i] <= time ) {

        // Calculate Response Ratio
        expectedServiceTime = time - arrivalTime[i];
        responseRatio = (processExecutionTime[i] + expectedServiceTime) / processExecutionTime[i];

        //if the ratio is less than the current ratio, update the ratio
        if (hrrn < responseRatio) {
          hrrn = responseRatio;
          place = i;
        }
      }
    }

    time = time + processExecutionTime[place]; //add execution time to current time value

    // Calculation of Turn Around Time
    turnAroundTime[place] = time - arrivalTime[place];

  
 	 //Calculation of Relative turnAoundTime
  	relativeTurnAroundTime[place] = turnAroundTime[place]/processExecutionTime[place];
  	
  }
  // Sum Turn Around Time 
    for(z=0;z<size;z++){
    turnAroundTimeSum += turnAroundTime[z];
    }

    //sum relative turnaround time
    for(i=0;i<size;i++){
      relativeTurnAroundTimeSum += relativeTurnAroundTime[i];
    }
    
    //Average turnaround time
    averageTurnaroundTime = (double)turnAroundTimeSum /(double) size;
  //printf("%f\n", (double)turnAroundTimeSum /(double) size);

  //Average realtive turaround time
    averageRelativeTurnaroundTime = (double) relativeTurnAroundTimeSum / (double)size;
  //printf("%f\n",(double) relativeTurnAroundTimeSum / (double)size);
    printf("HRRN Completed\n");

}

/*
 * randomNumGenerator
 * Generates a random number between 1-50
 *	
 * I/P: 
 * None
 * O/P:
 * Value = random number between 1-50
 */


int randomNumGenerator(){
      int min=1;
      int max=50;
      int value = min+ (rand() % max); 
      return value;
}