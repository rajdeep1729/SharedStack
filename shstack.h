#ifndef _MYLIB_H_
#define _MYLIB_H_
#include <sys/ipc.h> //shmget(2) shmctl(2)
#include <sys/shm.h>//shmget(2) shmat(2) shmdt(2) shmctl(2)
#include <sys/types.h>//shmat(2) shmdt(2)
#include <stdio.h>
#include <sys/sem.h> // for semget(2) semop(2) semctl(2)

#define Max_Stacks 10

int semid,sem_status;//semaphore variables

key_t mykey;//key for creating the shared stack descriptor

int *pi;//pointer to the shared stack descriptor



#define NO_SEM  1

#define P(s) semop(s, &Pop, 1);//P operation of semaphore
#define V(s) semop(s, &Vop, 1);//V operation of semaphore

struct sembuf Pop;
struct sembuf Vop;

typedef struct Stack_descriptor//stack descriptor data structure
{
 key_t key;//value of the key
 int element_size;//size of the element 
 int stack_size;//size of the stack
 int free;//stack descriptor is free or not
 int no_of_elements;//keeping track of no.of elements
 int shared_id;//id for attaching shared memory to own process space
}stackdesc;


int shstackget(int key, int element_size, int stack_size);//function to get handle of the stack
void shstackpush(int index,void* val);//function for pushing into stack
void* shstackpop(int index);//function for popping from the stack
void shstackrm(int index);//function for removing the stack and shared memory for it
void shstackdisplay(int index);//function for displaying the stack
void semaphore_init();//function for initializing the semaphore

#endif