#ifndef __BOSS_H__
#define __BOSS_H__

#include"members.h"
#include"clerk.h"

typedef struct
{
    char id[30];
    char passwd[33];
} boss;

int is_boss();
int change_password_boss();
void initial_sys();
void re_cl_password(vector v);
void re_m_password(list l);


#endif
