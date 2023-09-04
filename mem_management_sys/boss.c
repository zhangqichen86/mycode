#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "md5.h"
#include "password.h"
#include "boss.h"

void initial_sys()
{
    FILE*fp = fopen("./boss/boss", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
        return ;
    }

    boss* b = (boss*)malloc(sizeof(boss));

    strcpy(b->id, "admin");
    char pass[33] = "123456";
    char res_pass[33] = "";
    md5_data(pass, strlen(pass), res_pass);
    strcpy(b->passwd, res_pass);

    fwrite(b, sizeof(boss), 1, fp);

    free(b);
    fclose(fp);
}

int is_boss()
{
    system("clear");
    FILE*fp = fopen("./boss/boss", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
        return -1;
    }

    char in_pass[33] = "";
    char result_pass[33] = "";
    char boss_id[30] = "";
    int passtime = 0;

    boss* b = (boss*)malloc(sizeof(boss));

    fread(b, sizeof(boss), 1, fp);

    while(1)
    {
        printf("请输入管理员ID:");
        scanf("%s", boss_id);
        while(getchar() != '\n') ;
        if(strcmp(b->id, boss_id) != 0)
        {
            printf("用户名错误，请重新输入\n");
            continue;
        }
        else break;
    }

    while(1)
    {
        printf("请输入密码:");
        password_in(in_pass);    //调用password_in函数输入密码

        md5_data(in_pass, strlen(in_pass), result_pass);

        if(strcmp(result_pass, b->passwd) == 0)
        {
            printf("登陆成功！欢迎 %s 店长!\n\n", b->id);
            break;
        }
        else
        {
            system("clear");
            passtime++;
            printf("密码不正确...请重试, 还有%d次机会\n", (3 - passtime));

            if(passtime == 3)
            {
                printf("三次密码输入错误，请重新登陆\n");  
                return 0;
            }
        }
    }    

    if(strcmp(b->id, "admin") == 0)
    {
        int change = 0;
        printf("当前账号密码为初始账号密码,请及时修改\n");
        printf("1.现在修改    2.下次再说\n");
        printf(">:");
        scanf("%d", &change);

        if(change == 1)
        {
            char tmp1[33] = "";
            char tmp2[33] = "";
            char change_ps[33] = "";

            while (1)
            {    
                char user_name[30] = "";
                printf("请输入新用户名(不要与默认用户名重复):\n");
                scanf("%s", user_name);
                strcpy(b->id, user_name);
                if(strcmp(b->id, "admin") == 0)
                {
                    printf("用户名不要与默认重复\n");
                    continue;
                }
                break;
            }

            while (1)   //用户名检测完毕，进入密码注册
            {
                printf("请输入密码: ");
                password_in(tmp1);

                printf("再次输入确认:");
                password_in(tmp2);

                if (strcmp(tmp1, tmp2) != 0)
                {
                    printf("两次输入密码不同，请重新输入\n");
                    sleep(2);
                    system("clear");
                }
                else
                    break;
            }

            md5_data(tmp1, strlen(tmp1), change_ps);
            strcpy(b->passwd, change_ps);    //将md5处理后的密码复制给结构体中密码    
            fseek(fp, 0, SEEK_SET);   //文件位置指针往回移动一个结构体
            fwrite(b, sizeof(boss), 1, fp);            
            free(b);
            fclose(fp);
            return 1;
        }
        
        else
        {
            free(b);
            fclose(fp);
            return 1;
        }
    }
}


