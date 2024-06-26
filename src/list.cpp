#include "list.h"
#include "color.h"
#include "Dotter.h"

#define CHECH_MEMORY

const char* GRAPH_DOT = "Graph.dot";
const char* GRAPH_JMP = "Graph.ipg";



int CtorList(List** list)
{
    *list  = (List*) calloc(1, sizeof(List));
    
    if (!list)
        return 1;

    (*list)->free     = 1;
    (*list)->errors   = 0;
    (*list)->num_elem = 0;

    (*list)->size = NUM_NODES;
    (*list)->nodes = (Node*) calloc((*list)->size + FICT_ELEM, sizeof(Node));

    FillMemory((*list), FICT_ELEM, (*list)->size + FICT_ELEM);

    (*list)->nodes[0].data = (char*) POISON;
    (*list)->nodes[0].prev = 0;
    (*list)->nodes[0].next = 0;


    return 0;    
}


void DtorList(List* list)
{
    list->free   = POISON;
    list->errors = POISON;
    
    free(list->nodes);
    free(list);
}

void DtorNodeData(List* list)
{
    ssize_t counter = list->nodes[0].next;

    while (counter != 0)
    {
        free(list->nodes[counter].data);

        counter = list->nodes[counter].next;
    }

    return;
}

ssize_t ListInsert(List* list, Elem_t value, ssize_t position, Iterator* it)
{
    // position is index of element, which was after entered value
    // if (position == next[0])  value enter in start
    // if (prev[position] == -1) error!!!
    // if (position == 0)        value enter in end

    if (Verificator(list) != 0)
        return -1;

#ifdef CHECH_MEMORY
    if (CheckMemory(list) != 0)
        return -1;
#endif

    ssize_t position_entered_elem = list->free;

    ssize_t free_next = list->nodes[list->free].next;           // free_next = next[free]

    list->nodes[list->free].data = value;                       // data[free] = value
    list->nodes[list->free].next = position;                    // next[free] = position
    list->nodes[list->free].prev = list->nodes[position].prev;           // prev[free] = prev[position]

    list->nodes[list->nodes[position].prev].next = list->free;  // next[prev[position]] = free
    list->nodes[position].prev = list->free;                    //      prev[position]  = free


    it->index = list->free;
    it->list  = list;

    list->free = free_next;                                     // free = free_next

    

    list->num_elem++;
    
    return position_entered_elem;
}

ssize_t ListErase(List* list, ssize_t position, Iterator* it)
{
    //position - index of element, which need to remove
    // if (position == next[0])  value enter in start
    // if (position == prev[0])  value enter in end
    // if (prev[position] == -1) error!!!
    
    if (Verificator(list) != 0)
        return -1;

    it->index = list->nodes[position].next;
    it->list  = list;

#ifdef CHECH_MEMORY
    if (CheckMemory(list) != 0)
        return -1;
#endif

    it->index = list->nodes[position].next;
    it->list  = list;

    free(list->nodes[position].data);

    ssize_t return_value = list->nodes[position].next;

    list->nodes[list->nodes[position].prev].next = list->nodes[position].next;   // next[prev[position]] = next[position];
    list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;   // prev[next[position]] = prev[position];

    free(list->nodes[position].data);
    list->nodes[position].data =  (char*) POISON;               // data[position] = POISON;
    list->nodes[position].prev =  POISON_COUNTER;               // prev[position] =     -1;
    list->nodes[position].next =      list->free;               // next[position] =   free;

    list->free = position;                                      // free = position;

    list->num_elem--;

    return return_value;
}

ssize_t ListPushFront(List* list, Elem_t value, Iterator* it)
{
    return ListInsert(list, value, list->nodes[0].next, it);
}

ssize_t ListPushBack(List* list, Elem_t value, Iterator* it)
{
    return ListInsert(list, value, 0, it);
}

ssize_t ListPopFront(List* list, Iterator* it)
{
    return ListErase(list, list->nodes[0].next, it);
}


ssize_t  ListPopBack(List* list, Iterator* it)
{
    return ListErase(list, list->nodes[0].prev, it);
}

Iterator ListBegin(List* list)
{
    list->errors = Verificator(list);
    if (list->errors != 0)
    {
        return Iterator{NULL, -1};
    }

    return Iterator{list, list->nodes[0].next};
}

