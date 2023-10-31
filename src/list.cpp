#include "list.h"
#include "color.h"



void Constructor(Info* list)
{
    assert(list);

    list->free = 1;

    list->num_elem = NUM_NODES;
    list->nodes = (Node*) calloc(list->num_elem + FICT_ELEM, sizeof(Node));
    for (size_t counter = 0; counter < list->num_elem; counter++)
    {
        list->nodes[counter + FICT_ELEM].data = POISON;
        list->nodes[counter + FICT_ELEM].next = (int) (counter + FICT_ELEM + 1);
        list->nodes[counter + FICT_ELEM].prev = POISON_COUNTER;
        if (counter == 0)
        {
            list->nodes[counter].prev = NUM_NODES;
        }
    }

    list->nodes[0].data = 0;
    list->nodes[0].prev = 0;
    list->nodes[0].next = 0;
}

void  Destructor(Info* list)
{
    list->free = POISON;
    
    free(list->nodes);
}

void ListInsert(Info* list, int position, Elem_t value)
{
    assert(list);
    
    // position is index of element, which was after entered value
    // if (position == next[0])  value enter in start
    // if (prev[position] == -1) error!!!
    // if (position == 0)        value enter in end


    int free_next = list->nodes[list->free].next;               // free_next = next[free]

    list->nodes[list->free].data = value;                       // data[free] = value
    list->nodes[list->free].next = position;                    // next[free] = position
    list->nodes[list->free].prev = list->nodes[position].prev;  // prev[free] = prev[position]

    list->nodes[list->nodes[position].prev].next = list->free;  // next[prev[position]] = free
    list->nodes[position].prev = list->free;                    //      prev[position]  = free

    list->free = free_next;                                     // free = free_next
}

void ListErase(Info* list, int position)
{
    assert(list);
    
    //position - index of element, which need to remove
    // if (position == next[0])  value enter in start
    // if (position == prev[0])  value enter in end
    // if (prev[position] == -1) error!!!

    list->nodes[list->nodes[position].prev].next = list->nodes[position].next;   // next[prev[position]] = next[position];
    list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;   // prev[next[position]] = prev[position];

    list->nodes[position].data =          POISON;               // data[position] = POISON;
    list->nodes[position].prev =  POISON_COUNTER;               // prev[position] =     -1;
    list->nodes[position].next =      list->free;               // next[position] =   free;

    list->free = position;                                      // free = position;
}



void DumpList(Info* list)
{
    assert(list);
    
    printf(GREEN_COLOR"Information about this list\n");
    
    printf(BLUE_COLOR "Head of list(next[0]) = %2d.\nTail of list[prev[0]] = %2d.\n", list->nodes[0].next, list->nodes[0].prev);

    printf(YELLOW_COLOR "First free element = %2d.\n", list->free);
    printf("______________________________________\n");
    
    printf(BOLD_RED_COLOR "INDX ");
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        printf("%2lu ", counter);
    }
    printf("\n");

    printf(BOLD_RED_COLOR "DATA ");
    for (size_t counter = 0; counter < NUM_NODES + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%2d ", list->nodes[counter].data);
        else if ((list->nodes[counter].data == POISON) && (list->nodes[counter].prev == -1))
            printf(YELLOW_COLOR "ЯД ");
        else if (list->nodes[counter].data == POISON)
            printf(GREEN_COLOR "ЯД ");
        else
            printf(GREEN_COLOR "%2d ",list->nodes[counter].data);
    }
    printf("\n");
    
    printf(BOLD_RED_COLOR "NEXT ");
    for (size_t counter = 0; counter < NUM_NODES + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%2d ", list->nodes[counter].next);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%2d ", list->nodes[counter].next);
        else
            printf(GREEN_COLOR "%2d ",list->nodes[counter].next);
    }
    printf("\n");
    printf(BOLD_RED_COLOR "PREV ");
    for (size_t counter = 0; counter < NUM_NODES + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%2d ", list->nodes[counter].prev);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%2d ", list->nodes[counter].prev);
        else
            printf(GREEN_COLOR "%2d ",list->nodes[counter].prev);
    }
    printf("\n");
    printf(YELLOW_COLOR "______________________________________\n\n");
    printf(END_COLOR);
}