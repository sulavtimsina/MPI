/* Author: Sulav Timsina
  Email:  sulav.timsina@gmail.com
  Course:   Parallel Processing CS 6213
  Arkansas State University
  Department of Computer Science
  2017
  Code: p2p transfer in MPI
  Description: There are 4 processors in my world;P0,P1,P2 and P3. P0 is root processor.
  P0 generates a random string of length specified by used during runtime. The random 
  string is divided into 3 equal parts (or if not possible, P3 gets smaller sub string) and
  send to P1, P2 and P3 using MPI_Send and MPI_Recv functions. The P1,P2 and P3 sorts the substrings
  and send back to P0. P0 after receiving it concatenates the 3 substringgs and sorts them again.
  It is a demonstration of how Send and Recv commands work in MPI and not about how sorting algorithm 
  can be made efficient.
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
  	   if (arr[i] > arr[j])  	   {
  		 temp=arr[i];
  		 arr[i]=arr[j];
  		 arr[j]=temp;
  	   }
  	}
  }
}

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // world_rank = rank of respective processor
    // world_size = number of processors 
    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);    
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    
    //length_rand_str
    int randStrLength, length1,length2,length3;
    
      
    if(worldRank == 0){      
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
  //random string generation part ends
        
        length1 = (int) ceil((float) randStrLength/ (worldSize - 1));  //length of substring for P1
        length2 = length1;                                              //length of substring for P2
        length3 = randStrLength - length1 - length2;                    //length of substring for P3        
        
        char subStr1[length1+1];    // +1 is done to accomodate the last null character which is required 
        char subStr2[length2+1];    // to end the character array
        char subStr3[length3+1];
         
        for(i=0; i<length1; i++){
          subStr1[i] = randStr[i];          
        }
        subStr1[i] = '\0';        
        
        for(i=0; i<length2; i++){
          subStr2[i] = randStr[length1 + i];
        }
        subStr2[i] = '\0';        
              
        for(i=0; i<length3; i++){
          subStr3[i] = randStr[length1 + length2 + i];
        }
        subStr3[i] = '\0';
  
      char *rxStr = (char *)malloc(sizeof(char) * randStrLength);    //allocating size for concatenated string after receiving      
      char *rxBuf01 = (char *)malloc(sizeof(char) * length1);
      char *rxBuf02 = (char *)malloc(sizeof(char) * length2);
      char *rxBuf03 = (char *)malloc(sizeof(char) * length3);
     
       //sending the length of sub string1 to P1 at first, so that after receiving it
       // P1 will be able to allocate a receive buffer of this length
       // tag = 0; destination = 1
       MPI_Send(&length1, 1, MPI_INT, 1, 0,MPI_COMM_WORLD);     
       
       //Sending the actual sub string1 to P1. tag = 1 destination = 1
       MPI_Send(subStr1, length1+1, MPI_CHAR, 1, 1,MPI_COMM_WORLD);
       
       // sending the length info
       // tag = 0; destination = 2
       MPI_Send(&length2, 1, MPI_INT, 2, 0,MPI_COMM_WORLD);
       // sending the actual substring2
       // tag = 1; destination = 2       
       MPI_Send(subStr2, length2+1, MPI_CHAR, 2, 1,MPI_COMM_WORLD);
       
       // sending the length info
       // tag = 0; destination = 3
       MPI_Send(&length3, 1, MPI_INT, 3, 0,MPI_COMM_WORLD);
       // sending the actual substring2
       // tag = 2; destination = 3
       MPI_Send(subStr3, length3+1, MPI_CHAR, 3, 1,MPI_COMM_WORLD);
      
      //Receive function which is expecting the sorted substring
      // tag = 2; destination =1,2 and 3       
      MPI_Recv(rxBuf01, length1, MPI_CHAR, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(rxBuf02, length2, MPI_CHAR, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(rxBuf03, length3, MPI_CHAR, 3, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      for(int i = 0 ; i<length1; i++){
        rxStr[i] = rxBuf01[i];
      }
      
      for(int i = 0 ; i<length2; i++){
        rxStr[i+length1] = rxBuf02[i];
      }
      for(int i = 0 ; i<length3; i++){
        rxStr[i+length1+length2] = rxBuf03[i];
      }
      
      rxStr[randStrLength] = '\0';
      stringSort(rxStr);
      printf("\nFinal Sorted String:\n %s\n", rxStr);
}
   else if(worldRank == 1){
      MPI_Recv(&length1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  //receiving the length info      
      char *rxBuf11 = (char *)malloc(sizeof(char) * length1);
      MPI_Recv(rxBuf11, length1+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  //receiving the actual sub string      
      stringSort(rxBuf11);        //sorting of substring
      MPI_Send(rxBuf11, length1, MPI_CHAR, 0, 2,MPI_COMM_WORLD);       //sending the sorted substring to P0 with tag=2 
   } 
   else if(worldRank == 2){
     MPI_Recv(&length2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    //receiving the length info      
      char *rxBuf22 = (char *)malloc(sizeof(char) * length2);      
      MPI_Recv(rxBuf22, length2+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   //receiving the actual sub string     
      stringSort(rxBuf22);            //sorting of substring
      MPI_Send(rxBuf22, length2, MPI_CHAR, 0, 2,MPI_COMM_WORLD);      //sending the sorted substring to P0 with tag=2
   }
   else if(worldRank == 3){
     MPI_Recv(&length3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    //receiving the length info
     char *rxBuf33 = (char *)malloc(sizeof(char) * length3);      
     MPI_Recv(rxBuf33, length3+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    //receiving the actual sub string     
     stringSort(rxBuf33);           //sorting of substring     
     MPI_Send(rxBuf33, length3, MPI_CHAR, 0, 2,MPI_COMM_WORLD);      //sending the sorted substring to P0 with tag=2
   }   
  MPI_Finalize();      //Finalize the MPI Environment
}
