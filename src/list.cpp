#include "list.h"
#include "color.h"
#include "Dotter.h"

#define CHECH_MEMORY

const char* GRAPH_DOT = "Graph.dot";
const char* GRAPH_JMP = "Graph.ipg";


void Constructor(Iterator* it)
{
    assert(it);

    it->index = 0;
    it->list  = (List*) calloc(1, sizeof(List));

    List* list = it->list;

    list->free     = 1;
    list->errors   = 0;
    list->num_elem = 0;

    list->size = NUM_NODES;
    list->nodes = (Node*) calloc(list->size + FICT_ELEM, sizeof(Node));

    FillMemory(list, FICT_ELEM, list->size + FICT_ELEM);

    list->nodes[0].data = POISON;
    list->nodes[0].prev = 0;
    list->nodes[0].next = 0;
}

void Destructor(Iterator* it)
{
    List* list = it->list; 
    list->free   = POISON;
    list->errors = POISON;
    
    free(it->list->nodes);
    free(it->list);
}
 
ssize_t ListInsert(Iterator* it, Elem_t value)
{
    assert(it);

    List* list   = it-> list;
    ssize_t position = it->index; 
    
    // position is index of element, which was after entered value
    // if (position == next[0])  value enter in start
    // if (prev[position] == -1) error!!!
    // if (position == 0)        value enter in end

    list->errors = FindErrors(list);
    
    if (list->nodes[position].prev == POISON_COUNTER)
        list->errors |= 1 << 4;

    if (list->errors != 0)
        return -1;

#ifdef CHECH_MEMORY
    CheckMemory(list);
    if (list->errors != 0)
        return -1;
#endif

    ssize_t position_entered_elem = list->free;
    ssize_t free_next = list->nodes[list->free].next;           // free_next = next[free]

    list->nodes[list->free].data = value;                       // data[free] = value
    list->nodes[list->free].next = position;                    // next[free] = position
    list->nodes[list->free].prev = list->nodes[position].prev;           // prev[free] = prev[position]

    list->nodes[list->nodes[position].prev].next = list->free;  // next[prev[position]] = free
    list->nodes[position].prev = list->free;                    //      prev[position]  = free

    list->free = free_next;                                     // free = free_next

    list->num_elem++;
    
    return position_entered_elem;
}

ssize_t ListErase(Iterator* it)
{
    assert(it);

    List* list   = it-> list;
    ssize_t position = it->index;

    // int index = it.index
    
    //position - index of element, which need to remove
    // if (position == next[0])  value enter in start
    // if (position == prev[0])  value enter in end
    // if (prev[position] == -1) error!!!
    list->errors = FindErrors(list);
    
    if (list->nodes[position].prev == POISON_COUNTER)
        list->errors |= 1 << 4;

    if (list->errors != 0)
        return -1;

#ifdef CHECH_MEMORY
    CheckMemory(list);
    if (list->errors != 0)
        return -1;
#endif

    ssize_t return_value = list->nodes[position].next;

    list->nodes[list->nodes[position].prev].next = list->nodes[position].next;   // next[prev[position]] = next[position];
    list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;   // prev[next[position]] = prev[position];

    list->nodes[position].data =          POISON;               // data[position] = POISON;
    list->nodes[position].prev =  POISON_COUNTER;               // prev[position] =     -1;
    list->nodes[position].next =      list->free;               // next[position] =   free;

    list->free = position;                                      // free = position;

    list->num_elem--;

    return return_value;
}


