#include <sys/ipc.h> //shmget(2) shmctl(2)
#include <sys/shm.h>//shmget(2) shmat(2) shmdt(2) shmctl(2)
#include <sys/types.h>//shmat(2) shmdt(2)
#include <stdio.h>
#include <errno.h>//used for perror
#include <limits.h>//used for INT_MAX
#include <stdlib.h>//used for exit
#include <sys/sem.h> // for semget(2) semop(2) semctl(2)
#include "shstack1.h"//my library for stack functions

void semaphore_init()//function for initializing the semaphore
{
 
 union semun 
 {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
  } setval;

  setval.val=1;//setting the semaphore value to 1

  /*Operation for decrementing the semaphore value*/
  Pop.sem_num = 0;
  Pop.sem_op = -1;
  Pop.sem_flg = SEM_UNDO;

  /*Operation for incrementing the semaphore value*/
  Vop.sem_num = 0;
  Vop.sem_op = 1;
  Vop.sem_flg = SEM_UNDO;

  semid = semget(mykey, NO_SEM, IPC_CREAT | 0777);//getting the semaphore set
  
  if(semid == -1) 
  {
     perror("semget() failed");
     exit(1);
  }

  sem_status = semctl(semid, 0, SETVAL, setval);//setting value of the semaphore to setval.val
 
  if(sem_status == -1) 
   {
      perror("semctl() failed");
      exit(1);
   }



}


int shstackget(int key, int element_size, int stack_size)//function to get handle of the stack
{
	
  int index=-1;
  int flag=0;
  int shmid1;

  mykey=ftok("/home/rajdeep/Desktop/OS/Assg3/matrix.c",1);//defining mykey
  
  /*Creating the stack descriptor shared memory for the first time*/
  shmid1=shmget(mykey,Max_Stacks *sizeof(stackdesc),IPC_CREAT|0777|IPC_EXCL);
  
  if(shmid1!=-1)//the stack descriptor shared memory is getting alloated
  {
    pi=(int *)shmat(shmid1,NULL,0);//attaching the pointer shared stack descriptor memory
    
    /*Setting all the stack descriptors to free*/
    for(int i=3;i<=93;i+=10)
        pi[i]=1;
    
    semaphore_init();//initializing the semaphore  
  }
  
  else//the shared memory has been allocated
  
  {
    
    shmid1=shmget(mykey,Max_Stacks *sizeof(stackdesc),0);
    
    pi=(int *)shmat(shmid1,NULL,0);//attaching the pointer shared stack descriptor memory
  }
 
	for(int i=0;i<=90;i+=10)
     {
       if(pi[i]==key)//if already stack with key exists
       {
        
          flag=1;
          index=i/10;
          break;

       }
       
     }


  if(flag==0)//no stack corresponding to key exists
    {
     	for(int i=3;i<=93;i+=10)//checking the keys stored in stack descriptor
        {
     		 
         if(pi[i]==1)//getting first free stack descriptor
            {
       	      
              index=i/10;
       	      pi[i]=0;//setting the stack descriptor as occupied
       	      break;
            }
     	  }

    }

  if(index==-1)//no free space for creating any more stack 
    {
       
       printf("No free space\n");
       exit(1);
    }

     
  if(!flag)//no stack has been created previously
    {
     		    
        pi[index*10]=key;//setting the key value
        pi[index*10+1]=element_size;//setting the element size
        pi[index*10+2]=stack_size;//setting the stack size
        pi[index*10+4]=-1;//setting the stack top
        /*creating new shared memory for the corresponding stack*/
        pi[index*10+5]=shmget(key,stack_size *sizeof(int),IPC_CREAT|0777);
        
    }
  
	return index;//handle for indexth stack descriptor
}


void shstackpush(int index,int value) 
{

	int *spi;
  
  if(pi[index*10+3]==1)//some other process removed this shared stack
  {

    fprintf(stderr,"Some process has already deleted the shared memory!!!\n");
    return;
  }

  spi=shmat(pi[index*10+5],NULL,0);//attaching to shared memory

  if(pi[index*10+4]==pi[index*10+2]-1)//if the stack is full
		{
			printf("Max Size Reached\n");
	  }	
  
  else
   {
      P(semid);
      pi[index*10+4]++;//increasing value of stack top
      spi[pi[index*10+4]]=value;//pushing the value into the stack top
      V(semid);
      printf("%d has been pushed to stack with index=%d\n",value,index);
   }
     
  shmdt(spi);//detaching from shared memory
 
}


int shstackpop(int index)//pops value
{
	int x;
  
  if(pi[index*10+3]==1)//some other process removed this shared stack
  {

    printf("Some process has already deleted the shared memory!!!\n");
    return -2;
  }
 
  int *spi;
  spi=shmat(pi[index*10+5],NULL,0);//attaching to shared memory
  

  if(pi[index*10+4]==-1)//if the stack is empty
		{
			printf("No element to pop\n");
			return -1;
	  }	
  
  else
   {
	   P(semid);
     x=spi[pi[index*10+4]];//getting the value from stack top
	   pi[index*10+4]--;//decreasing stack top value
     V(semid);
     printf("%d has been popped from stack with index=%d\n",x,index);
   }
    
  shmdt(spi);//detaching from shared memory

	return x;
}


void shstackrm(int index)//deletes the stack and makes that stack descriptor index free
{
   P(semid);
   pi[index*10+4]=-1;//setting stack top to -1
   pi[index*10+3]=1;//setting the stack free
   pi[index*10]=INT_MAX;//changing the key value
   shmctl(pi[index*10+5],IPC_RMID,0);//removing shared memory for stack
   V(semid);
   printf("Shared Stack with index=%d has been removed\n",index);

}

void shstackdisplay(int index)
{

 int *spi=shmat(pi[index*10+5],NULL,0);//attaching to shared memory 
 int top=pi[index*10+4];//stack top

 printf("\n----Contents of Stack with index=%d----\n",index);
 
 if(pi[index*10+4]==-1)//if stack is empty
    {
      printf("\nNo elements to display\n");
      
    } 
 
 else
 {
   for(int i=top;i>=0;i--)
   {
      printf("%d\n",spi[i]);
   }
 }

 printf("-----------------\n");
 shmdt(spi);//detaching from shared memory

}