Iterator ListEnd(List* list)
{
    list->errors = Verificator(list);
    if (list->errors != 0)
    {
        return Iterator{NULL, -1};
    }

    return Iterator{list, 0};
}

Iterator Next(Iterator* it)
{
    if (Verificator(it->list) != 0)
    {
        return Iterator{NULL, -1};
    }

    return Iterator{it->list, ((it->list)->nodes)[it->index].next};
}

Iterator Prev(Iterator* it)
{
    if (Verificator(it->list) != 0)
    {
        return Iterator{NULL, -1};
    }

    return Iterator{it->list, ((it->list)->nodes)[it->index].prev};
}


Elem_t ListGetElem(Iterator* it)
{
    return it->list->nodes[it->index].data;
}

ssize_t ListSetElem(Iterator* it, Elem_t value)
{
    strcpy(it->list->nodes[it->index].data, value);
    return it->index;
}

Iterator FindElem(List* list, Elem_t value)
{
    Iterator wanted_elem = {list, -1};

    for (Iterator it_1 = ListBegin(list), it_2 = ListEnd(list); it_1.index != it_2.index; it_1 = Next(&it_1))
    {
        Elem_t current_value = ListGetElem(&it_1);

        if (strcmp(current_value, value) == 0)
        {
            wanted_elem = it_1;
            break;
        }
    }

    return wanted_elem;
}

void FillMemory(List* list, size_t start, size_t end)
{
    assert(list);

    for (size_t counter = start; counter < end; counter++) 
    {
        list->nodes[counter].data = (char*) POISON;
        list->nodes[counter].next = (int) (counter + FICT_ELEM);
        list->nodes[counter].prev = POISON_COUNTER;
    }
}



ssize_t Linearization(List* list)
{
    char* temporaryList[MAX_SIZE_LIST] = {};
    size_t counter      = 0;
    size_t counter_full = 0;

    do
    {
        temporaryList[counter_full] = list->nodes[counter].data;
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
    } while (counter != 0);

    free(list->nodes);

    Node* new_list = (Node*) calloc(list->size + FICT_ELEM, sizeof(Node)); 
    if (new_list == NULL) {list->errors = 1 << 9; return list->errors;}
    list->nodes = new_list;

    for (counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
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

    return list->errors;
}



void GraphicDumpList(List* list)
{
    dtBegin(GRAPH_DOT);             // Начало dot-описания графа

    CreateGraphicNodes(list);       // Создаем узлы
    CreateGraphicLinks(list);       // Создаем связи

    dtEnd();                        // Конец dot-описания графа

    dtRender(GRAPH_DOT);
}


void CreateGraphicNodes(List* list)
{
    assert(list);

    dtNodeStyle ().shape            ("box");   // Устанавливаем стиль узлов
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    char str[MAX_SIZE_LIST] = {};
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
                         "DATA = %s\n"
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
        dtLink ((int) counter, (int) (counter + 1), "");//"fontsize=200");
    }
    dtLinkStyle().style   ("bold");
    dtLinkStyle().color("#000000"); //HEX_BLACK
    
    for (size_t counter = 0; counter < list->size - 1; counter++)
    {
        next = (int) list->nodes[counter].next;
        dtLink((int)counter, next, "");
    }
}

void TextDumpList(List* list)
{
    printf(GREEN_COLOR  "\nList information about this list\n");
    
    printf(BLUE_COLOR   "Head of list(next[0]) = %3ld.\n"
                        "Tail of list[prev[0]] = %3ld.\n",\
                        list->nodes[0].next,\
                        list->nodes[0].prev);

    printf(YELLOW_COLOR "First free element = %3ld.\n", list->free);
    printf("______________________________________\n");
    
    printf(BOLD_RED_COLOR "INDX ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        printf("%10lu ", counter);
    }
    printf("\n");

    printf(BOLD_RED_COLOR "DATA ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "    ЯД    ");
        else if ((list->nodes[counter].data == (char*) POISON) && (list->nodes[counter].prev == -1))
            printf(YELLOW_COLOR "    ЯД    ");
        else if (list->nodes[counter].data == (char*) POISON)
            printf(GREEN_COLOR "    ЯД    ");
        else
            printf(GREEN_COLOR "%10s ",list->nodes[counter].data);
    }
    printf("\n");
    
    printf(BOLD_RED_COLOR "NEXT ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%10ld ", list->nodes[counter].next);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%10ld ", list->nodes[counter].next);
        else
            printf(GREEN_COLOR "%10ld ",list->nodes[counter].next);
    }
    printf("\n");
    printf(BOLD_RED_COLOR "PREV ");
    for (size_t counter = 0; counter < list->size + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%10ld ", list->nodes[counter].prev);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%10ld ", list->nodes[counter].prev);
        else
            printf(GREEN_COLOR "%10ld ",list->nodes[counter].prev);
    }
    printf("\n");
    printf(YELLOW_COLOR "______________________________________\n");
    printf(GREEN_COLOR "End information\n\n");
    printf(END_COLOR);
}

