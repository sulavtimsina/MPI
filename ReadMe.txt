Author: Sulav Timsina
        sulav.timsina@gmail.com
        Arkansas State University


Compile collect.c using the following command:
mpicc collect.c -o collect -lm

Run the executible collect using the following command:
mpiexec -iface eth1 -n 4 -host 192.168.100.235,192.168.100.228,192.168.100.236,192.168.100.230 ./collect 100
(where 100 is the length of random string)

Compile p2p.c using the following command:
mpicc p2p.c -o p2p -lm

Run the executible p2p using the following command:
mpiexec -iface eth1 -n 4 -host 192.168.100.235,192.168.100.228,192.168.100.236,192.168.100.230 ./p2p 100
(where 100 is the length of random string)

I have kept the reference to the code which I have taken from internet.