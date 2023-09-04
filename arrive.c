#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "arrive.h"
#include "members.h"
#include "clerk.h"


//创建顺序表
vec create_a(unsigned int cnt)
{
    vec n = (vec)malloc(cnt * sizeof(struct _vec));
    if(NULL == n)
    {
        fprintf(stderr, "malloc erorr\n");
        return NULL;
    }
    n->sz = 0;
    n->cap = cnt;
    n->data = (arrive*)malloc(sizeof(arrive) * n->cap);
    if(NULL == n->data)
    {
        fprintf(stderr, "malloc erorr\n");
        return NULL;
    }
    return n;
}

//顺序表摧毁
int destory(vec v)
{
    free(v->data);      //释放顺序表中内容
    free(v);    //释放整个顺序表
    return 1;
}

void add_vec(vec v, arrive* new)
{
    expand_capacity(v);
    v->data[v->sz] = *new;
    v->sz++;
}

//顺序表扩大容量
void expand_capacity(vec v)
{
    if(v->cap == v->sz)
    {
        arrive* p = (arrive*)realloc(v->data, v->cap + 100);
        
        if(NULL == p)
        {
            fprintf(stderr, "malloc error\n");
            return;
        }

        v->data = p;
        v->cap += 100;
    }
}

void trav_a(vec v, int(*visit)(arrive*))    // 遍历顺序表中所有数据元素
{
    int i;
    for(i = 0; i < v->sz; i++)
    {
        if(!visit(v->data + i))
            break;
    }   
}

int show_a(arrive* a)
{
    struct tm* ti;
    ti = localtime(&a->arr_tm);

    if(a->type == 'b')
        printf("%s 会员 %s %04d年%02d月%02d日 %02d时%02d分\n",
    a->name_a, a->ar_card, ti->tm_year + 1900, 
    ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min);
    
    if(a->type == 'a')
        printf("%s 店员 %s %04d年%02d月%02d日 %02d时%02d分\n",
    a->name_a, a->ar_card, ti->tm_year + 1900, 
    ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min);

    return 1;
}

void arr_m_in(vec v, members* data)
{
    FILE* fp = fopen("./arrival/time_arr", "ab+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file failed\n");
        return;
    }
    arrive* new = (arrive*)calloc(1, sizeof(arrive));

    strcpy(new->name_a, data->name_m);
    strcpy(new->ar_card, data->card_m);
    new->type = 'b';
    new->arr_tm = time(NULL);

    fwrite(new, sizeof(arrive), 1, fp);
    add_vec(v, new);
    fclose(fp);
    free(new);
}

void arr_cl_in(vec v, clerks* data)
{
    FILE* fp = fopen("./arrival/time_arr", "ab+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file failed\n");
        return;
    }
    arrive* new = (arrive*)calloc(1, sizeof(arrive));

    strcpy(new->name_a, data->name_cl);
    strcpy(new->ar_card, data->card_cl);
    new->type = 'a';
    new->arr_tm = time(NULL);

    fwrite(new, sizeof(arrive), 1, fp);
    add_vec(v, new);
    fclose(fp);
    free(new);
}

void traverse_a(vec v, char* wanna, int(*visit)(arrive*, char*))
{
    // 遍历顺序表中所有数据元素
    int i;
    for(i = 0; i < v->sz; i++)
    {
        if(!visit(v->data + i, wanna))
            break;
    }   
}

int visit_a_name(arrive* data, char* wanna)
{
    if(strcmp(data->name_a, wanna) == 0)
    {
        show_a(data);
    }
    return 1;
}

int visit_a_card(arrive* data, char* wanna)
{
    if(strcmp(data->ar_card, wanna) == 0)
    {
        show_a(data);
    }
    return 1;
}

int visit_a_type(arrive* data, char* wanna)
{
    char a = wanna[0];
    if(data->type == a)
    {
        show_a(data);
    }
    return 1;
}

int visit_a_clerks(arrive* data, char* wanna)
{
    if(data->type == 'b' || strcmp(data->name_a, wanna) == 0)
    {
        show_a(data);
    }
    return 1;
}