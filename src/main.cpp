#include "list.h"


int main()
{
    Iterator it = {};
    CtorIterator(&it);

    char name_1[] = "aboba";

    char name_2[] = "oleg";

    char name_3[] = "malina";

    char name_4[] = "telephone";
    char name_5[] = "blin";

    if (ListInsert(&it, name_1) == -1)
    {
        DumpErrors(&it);
        DtorIterator(&it);
        return 1;
    } 

    TextDumpList(&it);

    if (ListInsert(&it, name_2) == -1)
    {
        DumpErrors(&it);
        DtorIterator(&it);
        return 1;
    } 
    TextDumpList(&it);
    
    
    it.index = Begin(&it); 
   
    Push_Back(&it, name_3);
    TextDumpList(&it);

    Push_Front(&it, name_4);
    TextDumpList(&it);

    Push_Back(&it, name_5);
    TextDumpList(&it);

    GraphicDumpList(&it);

    DtorIterator(&it);
}