#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stdio.h>
#include <stdlib.h>

#define errmsg(msg) errprint(__FILE__, __LINE__,msg)

void * mymalloc(size_t size);
void myfree(void * addr);
unsigned int getunalloc();

#endif

