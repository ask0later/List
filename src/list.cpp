#include "list.h"
#include "color.h"
#include "Dotter.h"

const char* GRAPH_DOT = "Graph.dot";
const char* GRAPH_JMP = "Graph.ipg";


void Constructor(Info* list)
{
    assert(list);

    list->free   = 1;
    list->errors = 0;

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

    list->nodes[0].data = POISON;
    list->nodes[0].prev = 0;
    list->nodes[0].next = 0;
}

void Destructor(Info* list)
{
    list->free = POISON;
    
    free(list->nodes);
}
 
int ListInsert(Info* list, int position, Elem_t value)
{
    assert(list);
    
    // position is index of element, which was after entered value
    // if (position == next[0])  value enter in start
    // if (prev[position] == -1) error!!!
    // if (position == 0)        value enter in end

    if (list->nodes[position].prev == POISON_COUNTER)
        list->errors = 1 << 4;
    if (list->errors == 0)
        list->errors = FindErrors(list);
    
    if (list->errors != 0)
        return -1;

    CheckMemory(list);

    int position_entered_elem = list->free;
    int free_next = list->nodes[list->free].next;               // free_next = next[free]

    list->nodes[list->free].data = value;                       // data[free] = value
    list->nodes[list->free].next = position;                    // next[free] = position
    list->nodes[list->free].prev = list->nodes[position].prev;  // prev[free] = prev[position]

    list->nodes[list->nodes[position].prev].next = list->free;  // next[prev[position]] = free
    list->nodes[position].prev = list->free;                    //      prev[position]  = free

    list->free = free_next;                                     // free = free_next
    
    return position_entered_elem;
}

int ListErase(Info* list, int position)
{
    assert(list);
    
    //position - index of element, which need to remove
    // if (position == next[0])  value enter in start
    // if (position == prev[0])  value enter in end
    // if (prev[position] == -1) error!!!

    if (list->nodes[position].prev == POISON_COUNTER)
        list->errors = 1 << 4;
    if (list->errors == 0)
        list->errors = FindErrors(list);

    if (list->errors != 0)
        return -1;
    CheckMemory(list);
    TextDumpList(list);

    
    int return_value = list->nodes[position].next;
    list->nodes[list->nodes[position].prev].next = list->nodes[position].next;   // next[prev[position]] = next[position];
    list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;   // prev[next[position]] = prev[position];

    list->nodes[position].data =          POISON;               // data[position] = POISON;
    list->nodes[position].prev =  POISON_COUNTER;               // prev[position] =     -1;
    list->nodes[position].next =      list->free;               // next[position] =   free;

    list->free = position;                                      // free = position;

    return return_value;
}


void GraphicDumpList(Info* list)
{
    dtBegin(GRAPH_DOT);             // Начало dot-описания графа

    CreateGraphicNodes(list);       // Создаем узлы
    CreateGraphicLinks(list);       // Создаем связи

    dtEnd(); // Конец dot-описания графа

    dtRender(GRAPH_DOT);
}
void CreateGraphicNodes(Info* list)
{
    assert(list);

    dtNodeStyle ().shape            ("box"); // Устанавливаем стиль узлов
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    char str[80] = {};
    for (int counter = 0; counter < (int) list->num_elem; counter++)
    {
        if ((list->nodes[counter].prev == POISON_COUNTER) || (counter == 0)) 
        {
            sprintf(str, "INDX = %2d\n"
                         "DATA = ЯД\n"
                         "NEXT = %2d\n"
                         "PREV = %2d", counter, list->nodes[(size_t) counter].next, list->nodes[(size_t) counter].prev);
        }
        else
        {
            sprintf(str, "INDX = %2d\n"
                         "DATA = %2d\n"
                         "NEXT = %2d\n"
                         "PREV = %2d", counter, list->nodes[(size_t) counter].data, list->nodes[(size_t) counter].next, list->nodes[(size_t) counter].prev);
        }
        if (list->nodes[(size_t) counter].prev == POISON_COUNTER)     // Устанавливаем цвет узлов
            dtNodeStyle().fillcolor("#F8D568");//HEX_YELLOW       
        else if (counter == 0)
            dtNodeStyle().fillcolor("#EE204D");//HEX_RED
        else if ((counter == list->nodes[0].prev) || (counter == list->nodes[0].next))
            dtNodeStyle().fillcolor("#7BF2DA");//HEX_TIFFANY
        else 
            dtNodeStyle().fillcolor("#21C912");//HEX_GREEN

        dtNode(counter, str);
    }
}
void CreateGraphicLinks(Info* list)
{
    assert(list);
    int next = 0;
    dtLinkStyle().style ("invis");                  // Устанавливаем стиль связей
    dtLinkStyle().color("#FFFFFF"); //HEX_WHITE
    for (int counter = 0; counter < (int) list->num_elem - 1; counter++)
    {
        dtLink (counter, counter + 1, "");//"fontsize=200");
    }
    dtLinkStyle().style   ("bold");
    dtLinkStyle().color("#000000"); //HEX_BLACK
    
    for (int counter = 0; counter < (int) list->num_elem - 1; counter++)
    {
        next = list->nodes[(size_t) counter].next;
        dtLink(counter, next, "");
    }
}

