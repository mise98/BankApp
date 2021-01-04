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
#include <netinet/in.h> 
#include "bank.h"
#include "queue.h"
#include <pthread.h> 
#include <assert.h>

#define MAXTHREADS 8 //defines how many service desks(threads) will be created

pthread_rwlock_t lock_rw = PTHREAD_RWLOCK_INITIALIZER; //defines R/W lock


pthread_barrier_t barrier; //defines barrier

int check=0; 
int run = 1;




double bank_balance; // defines banks balance

struct Clientqueue **queuelist; // defines list of queues

struct Client **array; // defines list of clients

pthread_mutex_t lock; // defines mutex

char logfile[100]= "log.txt"; //Defines logfile name

/*Function for the threads*/
void *work(void *args) { 



int *argptr = (int*) args;
int n = *argptr; // converting thread parameter to int

assert(pthread_mutex_lock(&lock)==0); // creating mutex lock

queuelist[n] =newqueue(); // creting new queue for the thread
queuelist =realloc(queuelist, sizeof(struct Clientqueue*)*((n + 2)+1)); //allocing memory for the list of queues
queuelist[n+1]=NULL;
printf("Service desk number %d is ready\n",n+1);

assert(pthread_mutex_unlock(&lock)==0); //unlocks mutex lock

while(run){
assert(pthread_mutex_lock(&lock)==0); // creating mutex lock

if(queuelist[n]->head!=0){ //check if there is commands in queue
	pthread_rwlock_rdlock(&lock_rw);

	Log("Thread serves customer\n",logfile);
    bank_balance=serve_customer(queuelist[n]->head->reguest,array,bank_balance); //serves customer
    dequeue(queuelist[n]); //deletes served customer from the queue

	pthread_rwlock_unlock(&lock_rw);
}
	assert(pthread_mutex_unlock(&lock)==0); //opens mutex lock

if(check==1){ 
	pthread_barrier_wait(&barrier); //waits all threads to sync
	check=0;
}
}

return NULL;
}




int main(){

	timetolog(logfile);//writes time to logfile

	bank_balance=0.0; 

	Log("Creating barrier\n",logfile);
	assert((pthread_barrier_init(&barrier,NULL,MAXTHREADS+1)) == 0);//creates the barrier

 	array=malloc(sizeof(struct Client*)); //allocs memory for the student array
	

	int number_of_clients = 10; //defines number of clients in the bank
	Log("Creating clients\n",logfile);

	for (int i = 1; i <= number_of_clients; i++){  //adds number_of_clients clients to the bank
		array[i-1] = addclient(i,50*i);
		array = realloc(array, sizeof(struct Client*)*((i-1) + 2)+1);
		array[i] = NULL;
		} 
	
	bank_balance=balance_bank(array); 

   char s_message[256]= "Server is working";

	int server_socket; 

	server_socket = socket(AF_INET,SOCK_STREAM,0); // defines server socket

	Log("Creating socket\n",logfile);
	struct sockaddr_in server_address; //defines server adress
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(9002);
	server_address.sin_addr.s_addr=INADDR_ANY;

	bind(server_socket,(struct sockaddr*) &server_address,sizeof(server_address)); //binds connection

	listen(server_socket,5);

	int client_socket;

	Log("Creating connection\n",logfile);
	client_socket =accept(server_socket,NULL,NULL); 

	send(client_socket,s_message,sizeof(s_message),0); // sends message that server is working for the client
	Log("Server is now on\n",logfile);

    queuelist = malloc(sizeof(struct Clientqueue*)+1); //allocs memory for the clientqueue list
 	queuelist[0]=NULL;

    pthread_t tid[MAXTHREADS] = {0}; //defines thread id´s
	Log("Creating threads\n",logfile);

    for (int i = 0; i < MAXTHREADS; i++) { //creates MAXTHREADS number of threads
       assert(pthread_create(&tid[i], NULL, work, &i)==0); 

    usleep(1000*100); //sleeps litle bit just for save
    }

    char input[256]; //defines input string

    while(run){
	Log("Asking commands\n",logfile);
	recv(client_socket, &input, sizeof(input),0); // receives messages from client and writes them to input string
	

	if(input[0]=='b'&&strlen(input)==1){ //check if command is b and master have to check bank balance

	check=1;
	Log("Waiting all threads to report to the master\n",logfile);
	pthread_barrier_wait(&barrier); // master waits all threads to be in same point
	bank_balance = balance_bank(array); // master updates bank balance

	Log("All threads reported\n",logfile);
	printf("Balance of the bank is %0.2lf\n",bank_balance); //masster prints balance
	Log("giving balance of the bank\n",logfile);
	}

	else if(input[0]=='e'&&strlen(input)==1){ // checks if there is command to end the program
		run = 0;
	}


	else{

		assert(pthread_mutex_lock(&lock)==0); //makes mutex lock
		
		int n =shortest_queue(queuelist); // cheks which queue is shortest

		Log("Master thread sends command to the shortest queue\n",logfile);
		printf("Command '%s' has been set to queue number %d\n",input,n+1);

		enqueue(queuelist[n], input); // add new command to the shortest queue

		assert(pthread_mutex_unlock(&lock)==0); //unlocks mutex lock
	}
	}

	close(server_socket); //close socket
	printf("Program ends\n");

	Log("Waiting all threads to end\n",logfile);

	for (int i = 0; i < MAXTHREADS; i++) {//waits all threads to end
		assert((pthread_join(tid[i],NULL)) == 0);
	}
	Log("Destroying barrier\n",logfile);
	assert((pthread_barrier_destroy(&barrier)) == 0);//destroys barrier

	Log("Free list of clients\n",logfile);
	freeallclients(array); // free all client list

	Log("Free list of queues\n",logfile);
	freequeues(queuelist); // free queue list

	Log("Program ends\n",logfile);
	exit(0);
	
	return 0;
    }
