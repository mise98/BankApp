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





struct reg { // contains reguest of client and pointer to the next reguest in line
    struct reg *next;
    char reguest[256];
};

struct Clientqueue {  //contains first and last member of queue struct
    struct reg *head;  
    struct reg *tail;  
};

struct Clientqueue * newqueue(void) //makes new queue struct and allocs memory for it
{
    struct Clientqueue *q;

    q = malloc(sizeof(struct Clientqueue));

    q->head = q->tail = 0;

    return q;
}

void enqueue(struct Clientqueue *q, char newreguest[256]) //adds new reguest to the queue struct that is given as a parameter
{
    struct reg *new;

    new = malloc(sizeof(struct reg)+1);
    assert(new);

    strcpy(new->reguest,newreguest);
    new->next = 0;

    if(q->head == 0) {  
        q->head = new;
    } else {  
        q->tail->next = new;
    }
    q->tail = new;
}



void dequeue(struct Clientqueue *q){ //Removes first reguest from the queue

    struct reg *que;

    que = q->head;
    q->head = que->next;

    free(que);
}
void freequeues(struct Clientqueue **queuelist){ //free memory of the queuelist
    int i = 0;
    while (queuelist[i]!=NULL){
        while(queuelist[i]->head!=0){
            dequeue(queuelist[i]);
        }
        free(queuelist[i]);
        i++;
    }
    free(queuelist);
}


int queue_lenght(struct Clientqueue *q){ //returns lenght of clientqueue given as a parameter
    struct reg *cur;
    int lenght=0;
     for(cur = q->head; cur != 0; cur = cur->next) {
        lenght+=1;
    }
    return lenght;
}


int shortest_queue(struct Clientqueue **array) { //returns shortest queue from Clientqueue array
int i=0;
int shortest = 0;
while(array[i]!=NULL){
    
   if(queue_lenght(array[i])<queue_lenght(array[shortest])){
       shortest = i;
   }
   i++;
}
    return shortest;
}
