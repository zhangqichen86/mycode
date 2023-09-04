#ifndef __PASSWORD_H__
#define __PASSWORD_H__
#include "members.h"
#include "clerk.h"

void password_in(char *passwd);
int getch(void);
int is_card_num(char* num);
int is_phone_num(char* num);
int is_id_num(char* num);
void sign_up_clerk(vector v);
void sign_up_mem(list l);
clerks* log_in_clerk(vector v);
members* log_in_mem(list l);
void change_password_m(list l, members* data);
void change_password_cl(vector v, clerks* data);



#endif