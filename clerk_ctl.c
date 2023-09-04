#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clerk.h"


//创建顺序表
vector create_cl(unsigned int cnt)
{
    vector n = (vector)malloc(cnt * sizeof(struct _vector));
    if(NULL == n)
    {
        fprintf(stderr, "malloc erorr\n");
        return NULL;
    }

    n->size = 0;
    n->capacity = cnt;

    n->info_c = (clerks*)malloc(sizeof(clerks) * n->capacity);

    if(NULL == n->info_c)
    {
        fprintf(stderr, "malloc erorr\n");
        return NULL;
    }

    return n;
}

//顺序表摧毁
void destory_cl(vector v)
{
    free(v->info_c);      //释放顺序表中内容
    free(v);    //释放整个顺序表
    return ;
}

// 尾插
void push_back(vector v, clerks* data)
{
    v->info_c[v->size] = *data;
    v->size++;
}

//指定位置删除元素
void del(vector v, unsigned int position)
{
    unsigned int i;

    if(position > v->size)
    {
        fprintf(stderr, "删除位置错误\n");
        return ;
    }
    
    for(i = 0; i < v->size; i++)    //删除元素后面数组每个元素前移一个单位
    {
        v->info_c[i] = v->info_c[i + 1];
    }

    v->size--;

    return ;
}


void trav_cl(vector v, int(*visit)(clerks*))
{
    // 遍历顺序表中所有数据元素
    int i;
    for(i = 0; i < v->size; i++)
    {
        if(!visit(v->info_c + i))
            break;
    }   
}

int traverse_cl(vector v, clerks* wanna, int(*visit)(clerks*, clerks*))
{
    // 遍历顺序表中所有数据元素
    int i;
    for(i = 0; i < v->size + 1; i++)
    {
        if(!visit(v->info_c + i, wanna))
            break;
    }
    if(i <= v->size)   
        return i;
    else
        return -1;
}

int show_cl(clerks* data)
{   
    struct tm *ti = localtime(&data->first_in_cl);
    
    printf("%s %s %s %s %s %s %04d年%02d月%02d日\n", data->num_cl, 
    data->name_cl, data->phone_cl, data->sex_cl, data->card_cl, 
    data->id_cl,ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday);
    
    return 1;
}


int visit_cl_by_card(clerks* a, clerks* b)
{
    if(strcmp(a->card_cl, b->card_cl) == 0)
    {
        return 0;
    }
    else 
        return 1;
}

int visit_cl_by_phone(clerks* a, clerks* b)
{
    if(strcmp(a->phone_cl, b->phone_cl) == 0)
    {
        return 0;
    }
    else 
        return 1;
}

int visit_cl_by_num(clerks* a, clerks* b)
{
    if(strcmp(a->num_cl, b->num_cl) == 0)
    {
        return 0;
    }
    else 
        return 1;
}

void del_cl(vector v)
{
    char wanna[30] = "";
    int is_found = 0;

    printf("请输入要删除店员的工号:");
    scanf("%s", wanna);
    clerks* p = (clerks*)malloc(sizeof(clerks));
    strcpy(p->num_cl, wanna);

    is_found = traverse_cl(v, p, visit_cl_by_num);
    
    if(is_found == -1)
    {
        printf("很抱歉，没找到店员工号信息。\n退出请输入y：");
        char ch = getchar();
        if(ch == 'y' || ch == 'Y')
            return ;
    }

    *p = *(v->info_c + is_found);

    printf("当前店员信息如下:\n");
    show_cl(p);
    printf("确定要删除该店员吗？(y/Y.确定)\n");
    char ch = getchar();
    if(ch == 'y' || ch == 'Y')
    {
        printf(" %s 店员信息删除成功，江湖再见！\n", p->name_cl);
        del(v, is_found);
        save_cl(v);
        sleep(1);
        return;
    }
    else
    {
        printf("未删除信息\n");
        sleep(1);
        return ;
    }
}


void save_cl(vector v)
{
    FILE* fp = fopen("./clerks/info_cl", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file failed\n");
        return ;
    }
    
    for(int i; i < v->size; i++)
    {
        fwrite(v->info_c + i, sizeof(clerks), 1, fp);
    }

    fclose(fp);
}
