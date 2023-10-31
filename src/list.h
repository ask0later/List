#ifndef LISTlib
#define LISTlib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

typedef int Elem_t;
const int    POISON_COUNTER  =      -1;
const int    POISON          = INT_MAX;
const size_t NUM_NODES       =       5;
const size_t FICT_ELEM       =       1;

struct Node
{
    int data;
    int    next;
    int    prev;
};

struct Info
{
    Node* nodes;
    int free;
    size_t num_elem;
};


void    DumpList(Info* list);
void Constructor(Info* list);
void  Destructor(Info* list);
void  ListInsert(Info* list, int position, Elem_t value);
void   ListErase(Info* list, int position              ); 



#endif