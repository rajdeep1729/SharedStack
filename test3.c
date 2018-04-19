#include <stdio.h>
#include "shstack.h"
int main()

{

 //printf("----Hello----\n");
 int i=shstackget(100,1,5);
 char k='a';
 void *x=(void*)&k;//typecasting to void *
 shstackpush(i,x);
 k='b';
 //shstackpush(i,x);
 //printf("Pushing b\n");
 shstackdisplay(i);
 shstackrm(i);
 
 getchar();
 int k1=15;
 int j=shstackget(10,4,5);
 shstackpush(j,(void *)&k1);
 shstackdisplay(j);

 float k2=32.5;
 int l=shstackget(15,8,5);
 shstackpush(l,(void *)&k2);
 shstackdisplay(l);
 return 0;


}