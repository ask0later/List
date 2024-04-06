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

    
    ListPushFront(list, name_3, &it);

    ListPushFront(list, name_4, &it);

    ListPushBack(list, name_5, &it);

    TextDumpList(list);

    GraphicDumpList(list);

    DtorList(list);
}