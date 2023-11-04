#include "list.h"


int main()
{
    Iterator it = {};
    Constructor(&it);

    ListInsert(&it, 10);
    if (it.list->errors != 0)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    } 
    TextDumpList(&it);

    ListInsert(&it, 20);
    if (it.list->errors != 0)
    {
        DumpErrors(&it);
        Destructor(&it);
        return 1;
    } 
    TextDumpList(&it);
    
    int value = 0;
    // index = next(index) <-function
    it.index = Begin(&it); 
    for (ssize_t index_end = End(&it); it.index != index_end; it.index = Next(&it))
    {
        value = GetValueList(&it); // GetValueList(Iterator it);
        if (it.list->errors != 0)
        {
            DumpErrors(&it);
            Destructor(&it);
            return 1;
        }
        SetValueList(&it, value * 2);
    }

    TextDumpList(&it);
   
    GraphicDumpList(&it);

    Destructor(&it);
}