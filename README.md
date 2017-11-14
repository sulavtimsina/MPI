# MPI
Author: Sulav Timsina
        sulav.timsina@gmail.com
        Arkansas State University
        
This repository contains mainly 2 files, collect.c and p2p.c.
It is a mini project where I have 4 processors on a virtual machine and I use point-to-point communication(in p2p.c) and collective operation(in collect.c) to achieve the following requirements:

o	Use totally 4 processes on 4 virtual servers
mpiexec --map-by node --mca oob_tcp_if_include eth1 –n 4 ./MYPROG LENGTH
-	Parameter LENGTH indicates the string length (it could be any number).
-	Inside the program, randomly generate the string (only upper-case letters).
-	Print out the whole string on the screen.
-	Process 0 distributes substrings to the other three and let them sort the substring.
-	Process 3 might receiver a shorter substring
-	Process 0 collects all sorted substrings and merges them for a completely sorted string.
-	Print out the sorted string on the screen.

So Process 0 generates a random string, divides it into 3 substrings and send/scatters to other 3 processors. They sort the substrings and send it back to P0. P0 concatenates it, sorts them again(not done efficiently) and prints it in screen.

The use of point to point and collective communication is shown in the code and not how effective sorting methodology can be done.

Compile collect.c using the following command:
<b>mpicc collect.c -o collect -lm</b>

Run the executible collect using the following command:
mpiexec -iface eth1 -n 4 -host 192.168.100.235,192.168.100.228,192.168.100.236,192.168.100.230 ./collect 100
(where 100 is the length of random string)

Compile p2p.c using the following command:
mpicc p2p.c -o p2p -lm

Run the executible p2p using the following command:
mpiexec -iface eth1 -n 4 -host 192.168.100.235,192.168.100.228,192.168.100.236,192.168.100.230 ./p2p 100
(where 100 is the length of random string)

I have kept the reference to the code which I have taken from internet.
