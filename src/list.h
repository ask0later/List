#ifndef LISTlib
#define LISTlib
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

typedef int Elem_t;
const int    POISON_COUNTER  =      -1;
const int    POISON          = INT_MAX;
const size_t NUM_NODES       =      10;
const size_t FICT_ELEM       =       1;
const size_t MAX_SIZE_LIST   =     101;

struct Node
{
    int data;
    int    next;
    int    prev;
};

struct Info
{
    Node* nodes;
    size_t num_elem;
    int free;
    int errors;
};


void       TextDumpList(Info* list);
void    GraphicDumpList(Info* list);
void CreateGraphicNodes(Info* list);
void CreateGraphicLinks(Info* list);


void Constructor(Info* list);
void  Destructor(Info* list);
int   ListInsert(Info* list, int position, Elem_t value);
int    ListErase(Info* list, int position              ); 

int       DumpErrors(Info* list);
int       FindErrors(Info* list);
void     CheckMemory(Info* list);
void IncreaseRealloc(Info* list);
void   ReduceRealloc(Info* list);


int push_back (Info* list, Elem_t value);
int push_front(Info* list, Elem_t value);
int  pop_back (Info* list);
int  pop_front(Info* list);

int begin(Info* list);
int   end(Info* list);

int GetValueList(Info* list, int position);
int SetValueList(Info* list, Elem_t value, int position);




#endif


