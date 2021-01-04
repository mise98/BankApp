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
  


struct Client { //Contains client number and balance
	int client_number;
    double balance;
};



struct Client *addclient(int newnumber, double newbalance){ // makes new cliet with given client number and balance
	struct Client *new = malloc(sizeof(struct Client)+1);
	new->client_number= newnumber;
    new->balance = newbalance;
	return new;
}



void deposit(struct Client *m, double money){ //adds given amount of money to the client account
printf("%0.2lf euros was deposit to account number %d\n",money, m->client_number);
m->balance += money;
}



void withdraw(struct Client *m, double money){ //withdraws given amount of money for the clients account
printf("%0.2lf euros was withdraw from account number %d\n",money, m->client_number);
m->balance -= money;
}



void transfer(struct Client *from, struct Client *to, double money){ //transfers given amount of from account from to account to
if(from->balance>= money){
    from->balance -= money;
    to->balance += money;
    printf("%0.2lf euros was transfered from client %d to %d\n", money, from->client_number,to->client_number);
    }
else{
    printf("Client %d have not enough money to transfer\n",from->client_number);
    }
}



void balance(struct Client *m){  //prints clients balance
    printf("%d balance is %0.2lf euros\n",m->client_number,m->balance);
}




double balance_bank(struct Client **array){ //returns banks balance
    int i = 0;
    double money=0;
while(array[i]!=NULL){
    money+=array[i]->balance;
i++;
}
return money;
}




struct Client *find_client(struct Client **array,int clientnum){ //returns client with given clientnum
    int i = 0;
    while(array[i]!=NULL){
        if(array[i]->client_number==clientnum){
            return array[i];
        }
        i++;
    }
    return NULL;
}



int number_of_clients(struct Client **array){ //returns number of clients 
    int i=0;
      while(array[i]!=NULL){
        
        i++;
    }
    return i;
}



void freeallclients(struct Client **array) { // frees memory of all clients
    int i = 0;
    while(array[i]!=NULL){
        free(array[i]);
        i++;
    }
	free(array);
}



int isClient(struct Client **array,int clientnum){// returns 1 if array contains client with given client number else 0
    int i=0;
    int is=0;
    while(array[i]!=NULL){
        if(array[i]->client_number==clientnum){
            is = 1;
        }
        i++;
    }
    return is;
}



// serves customer according to command given in input string, returns balance of the bank after command has been processed
// checks also if command is valid
double serve_customer(char input[256],struct Client **array,double bank_Balance){
     if(input[0]=='l'){
        char com[10];
        int cus;
            if(sscanf(input,"%c %d",com, &cus)==2){
                if(isClient(array,cus)){
                    balance(find_client(array,cus));
                return  bank_Balance;
                }
                else{
                     printf("Incorrect client number\n");
                     return  bank_Balance;
                }
            }
            else
            {
                printf("Incorrect command\n");
                return  bank_Balance;
             }
            }
    else if(input[0]=='w'){
    
        char com[10];
        int cus;
        double money;
        if(sscanf(input,"%c %d %lf",com, &cus, &money)!=3){
            printf("Incorrect command\n");
            return bank_Balance;
        }
        else{
        if(isClient(array,cus)){
            if(money>=0 && find_client(array,cus)->balance>=money){
                withdraw(find_client(array,cus),money);
                bank_Balance -=money;
                return bank_Balance;
            }
            else{
                if(money<0){
                printf("You can't withdraw negative values\n");
                return bank_Balance;
                }
                else{
                printf("Client %d have not enough money to withdraw\n",cus);
                return bank_Balance;
                }
            }
        }
        else{
            printf("Incorrect client number\n");
            return  bank_Balance;
        }
        }
        }

    else if(input[0]=='d'){
        char com[10];
        int cus;
        double money;
       if(sscanf(input,"%c %d %lf",com, &cus, &money)!=3){
            printf("Incorrect command\n");
            return bank_Balance;
        }
        else{
        if(isClient(array,cus)){
            if(money>=0){
                deposit(find_client(array,cus),money);
                bank_Balance+= money;
                return bank_Balance;
            }
            else{
                 printf("You can't deposit negative values\n");
                 return bank_Balance;
            }
        }
        else{
             printf("Incorrect client number\n");
             return  bank_Balance;
        
        }
        }
    }
    else if(input[0]=='t'){
        char com[10];
        int from;
        int to;
        double money;
        if(sscanf(input,"%c %d %d %lf",com, &from,&to, &money)!=4){
            printf("Incorrect command\n");
        return bank_Balance;
        }
        else{
            if(isClient(array,from)&&isClient(array,to)){
                if(money>=0){
                    transfer(find_client(array,from),find_client(array,to),money);
                    return bank_Balance;
                }
                else{
                    printf("You can't transfer negative values\n");
                    return  bank_Balance;
            }
            }
            else{
                printf("Incorrect client numbers\n");
                return  bank_Balance;
            }
        }
    }
    else if(input[0]=='p'&&strlen(input)==1){
        int i = 0;
        while(array[i]!=NULL){
           balance(array[i]);
            i++;
        }
        return  bank_Balance;
    }
    else{
        printf("Incorrect command\n");
        return bank_Balance;
    }
}

void help(void){ //prints all commands
    printf("'l clientnum' to get client balance\n'w clientnum amount' to withdraw money\n't from to amount' to transfer money\n'd clientnum amount' to deposit money\n'p' to print all clients\n'e' to end program\n'b' to get bank balance\n'h' to get all commands\n");
}


int Log (char *message, char *filename) //writes given message to the logfile which name is given
{
FILE *fileAddress;
   fileAddress = fopen(filename, "a+");
   if (fileAddress != NULL) {
	
	   fprintf( fileAddress,"%s", message);
   
	fclose(fileAddress);
    return 1;		
   }
   else {
    return 0;
   } 
	
}



/*
timeforlog writes time at the moment to the logfile which name is given as a parameter.
*/
int timetolog(char *filename){
	FILE *fileAddress;
   fileAddress = fopen(filename, "a+");
   if (fileAddress != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        Log("-------------------\n",filename);
        fprintf(fileAddress,"%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        fclose(fileAddress);
        return 1;		
   }
   else {
        return 0;
   } 
	
}







 
