#include "list.h"


int main()
{
    Iterator it = {};
    List* list = NULL;

    CtorList(&list);
    it.list = list;

    char name_3[] = "malina";
    char name_4[] = "telephone";
    char name_5[] = "blin";

   
    TextDumpList(list);

    
    ListPushFront(list, name_3, &it);
    TextDumpList(list);

    ListPushFront(list, name_4, &it);
    TextDumpList(list);

    ListPushBack(list, name_5, &it);
    TextDumpList(list);

    GraphicDumpList(list);

    DtorList(list);
}