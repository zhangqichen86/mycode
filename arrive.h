#ifndef __ARRIVE_H__
#define __ARRIVE_H__

#include "members.h"
#include "clerk.h"


typedef struct 
{
    time_t arr_tm;      //到店时间
    char name_a[33];      //到店人
    char ar_card[15];   //到店人的卡号
    char type;          //到店人类型 a.店员 b.会员
} arrive;

typedef struct _vec
{
    arrive* data;
    unsigned int cap;
    unsigned int sz;
} *vec;

vec create_a(unsigned int cnt);
int destory(vec v);
void add_vec(vec v, arrive* new);
void trav_a(vec v, int(*visit)(arrive*));
int show_a(arrive* a);
void traverse_a(vec v, char* wanna, int(*visit)(arrive*, char*));
int visit_a_name(arrive* data, char* wanna);
int visit_a_card(arrive* data, char* wanna);
int visit_a_type(arrive* data, char* wanna);
int visit_a_clerks(arrive* data, char* wanna);

void expand_capacity(vec v);
void arr_m_in(vec v, members* data);
void arr_cl_in(vec v, clerks* data);


#endif