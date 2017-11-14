# MPI
Implementation of MPI_Send, MPI_Recv

This repository contains mainly 2 files, collect.c and p2p.c.
It is a mini project where I have 4 processors on a virtual machine and I use point-to-point communication(in p2p.c) and collective operation(in collect.c) to achieve the following requirements:

o	Use totally 4 processes on 4 virtual servers
mpiexec --map-by node --mca oob_tcp_if_include eth1 –n 4 ./MYPROG LENGTH
o	Parameter LENGTH indicates the string length (it could be any number).
o	Inside the program, randomly generate the string (only upper-case letters).
o	Print out the whole string on the screen.
o	Process 0 distributes substrings to the other three and let them sort the substring.
o	Process 3 might receiver a shorter substring
o	Process 0 collects all sorted substrings and merges them for a completely sorted string.
o	Print out the sorted string on the screen.

So Process 0 generates a random string, divides it into 3 substrings and send/scatters to other 3 processors. They sort the substrings and send it back to P0. P0 concatenates it, sorts them again(not done efficiently) and prints it in screen.

The use of point to point and collective communication is shown in the code and not how effective sorting methodology can be done.