void TextDumpList(Info* list)
{
    assert(list);
    
    printf(GREEN_COLOR  "\nInformation about this list\n");
    
    printf(BLUE_COLOR   "Head of list(next[0]) = %2d.\n"
                        "Tail of list[prev[0]] = %2d.\n",\
                        list->nodes[0].next,\
                        list->nodes[0].prev);

    printf(YELLOW_COLOR "First free element = %2d.\n", list->free);
    printf("______________________________________\n");
    
    printf(BOLD_RED_COLOR "INDX ");
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        printf("%2lu ", counter);
    }
    printf("\n");

    printf(BOLD_RED_COLOR "DATA ");
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "ЯД ");
        else if ((list->nodes[counter].data == POISON) && (list->nodes[counter].prev == -1))
            printf(YELLOW_COLOR "ЯД ");
        else if (list->nodes[counter].data == POISON)
            printf(GREEN_COLOR "ЯД ");
        else
            printf(GREEN_COLOR "%2d ",list->nodes[counter].data);
    }
    printf("\n");
    
    printf(BOLD_RED_COLOR "NEXT ");
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
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
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        if (counter == 0)
            printf(RED_COLOR "%2d ", list->nodes[counter].prev);
        else if (list->nodes[counter].prev == -1)
            printf(YELLOW_COLOR "%2d ", list->nodes[counter].prev);
        else
            printf(GREEN_COLOR "%2d ",list->nodes[counter].prev);
    }
    printf("\n");
    printf(YELLOW_COLOR "______________________________________\n");
    printf(GREEN_COLOR "End information\n\n");
    printf(END_COLOR);
}


void CheckMemory(Info* list)
{
    assert(list);
    size_t numfree = 0;

    if (list->free == (int) list->num_elem)
    {
        IncreaseRealloc(list);
    }
    else
    {
        for (size_t counter = 1; counter < list->num_elem + FICT_ELEM; counter++)
        {
            if (list->nodes[counter].prev != -1)
            {
                numfree++;
            }
        }
        if (numfree <= list->num_elem / 4)
        {
            if (list->num_elem > NUM_NODES / 2)
                ReduceRealloc(list);
        }
    }
}
void IncreaseRealloc(Info* list)
{
    assert(list);

    Node temporaryList[MAX_SIZE_LIST] = {};
    for (size_t counter = 0; counter < list->num_elem + FICT_ELEM; counter++)
    {
        temporaryList[counter].data = list->nodes[counter].data;
        temporaryList[counter].next = list->nodes[counter].next;
        temporaryList[counter].prev = list->nodes[counter].prev;
    }
    free(list->nodes);

    list->num_elem = 2 * list->num_elem;

    list->nodes = (Node*) calloc(list->num_elem + FICT_ELEM, sizeof(Node));
    for (size_t counter = 0; counter < list->num_elem / 2 + FICT_ELEM; counter++)
    {
        list->nodes[counter].data = temporaryList[counter].data;
        list->nodes[counter].next = temporaryList[counter].next;
        list->nodes[counter].prev = temporaryList[counter].prev;
    }
    for (size_t counter = list->num_elem / 2 + FICT_ELEM; counter < list->num_elem + FICT_ELEM; counter++)
    {
        list->nodes[counter].data = POISON;
        list->nodes[counter].next = (int) (counter + FICT_ELEM);
        list->nodes[counter].prev = POISON_COUNTER;
    }
}
void ReduceRealloc(Info* list)
{
    assert(list);

    int temporaryList[MAX_SIZE_LIST] = {};
    size_t counter = 0;
    size_t counter_full = 0;
    do
    {
        temporaryList[counter_full] = list->nodes[counter].data;
        counter = (size_t) list->nodes[counter].next;
        counter_full++;
    } while (counter != 0);

    free(list->nodes);

    list->num_elem = list->num_elem / 2;

    list->nodes = (Node*) calloc(list->num_elem + FICT_ELEM, sizeof(Node));

    for (counter = 0; counter < counter_full; counter++)
    {
        list->nodes[counter].data = temporaryList[counter];

        if (counter == counter_full - 1)
            list->nodes[counter].next = 0;
        else
            list->nodes[counter].next = counter + 1;
        if (counter == 0)
            list->nodes[counter].prev = counter_full - 1;
        else 
            list->nodes[counter].prev = counter - 1;
    }
    list->free = (int) counter_full;
    for (counter = counter_full; counter < list->num_elem + FICT_ELEM; counter++)
    {
        list->nodes[counter].data = POISON;
        list->nodes[counter].next = (int) (counter + FICT_ELEM);
        list->nodes[counter].prev = POISON_COUNTER;
    }
}

int FindErrors(Info* list)
{
    if (list == NULL)                         list->errors = 1 << 0;
    if (list->nodes == NULL)                  list->errors = 1 << 1;
    if (list->free > (int) list->num_elem)    list->errors = 1 << 2;
    if (list->nodes[0].data != POISON)        list->errors = 1 << 3;

    return list->errors;
}

int DumpErrors(Info* list)
{
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
        default:
            printf("EXTRA ERROR\n");
            break;
    }

    return list->errors;
}