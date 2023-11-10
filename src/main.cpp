#include "list.h"


int main()
{
    Iterator it = {};
    Constructor(&it);

    
    if (ListInsert(&it, 10) == -1)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    } 
    TextDumpList(&it);

    if (ListInsert(&it, 20) == -1)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    } 
    TextDumpList(&it);
    
    int value = 0;
    
    it.index = Begin(&it); 
    for (ssize_t index_end = End(&it); it.index != index_end; it.index = Next(&it))
    {
        value = GetValueList(&it);
        if (it.list->errors != 0)
        {
            DumpErrors(&it);
            Destructor(&it);
            return 1;
        }
        SetValueList(&it, value * 2);
    }
    Push_Back(&it, 90);
    Push_Back(&it, 100);
    Push_Back(&it, 120);
    Push_Back(&it, 130);
    Push_Back(&it, 140);
    Push_Back(&it, 150);
    Push_Back(&it, 160);

    it.index = 5;
    
    TextDumpList(&it);

    it.index = 2;
    ListErase(&it);

    TextDumpList(&it);

    it.index = 7;
    if (ListErase(&it) == -1)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    } 

    TextDumpList(&it);

    it.index = 6;
    if (ListErase(&it) == -1)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    }

    TextDumpList(&it);

    it.index = 3;
    if (ListErase(&it) == -1)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    }

    TextDumpList(&it);
    
    Linearization(&it);

    TextDumpList(&it);


    GraphicDumpList(&it);

    Destructor(&it);
}