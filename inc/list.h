#ifndef LISTlib
#define LISTlib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

typedef char* Elem_t;

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
    ssize_t   errors;
};

struct Iterator 
{
    List* list = {};
    ssize_t   index;
};


void         TextDumpList(List* List);
void    GraphicDumpList(List* list);
void CreateGraphicNodes(List* list);
void CreateGraphicLinks(List* list);



int  CtorList(List** list);
void DtorList(List* list);

void     DtorNodeData(List* list);


ssize_t   ListInsert(Iterator* it, Elem_t value);
ssize_t    ListErase(Iterator* it); 

int       DumpErrors(Iterator* it);

ssize_t       FindErrors(Iterator* it);
ssize_t      CheckMemory(Iterator* it);
ssize_t  СheckForLooping(List* list);
ssize_t       LogicCheck(List* list);


ssize_t  IncreaseRealloc(List* list);
ssize_t    ReduceRealloc(List* list);
ssize_t    Linearization(List* list);


ssize_t Push_Back (List* list, Elem_t value);
ssize_t Push_Front(List* list, Elem_t value);
ssize_t  Pop_Back (List* list);
ssize_t  Pop_Front(List* list);

ssize_t      Begin(Iterator* it);
ssize_t        End(Iterator* it);
ssize_t       Next(Iterator* it);

Elem_t  GetValueList(Iterator* it);
ssize_t SetValueList(Iterator* it, Elem_t value);


ssize_t FindElemByValue(Iterator* it, Elem_t value);

void FillMemory(List* list, size_t start, size_t end);



#endif