int change_password_boss()
{
    char check_ps[33] = "";
    char check_md[33] = "";
    char tmp1[33] = "";
    char tmp2[33] = "";
    char change_ps[33] = "";
    int times = 0;

    FILE*fp = fopen("./boss/boss", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
        return 0;
    }
    boss* b = (boss*)malloc(sizeof(boss));
    fread(b, sizeof(boss), 1, fp);
    
    while(1)
    {
        printf("请输入密码:\n");
        scanf("%s", check_ps);
        md5_data(check_ps, strlen(check_ps), check_md);

        if(strcmp(b->passwd, check_md) == 0)
        {
            printf("验证成功\n");
            break;
        }
        else
        {
            times++;
            if(times == 3)
                return 0;
            printf("密码错误，还有 %d 次机会\n", 3 - times);
            continue;
        }
    }
        
    //验证成功，修改用户名
    while (1)
    {    
        char user_name[30] = "";
        printf("请输入新用户名(不要与默认用户名重复):\n");
        scanf("%s", user_name);
        strcpy(b->id, user_name);
        if(strcmp(b->id, "admin") == 0)
        {
            printf("用户名不要与默认重复\n");
            continue;
        }
        break;
    }

    while(1)   //用户名检测完毕，进入密码注册
    {
        printf("请输入密码: ");
        password_in(tmp1);

        printf("再次输入确认:");
        password_in(tmp2);

        if (strcmp(tmp1, tmp2) != 0)
        {
            printf("两次输入密码不同，请重新输入\n");
            sleep(2);
            system("clear");
        }
        else
            break;
    }

    md5_data(tmp1, strlen(tmp1), change_ps);
    strcpy(b->passwd, change_ps);    //将md5处理后的密码复制给结构体中密码    
    fseek(fp, 0, SEEK_SET);   //文件位置指针往回移动一个结构体
    fwrite(b, sizeof(boss), 1, fp);            

    free(b);
    fclose(fp);
    return 1;
}


void re_m_password(list l)
{
    char wanna[33] = "";
    char ini_pass[33] = "";
    char result_pass[33] = "";
    int is_found = 0;

    printf("请输入需要重置密码会员的手机号:\n");
    scanf("%s%*c", wanna);
    node* p = (node*)malloc(sizeof(node));
    p = l->head->next;

    while(NULL != p)
    {
        if(strcmp(wanna, p->info_m.phone_m) == 0)
            break;
        is_found++;
        p = p->next;
    }
    if(p == NULL)
        printf("没找到对应的手机号\n");

    else
    {
        strncpy(ini_pass, p->info_m.phone_m + 5, 6);
        md5_data(ini_pass, strlen(ini_pass), result_pass);
        strcpy(p->info_m.passwd_m, result_pass);

        FILE* fp = fopen("./members/info_m", "rb+");
        if(NULL == fp)
            fprintf(stderr, "open file error\n");
        fseek(fp, is_found * sizeof(members), SEEK_SET);
        fwrite(&p->info_m, sizeof(members), 1, fp);

        fclose(fp);
        printf("密码重置成功\n");
        sleep(1);
    }
}


void re_cl_password(vector v)
{
    system("clear");

    char wanna[33] = "";
    char ini_pass[33] = "";
    char result_pass[33] = "";
    int is_found = 0;

    printf("请输入需要重置密码店员的手机号:\n");
    scanf("%s%*c", wanna);
    clerks* p = (clerks*)malloc(sizeof(clerks));
    
    for(int i = 0; i < v->size + 1; i++)
    {
        if(strcmp(wanna, v->info_c[i].phone_cl) == 0)
            break;
        is_found++;
    }
    if(is_found == v->size + 1)
        printf("未找到手机号\n");
    
    else
    {
        strncpy(ini_pass, p->phone_cl + 5, 6);
        md5_data(ini_pass, strlen(ini_pass), result_pass);
        strcpy(p->passwd_cl, result_pass);

        FILE* fp = fopen("./clerks/info_cl", "rb+");
        if(NULL == fp)
            fprintf(stderr, "open file error\n");
        
        fseek(fp, is_found * sizeof(clerks), SEEK_SET);
        fwrite(p, sizeof(clerks), 1, fp);

        fclose(fp);

        printf("密码重置成功\n");
        sleep(1);
    }
}