void GraphicDumpList(Iterator* it)
{
    List* list = it-> list;

    dtBegin(GRAPH_DOT);             // Начало dot-описания графа

    CreateGraphicNodes(list);       // Создаем узлы
    CreateGraphicLinks(list);       // Создаем связи

    dtEnd(); // Конец dot-описания графа

    dtRender(GRAPH_DOT);
}
void CreateGraphicNodes(List* list)
{
    assert(list);

    dtNodeStyle ().shape            ("box"); // Устанавливаем стиль узлов
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    char str[80] = {};
    for (size_t counter = 0; counter < list->size; counter++)
    {
        if ((list->nodes[counter].prev == POISON_COUNTER) || (counter == 0)) 
        {
            sprintf(str, "INDX = %2lu\n"
                         "DATA = ЯД\n"
                         "NEXT = %2ld\n"
                         "PREV = %2ld", counter, list->nodes[counter].next, list->nodes[counter].prev);
        }
        else
        {
            sprintf(str, "INDX = %2lu\n"
                         "DATA = %2d\n"
                         "NEXT = %2ld\n"
                         "PREV = %2ld", counter, list->nodes[counter].data, list->nodes[counter].next, list->nodes[counter].prev);
        }
        if (list->nodes[counter].prev == POISON_COUNTER)     // Устанавливаем цвет узлов
            dtNodeStyle().fillcolor("#F8D568");//HEX_YELLOW       
        else if (counter == 0)
            dtNodeStyle().fillcolor("#EE204D");//HEX_RED
        else if ((counter == (size_t)list->nodes[0].prev) || (counter == (size_t)list->nodes[0].next))
            dtNodeStyle().fillcolor("#7BF2DA");//HEX_TIFFANY
        else 
            dtNodeStyle().fillcolor("#21C912");//HEX_GREEN

        dtNode((int) counter, str);
    }
}
void CreateGraphicLinks(List* list)
{
    assert(list);
    int next = 0;
    dtLinkStyle().style  ("invis"); // Устанавливаем стиль связей
    dtLinkStyle().color("#FFFFFF"); //HEX_WHITE
    for (size_t counter = 0; counter < list->size - 1; counter++)
    {
        dtLink ((int) counter, (int) (counter + 1), "weight = 1000");//"fontsize=200");
    }
    dtLinkStyle().style   ("bold");
    dtLinkStyle().color("#000000"); //HEX_BLACK
    
    for (size_t counter = 0; counter < list->size - 1; counter++)
    {
        next = (int) list->nodes[counter].next;
        dtLink((int)counter, next, "weight = 0");
    }
}

void TextDumpList(Iterator* it)
{
    assert(it);

    List* list = it-> list;

    
    printf(GREEN_COLOR  "\nListrmation about this list\n");
    
    printf(BLUE_COLOR   "Head of list(next[0]) = %3ld.\n"
                        "Tail of list[prev[0]] = %3ld.\n",\
                        list->nodes[0].next,\
                        list->nodes[0].prev);

    printf(YELLOW_COLOR "First free element = %3ld.\n", list->free);
    printf("______________________________________\n");
    
    printf(BOLD_RED_COLOR "INDX ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        printf("%3lu ", counter);
    }
    printf("\n");

    printf(BOLD_RED_COLOR "DATA ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR " ЯД ");
        else if ((list->nodes[counter].data == POISON) && (list->nodes[counter].prev == -1))
            printf(YELLOW_COLOR " ЯД ");
        else if (list->nodes[counter].data == POISON)
            printf(GREEN_COLOR " ЯД ");
        else
            printf(GREEN_COLOR "%3d ",list->nodes[counter].data);
    }
    printf("\n");
    
    printf(BOLD_RED_COLOR "NEXT ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%3ld ", list->nodes[counter].next);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%3ld ", list->nodes[counter].next);
        else
            printf(GREEN_COLOR "%3ld ",list->nodes[counter].next);
    }
    printf("\n");
    printf(BOLD_RED_COLOR "PREV ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%3ld ", list->nodes[counter].prev);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%3ld ", list->nodes[counter].prev);
        else
            printf(GREEN_COLOR "%3ld ",list->nodes[counter].prev);
    }
    printf("\n");
    printf(YELLOW_COLOR "______________________________________\n");
    printf(GREEN_COLOR "End information\n\n");
    printf(END_COLOR);
}

