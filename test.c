#include <stdio.h>
#include "shstack1.h"
int main()

{

 //printf("----Hello----\n");

 int i=shstackget(100,4,20);
 shstackpush(i,15);
 //printf("Pushing 15\n");
 shstackpush(i,25);
 shstackpop(i);
 //printf("Pushing 25\n");
 shstackdisplay(i);
 //shstackpop(i);
 shstackrm(i);
 //shstackdisplay(i);
 getchar();
 //shstackpop(i);
 //shstackdisplay(i);
 //int j=shstackget(10,4,20);
 //shstackpush(j,15);
 //shstackdisplay(j);
 return 0;


}