# BankApp
C programming project that simulates simplified bank with using concepts like socket and thread programming.

# Info
Application simulates bank that contains N service desks. Each service desk run it's own thread.

Each service desk has own queue from which it waits commands. When the user issues a command, it goes to the queue of service desk(thread) that has shortest queue.

Program also has master thread that can skip all queues to get the current balance of the bank.

Application works with two programs which are connected with socket. server.c is the program that contains the bank. It waits messages from client.c where user gives commands on command promt.

The commands could be for example:
'l 2' to get balance of the client with client number 2.
‘w 3 20' to withdraw 20€ from client with client number 3.
't 2 3 100' to transfer 100€ from client with client number 2 to client with client number 3.
'd 2 200' to deposit 200€ to client with client number 2.
'p' to print all clients
'e' to end program
'b' to get bank balance
'h' to get all commands


Main idea of the project is to practice use of the threads and communication between two programs.