int CheckMemory(List* list)
{
    assert(list);

    if (list->free == (int) list->size)
        return IncreaseRealloc(list);
    else if ((list->num_elem <= list->size / 4) && (list->size > MIN_SIZE_LIST))
        return ReduceRealloc(list);
    return 0;
}
int IncreaseRealloc(List* list)
{
    assert(list);
    
    list->size *= VALUE_REALLOC;
    list->nodes = (Node*) realloc(list->nodes, (list->size + FICT_ELEM) * sizeof(Node));
    if (list->nodes == NULL) {list->errors = 1 << 5; return list->errors;}
    
    FillMemory(list, list->size / VALUE_REALLOC + FICT_ELEM, list->size + FICT_ELEM);
    

    list->errors = FindErrors(list);
    return list->errors;
}
int ReduceRealloc(List* list)
{
    assert(list);

    int temporaryList[MAX_SIZE_LIST] = {};
    size_t counter      = 0;
    size_t counter_full = 0;
    do
    {
        temporaryList[counter_full] = list->nodes[counter].data;
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
    } while (counter != 0);

    free(list->nodes);

    list->size /= VALUE_REALLOC;

    list->nodes = (Node*) calloc(list->size + FICT_ELEM, sizeof(Node));
    if (list->nodes == NULL) {list->errors = 1 << 5; return list->errors;}

    for (counter = 0; counter < counter_full; counter++)
    {
        list->nodes[counter].data = temporaryList[counter];

        if (counter == counter_full - 1)
            list->nodes[counter].next = 0;
        else
            list->nodes[counter].next = (int) (counter + 1);
        if (counter == 0)
            list->nodes[counter].prev = (int) (counter_full - 1);
        else 
            list->nodes[counter].prev = (int) (counter - 1);
    }
    list->free = (int) counter_full;

    FillMemory(list, counter_full, list->size + FICT_ELEM);

    list->errors = FindErrors(list);
    return list->errors;
}

int FindErrors(List* list)
{
    if (list == NULL)                         list->errors = 1 << 0;
    if (list->nodes == NULL)                  list->errors = 1 << 1;
    if (list->free > (int) list->size)        list->errors = 1 << 2;
    if (list->nodes[0].data != POISON)        list->errors = 1 << 3;

    return list->errors;
}

int DumpErrors(Iterator* it)
{
    List* list = it-> list;
    if (list->errors == 0)
        return 0;
    switch(list->errors) 
    {
        case 1 << 0:
            printf("ERRORS : LIST IS NULL\n");
            break;
        case 1 << 1:
            printf("ERRORS : NODES IS NULL\n");
            break;
        case 1 << 2:
            printf("ERRORS : EXCESS MEMORY IS USED\n");
            break;
        case 1 << 3:
            printf("ERRORS : THE FICTITIOUS ELEMENT WAS AFFECTED\n");
            break;
        case 1 << 4:
            printf("ERRORS : INVALID FUNCTION ARGUMENT\n");
            break;
        case 1 << 5:
            printf("ERRORS : FAIL ALLOCATION\n");
            break;
        default:
            printf("EXTRA ERROR\n");
            break;
    }

    return list->errors;
}


ssize_t Push_Back(Iterator* it, Elem_t value)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    it->index = 0;
    
    return ListInsert(it, value);
}

ssize_t Push_Front(Iterator* it, Elem_t value)
{
    List* list = it->list;
    
    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    it->index = list->nodes[0].next;
    
    return ListInsert(it, value);
}

ssize_t Pop_Back(Iterator* it)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;
    
    it->index = list->nodes[0].prev;
    
    return ListErase(it);
}

ssize_t Pop_Front(Iterator* it)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    it->index = list->nodes[0].next;
    
    return ListErase(it);
}

ssize_t Begin(Iterator* it)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    return (list->nodes[0].next);
}

ssize_t   End(Iterator* it)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;
    // return 0 is OK
    return (list->nodes[list->nodes[0].prev].next);  // always return 0
}

Elem_t GetValueList(Iterator* it)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    Elem_t return_value = list->nodes[it->index].data;
    return return_value;
}

ssize_t SetValueList(Iterator* it, Elem_t value)
{
    List* list = it->list;

    list->errors = FindErrors(list);
    if (list->errors != 0)
        return -1;

    list->nodes[it->index].data = value;
    return it->index;
}

ssize_t Next(Iterator* it)
{
    return (it->list->nodes[it->index].next);
}

void FillMemory(List* list, size_t start, size_t end)
{
    assert(list);

    for (size_t counter = start; counter < end; counter++) 
    {
        list->nodes[counter].data = POISON;
        list->nodes[counter].next = (int) (counter + FICT_ELEM);
        list->nodes[counter].prev = POISON_COUNTER;
    }
}