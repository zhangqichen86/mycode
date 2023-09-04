#ifndef __MEMBERS_H__
#define __MEMBERS_H__

#include <time.h>

//会员人数较多，增删操作频繁，使用链表实现
typedef struct 
{
    char id_m[7];       //  ID: 长度限制为6位
    char name_m[30];    //  姓名: 可能出现重名情况 卡号和手机号唯一
    char sex_m[4];      //  性别: 男 女
    char phone_m[12];   //  手机号: 字符串长度为11
    char type_m;        //  会员类型: ‘1’普通会员 ‘2’高级会员
    char card_m[15];    //  独立卡号: 长度为14，独立卡号可以无需输入密码
    time_t opem_tm;     //  时间: time.h 头文件中定义的类型
    time_t last_tm;     //  上次续卡日期
    time_t end_tm;      //  截止时间：高级会员按时间计费
    unsigned int times; //  剩余次数：普通会员按次数计费
    char passwd_m[33];  //  密码: 密码通过MD5方式存储，长度为32位
    int is_del;         //  0表示有效 1表示无效  状态标志用于记录会员是否过期以及是否信息被删除
} members;


// 声明链表节点结构体类型
typedef struct node
{
    members info_m;         //节点内部数据域存放会员结构体信息
    struct node* next;      //指针域存放next指针(单链表)
} node;


// 声明链表结构体类型，存放链表的属性信息
typedef struct _list
{
    node* head;
    node* tail;
    unsigned int size_ml;
} *list;


list create_m(void);
void destroy_m(list l);
void add_mem(list l, members* new);
int traverse_m(list l, members* new_data, int(*visit)(members*, members*));
void trav_m(list l, int(*visit)(members*));
int show_mem(members* info);
int visit_m_by_id_show(members* data, members* wanna);
int visit_m_by_name_show(members* data, members* wanna);
int visit_m_by_phone_show(members* data, members* wanna);
int visit_m_by_type_show(members* data, members* wanna);
int visit_m_by_card_show(members* data, members* wanna);
int visit_m_by_id(members* a, members* b);
int visit_m_by_phone(members* a, members* b);
int visit_m_by_card(members* a, members* b);
void del_m(list l);
void watch_remain_tm(members* data);
void add_time();
void look_manual();
void look_activity();
void my_sort_m(list l, int(*cmp)(const void*, const void*));
int cmp_m_id(const void* a, const void* b);
int cmp_m_card(const void* a, const void* b);
int cmp_m_type(const void* a, const void* b);





#endif