#ifndef LISTlib
#define LISTlib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

typedef int Elem_t;
const int    POISON_COUNTER  =      -1;
const int    POISON          = INT_MAX;
const size_t NUM_NODES       =      10;
const size_t MIN_SIZE_LIST   =       5;
const size_t FICT_ELEM       =       1;
const size_t MAX_SIZE_LIST   =     101;
const size_t VALUE_REALLOC   =       2;


struct Node
{
    Elem_t   data;
    ssize_t  next;
    ssize_t  prev;
};

struct List
{
    Node*      nodes;
    size_t      size;
    size_t  num_elem;
    ssize_t     free;
    int       errors;
};

struct Iterator 
{
    List* list = {};
    ssize_t   index;
};


void       TextDumpList(Iterator* it);
void    GraphicDumpList(Iterator* it);
void CreateGraphicNodes(List* list);
void CreateGraphicLinks(List* list);


void     Constructor(Iterator* it);
void      Destructor(Iterator* it);
ssize_t   ListInsert(Iterator* it, Elem_t value);
ssize_t    ListErase(Iterator* it); 

int       DumpErrors(Iterator* it);

int       FindErrors(List* list);
int      CheckMemory(List* list);
int  IncreaseRealloc(List* list);
int    ReduceRealloc(List* list);


ssize_t Push_Back (Iterator* it, Elem_t value);
ssize_t Push_Front(Iterator* it, Elem_t value);
ssize_t  Pop_Back (Iterator* it);
ssize_t  Pop_Front(Iterator* it);

ssize_t      Begin(Iterator* it);
ssize_t        End(Iterator* it);
ssize_t       Next(Iterator* it);

Elem_t  GetValueList(Iterator* it);
ssize_t SetValueList(Iterator* it, Elem_t value);




void FillMemory(List* list, size_t start, size_t end);




#endif


