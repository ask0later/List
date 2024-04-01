#include "list.h"


int main()
{
    Iterator it = {};
    List* list = NULL;

    CtorList(&list);
    it.list = list;

    char name_1[] = "aboba";

    char name_2[] = "oleg";

    char name_3[] = "malina";

    char name_4[] = "telephone";
    char name_5[] = "blin";

    if (ListInsert(&it, name_1) == -1)
    {
        DumpErrors(&it);
        DtorList(list);
        return 1;
    } 

    TextDumpList(list);

    if (ListInsert(&it, name_2) == -1)
    {
        DumpErrors(&it);
        DtorList(list);
        return 1;
    } 
    TextDumpList(list);
    
    
    it.index = Begin(&it); 
   
    Push_Back(list, name_3);
    TextDumpList(list);

    Push_Front(list, name_4);
    TextDumpList(list);

    Push_Back(list, name_5);
    TextDumpList(list);

    GraphicDumpList(list);

    DtorList(list);
}