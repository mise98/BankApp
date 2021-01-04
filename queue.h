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
#include "queue.c"



struct reg; // contains reguest of client and pointer to the next reguest in line

struct Clientqueue; //contains first and last member of queue struct

struct Clientqueue *newqueue(void); //makes new queue struct and allocs memory for it

void enqueue(struct Clientqueue *q, char newreguest[256]); //adds new reguest to the queue struct that is given as a parameter

void dequeue(struct Clientqueue *q); //Removes first reguest from the queue

void freequeues(struct Clientqueue **queuelist); //free memory of the queuelist

int queue_lenght(struct Clientqueue *q); //returns lenght of  clientqueue given as a parameter

int shortest_queue(struct Clientqueue **array); //returns shortest queue from Clientqueue array

