/* Author: Sulav Timsina
  Email:  sulav.timsina@gmail.com
  Course:   Parallel Processing CS 6213
  Arkansas State University
  Department of Computer Science
  2017
  Code: Collective communication in MPI
  Description: There are 4 processors in my world;P0,P1,P2 and P3. P0 is root processor.
  P0 generates a random string of length specified by used during runtime. The random 
  string is divided into 3 equal parts (or if not possible, P3 gets smaller sub string) and
  scattered to P1, P2 and P3 using MPI_Scatterv and MPI_Gatherv functions. The P1,P2 and P3 sorts the substrings
  and send back to P0. P0 after receiving it concatenates the 3 substringgs and sorts them again.
  It is a demonstration of how MPI_Scatterv and  MPI_Gatherv commands work in MPI and not about how sorting algorithm 
  can be made efficient.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

/* Function for sorting the string*/
/* Function Referenced from https://www.tutorialspoint.com/learn_c_by_examples/program_to_sort_string_characters_in_c.htm with modification */
void stringSort(char arr[])
{
  char temp;
  int i, j, length;
  length=strlen(arr);
  for(i=0; i<length-1; i++)  {
  	for (j=i+1; j<length; j++)  	{
  	   if (arr[i] > arr[j])  	 {
  		 temp=arr[i];
  		 arr[i]=arr[j];
  		 arr[j]=temp;
  	   }
  	}
  }
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);      // Initialize the MPI environment
  int randStrLength, worldRank, worldSize ;  
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);  
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  randStrLength = atoi(argv[1]);
  char randStr[randStrLength+1];
  
  //random string generation part start  
      srand(time(NULL));
      int i=0;      
      while(i < randStrLength) {
      		randStr[i] = 'A' + (rand() % 26) ;
      		i++;
       }
       randStr[i] = '\0';
       if(worldRank == 0)
         printf("\n\nRandom String:\n %s\n",randStr);
  //random string generation part start 
  
  int length1,length2,length3;
  length1 = (int) ceil((float) randStrLength/ (worldSize - 1));  //length of substring for P1
  length2 = length1;                                              //length of substring for P2
  length3 = randStrLength - length1 - length2;                    //length of substring for P3        
    
  int disp[] = {0,0,length1,length1+length2};                     //displacement vector
  int sendCount[] = {0,length1,length2,length3};                  //send count vector for scatterv
  char *subStr = (char *)malloc(sizeof(char) * randStrLength);
 
  MPI_Scatterv(randStr, sendCount, disp, MPI_CHAR, subStr, length1, MPI_CHAR, 0, MPI_COMM_WORLD);
  
  subStr[sendCount[worldRank]]='\0';                             //terminating each received substr with null character         
  stringSort(subStr);                                            // Sort the subset of char array  

  char *recvbuf = NULL;  
  if (worldRank == 0)   {    
    recvbuf = malloc(sizeof(char) * randStrLength);    
  }  
  int sendCountGather = strlen(subStr);                         //for each proc. send_count is equal to the length of their substring
  MPI_Gatherv(subStr, sendCountGather, MPI_CHAR, recvbuf, sendCount,disp,MPI_CHAR, 0, MPI_COMM_WORLD); 
      
  if (worldRank == 0) {  
    recvbuf[randStrLength]='\0';      
    stringSort(recvbuf);
    printf("\nFinal Sorted String:\n %s\n", recvbuf);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}