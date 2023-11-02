#include "list.h"



int main()
{
    //Info list;

    struct Info list;
    Constructor(&list);

    ListInsert(&list, list.nodes[0].next, 50);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    } 
    TextDumpList(&list);
    
    ListInsert(&list, 0, 10);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);
    
    ListInsert(&list, 0, 20);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);
    
    ListInsert(&list, 0, 30);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 40);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 50);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 60);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 70);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 80);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListInsert(&list, 0, 90);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 2);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 3);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 5);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 7);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 8);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    ListErase(&list, 1);
    if (list.errors != 0)
    {
        DumpErrors(&list);
        Destructor(&list);
        return 1;
    }
    TextDumpList(&list);

    // ListErase(&list, 6);
    // if (list.errors != 0)
    // {
    //     DumpErrors(&list);
    //     Destructor(&list);
    //     return 1;
    // }
    // TextDumpList(&list);

    // ListErase(&list, 4);
    // if (list.errors != 0)
    // {
    //     DumpErrors(&list);
    //     Destructor(&list);
    //     return 1;
    // }
    // TextDumpList(&list);

    //GraphicDumpList(&list);

    Destructor(&list);
}