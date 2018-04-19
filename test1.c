#include <stdio.h>
#include "shstack1.h"
int main()

{

 //printf("----Hello----\n");
 int i=shstackget(100,4,20);
 int data;
 //printf("Index=%d",i);
 shstackdisplay(i);
 //printf("----Hello3----\n");
 shstackpush(i,10);
 printf("Give Data\n");
 scanf("%d",&data);
 /*checking to see if after stack has been removed by other process it is giving error or not*/
 shstackpush(i,data);
 //shstackpop(i);
 //getchar();
 shstackdisplay(i);
 
 //shstackrm(i);
 int j=shstackget(10,4,20);
 //printf("Index=%d",j);
 shstackpush(j,15);
 shstackdisplay(j);
 return 0;


}