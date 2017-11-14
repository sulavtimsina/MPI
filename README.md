# MPI
Implementation of MPI_Send, MPI_Recv
CS 6213   Parallel Processing

Fall 2017

Homework 1

Assignment Date:		Wednesday, October 25, 2017

Due Date:			at 2:00pm,  Monday, November 06, 2017

Submission:                       On machine 147.97.156.236, create a directory ~/6213/hw1  
 				And leave your programs there. Don’t modify them later since 
		the timestamp will be changed (If you really want to modify the
				file, create another file for new version and inform me by email).


Programming Assignment (35 points):    

1.	SSH configuration (5 points):
Follow the SSHpasswd.doc to set up your MPI environment. NO password will be asked for remote access.

2.	MPI programming:  distributed sorting   (30 points)

•	Requirement:
o	Use totally 4 processes on 4 virtual servers
mpiexec --map-by node --mca oob_tcp_if_include eth1 –n 4 ./MYPROG LENGTH
o	Parameter LENGTH indicates the string length (Be careful: it could be any number).
o	Inside the program, randomly generate the string (Let’s use upper-case letters).
o	Print out the whole string on the screen.
o	Process 0 distributes substrings to the other three and let them sort the substring.
o	Process 3 might receiver a shorter substring
o	Process 0 collects all sorted substrings and merges them for a completely sorted string.
o	Print out the sorted string on the screen.
o	Follow the instruction closely. Otherwise, penalty will be applied.

•	Two programs:
o	p2p.c  :  only use point-to-point communication primitives (send/receive)
o	collect.c  :  only use collective operations 
o	You have to leave the executable files p2p and collect on the virtual server. Otherwise, a penalty will be applied. 
 