ssize_t CheckMemory(List* list)
{
    if (list->free == (int) list->size)
        return IncreaseRealloc(list);
    else if ((list->num_elem <= list->size / 4) && (list->size > MIN_SIZE_LIST)) 
        return  ReduceRealloc(list);
    
    list->errors = Verificator(list);

    return list->errors;
}

ssize_t IncreaseRealloc(List* list)
{
    list->size *= VALUE_REALLOC;
    Node* new_list = (Node*) realloc(list->nodes, (list->size + FICT_ELEM) * sizeof(Node));
    if (new_list == NULL) {list->errors = 1 << 9; return list->errors;}
    list->nodes = new_list;
        
    FillMemory(list, list->size / VALUE_REALLOC + FICT_ELEM, list->size + FICT_ELEM);
    
    return list->errors;
}

ssize_t ReduceRealloc(List* list)
{
    list->size /= VALUE_REALLOC;
    
    Linearization(list);
    
    return list->errors;
}

ssize_t Verificator(List* list)
{
    if (list == NULL)                                    return 1 << 1;

    if (list->nodes == NULL)                             return 1 << 2;
    if (list->free > (int) list->size)          list->errors |= 1 << 5;
    if (list->nodes[0].data != (char*) POISON)  list->errors |= 1 << 6;
    
    list->errors |= СheckForLooping(list);
    list->errors |= LogicCheck(list);

    return list->errors;
}

ssize_t СheckForLooping(List* list)
{
    size_t counter_full = 0;
    size_t counter      = (size_t) list->nodes[0].next;
    size_t counterstart = counter;
    if (list->num_elem == 0)
        return 0;
    do
    {
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
        if (counter == 0)
            return 0;
        if (counter == counterstart)
            return 1 << 7;
    } while (counter_full < list->size);
    
    return 0;
}

ssize_t LogicCheck(List* list)
{
    size_t counter_full = 0;
    size_t counter_new  = 0;
    size_t counter_old  = 0;
    do
    {
        counter_old = counter_new;
        counter_new = (size_t) list->nodes[counter_old].next;
        if (list->nodes[counter_new].prev != (int) counter_old)
            return 1 << 8;
        counter_full++;
    } while (counter_full < list->size);
    
    return 0;
}


int DumpErrors(Iterator* it)
{
    List* list = it-> list;
    if (list->errors == 0)
        list->errors = 1 << 0;

    switch(list->errors)
    {
        case 1 << 0:
            printf("ERRORS : it is null\n");
            break;
        case 1 << 1:
            printf("ERRORS : list is null\n");
            break;
        case 1 << 2:
            printf("ERRORS : nodes is null\n");
            break;
        case 1 << 3:
            printf("ERROR : index above size\n");
            break;
        case 1 << 4:
            printf("ERROR : index on free memory\n");
            break;
        case 1 << 5:
            printf("ERROR : excess memory is used\n");
            break;
        case 1 << 6:
            printf("ERROR : the dictitious element was affecred\n");
            break;
        case 1 << 7:
            printf("ERROR : elements are looped\n");
            break;
        case 1 << 8:
            printf("ERROR : logical error(prev[next[position]] != position)\n");
            break;
        case 1 << 9:
            printf("ERROR : FAIL ALLOCATION\n");
            break;
        default:
            printf("EXTRA ERROR\n");
            break;
    }

    return (int) list->errors;
}
