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
#include <assert.h>
#include "bank.c"



struct Client; //Contains client number and balance

struct Client *addclient(int newnumber, double newbalance); // makes new cliet with given client number and balance

void deposit(struct Client *m, double money); // adds given amount of money to the client account

void withdraw(struct Client *m, double money); // withdraws given amount of money for the clients account

void transfer(struct Client *from, struct Client *to, double money); //transfers given amount of from account from to account to

void balance(struct Client *m); // prints clients balance

double balance_bank(struct Client **array); //returns banks balance

struct Client *find_client(struct Client **array,int clientnum); //returns client with given clientnum

int number_of_clients(struct Client **array); //returns number of clients 

void freeallclients(struct Client **array); // frees memory of all clients

int isClient(struct Client **array,int clientnum);// returns 1 if array contains client with given client number else 0

// serves customer according to command given in input string, returns balance of the bank after command has been processed
double serve_customer(char input[256],struct Client **array,double bank_Balance);

int Log (char *message, char *filename);//writes fiven message to the logfile which name is given

int timetolog(char *filename);//timeforlog writes time at the moment to the logfile which name is given as a parameter.

