# BankApp
C programming project that simulates simplified bank with using concepts like socket and thread programming.

# Info
Application simulates bank that contains N service desks. Each service desk run it's own thread.

Each service desk has own queue from whch it waits commands. When the user issues a command, it goes to the queue of service desk(thread) that has shortest queue.

The commands could be for example:
'l 2' to get balance of the client with client number 2.
‘w 3 20' to withdraw 20€ from client with client number 3.
't 2 3 100' to transfer 100€ from client with client number 2 to client with client number 3.
'd 2 200€' to deposit money to client with client number 2.
'p' to print all clients
'e' to end program
'b' to get bank balance
'h' to get all commands




Program 

Program also has master thread that can 
