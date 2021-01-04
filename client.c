//
// Created by Mikko Seppi on 20.11.2019.
//
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> 
#include <pthread.h> 
#include <sys/socket.h>
#include <assert.h>
#include <netinet/in.h> 
#include <setjmp.h>
#include "bank.h"

jmp_buf return_here;
 

void sigint_handler(int sig)  //Defines that program will end by signal given as a parameter
{    
    longjmp (return_here, 1);
} 


int main(){


signal(SIGINT, sigint_handler); 

int network_socket; 
network_socket=socket(AF_INET,SOCK_STREAM,0);// defines socket
struct sockaddr_in server_address; // defines server adress
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9002);
server_address.sin_addr.s_addr=INADDR_ANY;

int connection_status = connect(network_socket,(struct sockaddr*)&server_address, sizeof(server_address)); // makes connection to server

if(connection_status==-1){  // checks if connection succeed or failed
    printf("error in socket\n");
}
else{

    char res[256];
    recv(network_socket, &res, sizeof(res),0); // receive message from server
    printf("%s\n",res);

    char input[256];// defines input string
    int run =1;
    printf("'l clientnum' to get client balance\n'w clientnum amount' to withdraw money\n't from to amount' to transfer money\n'd clientnum amount' to deposit money\n'p' to print all clients\n'e' to end program\n'b' to get bank balance\n'h' to get all commands\n");

while(run){

printf("What do you want to do?\n");
 if (setjmp (return_here)) { //jumps here if SIGINT signal is receive
    char end[256]="e"; 
    send(network_socket, end , sizeof(end), 0); // sends command to close server
    run = 0;
    signal (SIGINT, sigint_handler);
    }  
 else{   
	fgets(input, sizeof input, stdin); // asks commands

	if(input[strlen(input)-1]=='\n'){//deletes newline from input string
        input[strlen(input)-1]='\0';
    }

   if(input[0]=='h'&&strlen(input)==1){
		help();
	}
    
    else{
        send(network_socket, input , sizeof(input), 0); //sends command for the server

    if(input[0]=='e'&&strlen(input)==1){ //ends program if comand was "e"
       run = 0;
    }
    }
    }
}
}

printf("Program ends\n");
close(network_socket); // closes socket
return 0;

}


