///////////////////////////////////////////////////////////////////////////////
// Prgm3_Sidhu
// Author: Amol Sidhu (ass140230)
// Date: April 15th, 2018

// Procedures:
//
// main:          Creates the page address stream array, and runs the simulation on all 3 algorithims
//CLOCKPageFault: Calculates the number of page faults using clock algorithim
//FIFOPageFault:  Calculates the number of page faults using fifo algorithim
//lruPageFault:   Calculates the number of page faults using lru algorithim
//randomNumGenerator: Outputs a random number between 1-50


///////////////////////////////////////////////////////////////////////////////

#include<stdio.h>


/*
 * FIFOPageFault
 * FIFO Page Replacement Algorithim
 *
 * I/P:
 * int pages[] = the apge address stream
 * int numPages = the number of pages 
 * int frameSize = the number of frames to be allocated
 *
 * O/P:
 * int pageFault 
 */

int FIFOPageFault(int pages[], int numPages, int frameSize){
      int i,k,j=0;
      int frame[frameSize],available,pageFault=0;
            
      
      //Forms a queue to hold all the pages                      
      for(i=0;i<=numPages;i++)
      {
             available=0;
     
            for(k=0;k<frameSize;k++){
                  if(frame[k]==pages[i]){
                  available=1;
                }
            }
            //FIFO
            if (available==0){
                frame[j]=pages[i];
                j=(j+1)%frameSize; 
                pageFault++;
            }
      }

      return pageFault;
}


/*
 * CLOCKPageFaults
 * Clock Page Replacement algorithim
 *
 * I/P:
 * pages[] = page address stream
 * numPages = the total number of pages
 * frameSize = the number of frames to be allocated
 *
 * O/P:
 * int pageFaults
 */



int CLOCKPageFault(int pages[],int numPages,int frameSize) 
{
  int i, j, available=0, pageFault=0;
  int frame[frameSize],usebit[4000];  //use bit tracks how often a page is accessed

 
 for(i=0;i<numPages;i++)
 {      
    for(j=0;j<frameSize;j++){
     if(pages[i]==frame[j])
     {
      //set bit when a page is referenced
      available=1;
      pageFault++;
      usebit[j]=1;
     }
    }
      //reset used bit
    if(available==0){
     for(j=0;j<frameSize;j++){
        if(usebit[j]==0){
          frame[j]=pages[i];
          usebit[j]=1;
          available=1;
        }
      }
    }
 
 } 
 return pageFault;
} 




/*
 * lruPageFaults
 * LRU Page Replacement algorithim
 *
 * I/P:
 * pages[] = page address stream
 * numPages = the total number of pages
 * frameSize = the number of frames to be allocated
 *
 * O/P:
 * int pageFaults
 */

int lruPageFault(int pages[], int numPages, int frameSize){
     //initalize all variables
      int i,j,z,k=0,a=0;
      int d,temp,pageFault=0;
      int b[frameSize],a1[frameSize],q[frameSize];

      //initlaize q with the first value of the stream and increment counters
      
      q[k]=pages[k];
      pageFault++;
      k++;

      for(i=0;i<numPages;i++) 
      {
            for(j=0;j<frameSize;j++){
                  if(pages[i]!=q[j])
                  {
                   a++;
                  }
            }

            //if a is at frameSize increment pageFault
            if(a==frameSize)
            {
                  pageFault++;

                  if(k<frameSize){
                        q[k]=pages[i];
                        k++;
                  }
                  
                  else //k>frameSize
                  {
                        for(z=0;z<frameSize;z++) {
                              a1[z]=0;
                              
                              for(j=i-1;j<numPages;j--)
                              {
                                    if(q[z]!=pages[j]){
                                    a1[z]++; }

                              }  
                        }
                          //copy array 
                        for(z=0;z<frameSize;z++){
                              b[z]=a1[z];
                        }
                          //compare pages
                        for(z=0;z<frameSize;z++){
                              for(j=z;j<frameSize;j++){

                                    if(b[z]<b[j])
                                    {
                                          temp=b[z];
                                          b[z]=b[j];
                                          b[j]=temp;
                                    }
                              }

                        }
                          //move forward
                        for(z=0;z<frameSize;z++)
                        {
                                    if(a1[z]==b[0])
                                    q[z]=pages[i];          
                        }
                
                  }
            }
      }
      return pageFault;
}




/*
 * randomNumGenerator()
 * Generates a random number between 1-50 
 *
 * I/P:
 * none
 *
 * O/P:
 * int value = the random number generated
 */

int randomNumGenerator(){
      int min=1;
      int max=50;
      int value = min+ (rand() % max); 
      return value;
}
/*
 * main
 * Creates the page address stream and executes all 3 page replacement algorithims. Simulates all 3 algorithims 1000 times, and frameSize is changed
 * manually in the program.
 *
 * I/P:none
 * 
 *
 * O/P:
 * int Status code
 */


int main(){
      int addressStream[4000]; //Page address stream
      int arraySize = 4000;   //number of pages
      int frameSize = 2;      //Number of frames to be allocated
      int value,i;



      //add values from 1-50 to addressStream
      for(i=0; i<4000;i++){
            value = randomNumGenerator();
            addressStream[i]=value;
      }
      
      //1000 simulations for each algorithim
      for(i=0; i<1000;i++){
      lruPageFault(addressStream, arraySize, frameSize);
      FIFOPageFault(addressStream,arraySize,frameSize);  
      CLOCKPageFault(addressStream,arraySize,frameSize);  
      }


      printf("\nProgram Completed\n");

  return 0;

}













