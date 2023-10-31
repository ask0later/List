#include "list.h"



int main()
{
    //Info list;

    struct Info list;
    Constructor(&list);
    
    DumpList(&list);
    
    ListInsert(&list, 0, 50);
    DumpList(&list);
    
    ListInsert(&list, 0, 30);
    DumpList(&list);
    
    ListInsert(&list, 1, 60);
    DumpList(&list);

    ListErase(&list, 2);
    DumpList(&list);

    Destructor(&list);
}