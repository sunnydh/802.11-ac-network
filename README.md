# 802.11-ac-network

* Comprehension of asuswrt-merlin: firmware used by Asus routers
* Studied the impact on transport layer parameters by modifying mac and physical layer parameters
* Involved refactoring of router kernel code and system level programming

## Socket Programming

* Our aim is to measure the TCP performance of 802.11 ac networks, for generating traffic over the network we wrote sockets in C.
* Introductory guide for writing sockets in C language can be found here in [[1]](http://parsys.eecs.uic.edu/~solworth/bgnet.pdf) and [[2]](http://www.codeproject.com/Articles/586000/Networking-and-Socket-programming-tutorial-in-C).
* To measure the TCP related parametes, we used ``` getsockopt() ``` system call, the detailed guide can be found here [[1]] (http://linuxgazette.net/136/pfeiffer.html) and [[2]] (http://linux.die.net/man/2/getsockopt)
