#ifndef __CLERK_H__
#define __CLERK_H__


#include<time.h>

//店员人数较少，可以使用顺序表实现
typedef struct _clerks      //店员结构体
{   
    char num_cl[9];         //工号
    char name_cl[30];       //姓名
    char phone_cl[12];      //手机号：限定11位
    char sex_cl[4];         //性别
    char card_cl[15];       //卡号
    char id_cl[19];         //身份证号：限定18位
    char passwd_cl[33];     //密码
    time_t first_in_cl;     //入职时间

} clerks;

// 声明链表节点结构体类型
typedef struct _vector
{
    clerks* info_c;         //节点内部数据域存放会员结构体信息
    unsigned int capacity;
    unsigned int size; 
} *vector;


int show_cl(clerks* data);
int traverse_cl(vector v, clerks* wanna, int(*visit)(clerks*, clerks*));
void trav_cl(vector v, int(*visit)(clerks*));
void destory_cl(vector v);
vector create_cl(unsigned int cnt);
void push_back(vector v, clerks* data);
int visit_cl_by_card(clerks* a, clerks* b);
int visit_cl_by_phone(clerks* a, clerks* b);
int visit_cl_by_num(clerks* a, clerks* b);
void save_cl(vector v);
void del_cl(vector v);
void del(vector v, unsigned int position);


#endif