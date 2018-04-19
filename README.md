#SharedStack
Implementation of stack functions in C that I did as part of my OS Lab

The shstack1.c and shstack1.h are respectively c and header files for integer stack using shared memory.
The shstack.c and shstack.h are respectively c and header files for generic stack using shared memory.

You can compile and generate object files from these using the following command:

gcc -c shstack.c -o shstack.o
gcc -c shstack1.c -o shstack1.o
gcc test.c shstack1.o
gcc  test3.c shstack.o

test.c and test1.c can be used to test the functions of shstack1.c

test2.c and test3.c can be used to test the functions of shstack.c

The functions:
shstackget()is used to get the handle of the shared memory and other functions are normal stack functions.shstackrm() removes the shared memory allocated for the stack.
