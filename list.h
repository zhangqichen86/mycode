#ifndef __LIST_H__
#define __LIST_H__

#include "members.h"
#include "clerk.h"
#include "arrive.h"

void list_main();
void list_mem(list l, vec a);
void list_clerk(list l, vector v, vec a);
void list_search_m(list l);
void list_boss(list l, vector v, vec a);
void list_catch_all(list l);


#endif