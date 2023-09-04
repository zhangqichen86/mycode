#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "md5.h"
#include "members.h"
#include "password.h"

members* log_in_mem(list l)
{ // 登陆函数密码校验 返回值1表示用户名密码正确 0为错误 -1为文件错误
    int is_found = 0;
    int passtime = 0;
    char result_ps[33] = "";
    char passwd1[33] = "";

    members* userin = (members*)malloc(sizeof(members));
    
    while (1)
    {
        char str1[33] = " ";
        printf("请输入ID或手机号(或刷卡):");
        scanf("%s", str1);
        while (getchar() != '\n')
            ;
        system("clear");

        if (strlen(str1) == 14)
        {
            strcpy(userin->card_m, str1);
            is_found = traverse_m(l, userin, visit_m_by_card);
            if ((is_found != -1) && userin->is_del == 0)
            {
                printf("刷卡成功！欢迎 %s 会员!\n", userin->name_m);
                return userin;
            }

            printf("会员卡错误或者已到期,请联系前台换卡或续卡\n");
            sleep(1);
            system("clear");
            continue;
        }

        if (strlen(str1) == 11)
        {
            strcpy(userin->phone_m, str1);
            is_found = traverse_m(l, userin, visit_m_by_phone);
            // 未找到手机号
            if (is_found == -1)     
            {
                printf("输入的手机号未注册，请重试\n\n");
                continue;
            }
            // 已经删除或者逾期
            if (userin->is_del != 0)
            {
                printf("账户已逾期，请前往前台办理续卡\n\n");
                continue;
            }
        }

        if (strlen(str1) == 6)
        {
            strcpy(userin->phone_m, str1);
            is_found = traverse_m(l, userin, visit_m_by_id);
            // 未找到ID
            if (is_found == -1)
            {
                printf("输入的ID未注册，请重试\n\n");
                continue;
            }
            if (userin->is_del != 0)
            {
                printf("账户已逾期，请前往前台办理续卡\n\n");
                continue;
            }
        }

        if (strlen(str1) != 14 && strlen(str1) != 11 && strlen(str1) != 6)
        {
            printf("输入格式错误，无法识别\n\n");
            continue;
        }

        node* p = l->head->next;
        for(int i = 0; i < is_found; i++)
        {
            p = p->next;
        }
        *userin = p->info_m;

        while (1)
        {
            printf("请输入密码:");
            password_in(passwd1); // 调用password_in函数输入密码

            md5_data(passwd1, strlen(passwd1), result_ps);

            if (strcmp(result_ps, userin->passwd_m) == 0)
            {   
                system("clear");
                printf("登陆成功!\n");
                sleep(1);
                system("clear");
                printf("登陆成功！欢迎 %s 会员!\n", userin->name_m);
                return userin;
            }
            else
            {
                system("clear");
                passtime++;
                printf("密码不正确...请重试, 还有%d次机会\n", (3 - passtime));

                if (passtime == 3)
                {
                    printf("三次密码输入错误，请重新登陆\n");
                    sleep(1);
                    return NULL;
                }
            }
        }
    }
}

void sign_up_mem(list l) // 会员信息注册(会员信息初始化)
{
    int is_found = 0;
    char tmp[7];
    char is_continue = 0;

    members *user = (members *)calloc(1, sizeof(members));
    if(NULL == user)
    {
        fprintf(stderr, "malloc failed\n");
        return ;
    }

    while (1)
    {
        system("clear");
        printf("请输入用户ID:");
        scanf("%s", user->id_m);
        while (getchar() != '\n') ;

        is_found = traverse_m(l, user, visit_m_by_id);
      
        if(is_found != -1)
        {
            printf("ID已被注册,请重新注册，谢谢配合。\n");
            sleep(1);
            continue;;
        }
      
        while (1)
        {
            printf("请输入会员的手机号:");
            scanf("%11s", user->phone_m);
            if (!is_phone_num(user->phone_m)) // 判断格式不正确时重新输入手机号
            {
                system("clear");
                printf("输入手机号格式不正确，请重新输入...\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
            
            is_found = traverse_m(l, user, visit_m_by_id);
            if(is_found != -1)
            {
                system("clear");
                printf("输入手机号已经注册，请重新输入...\n");
                while (getchar() != '\n') ;
                continue;
            }

            strncpy(tmp, &user->phone_m[5], 6); // 取手机号后六位为默认密码

            md5_data(tmp, strlen(tmp), user->passwd_m); // 将6位初始密码通过MD5方式存储

            system("clear");
            printf("请输入会员姓名: ");
            scanf("%30s", user->name_m);
            while (getchar() != '\n')
                ;
            printf("性别(男or女): ");
            scanf("%s", user->sex_m);
            while (getchar() != '\n')
                ;

            while (1)
            {
                printf("请选择会员类型(1.普通   2.高级): ");
                scanf("%c", &user->type_m);
                while (getchar() != '\n') ;

                if (user->type_m < '1' || user->type_m > '2')
                {
                    printf("类型选择错误，请重新输入\n");
                    continue;
                }
                else
                    break;
            }

            user->times = 0;

            while (1)
            {
                printf("请输入卡号(首位为大写字母后13位为数字)或扫描会员卡\n>:");
                scanf("%14s", user->card_m);
                while (getchar() != '\n') ;

                system("clear");
                
                if (!is_card_num(user->card_m))
                {
                    printf("卡号异常，请重新输入或刷卡\n");
                    continue;
                }
                else
                    break;
            }

            user->is_del = 0; // 状态标量置为0表示有效用户

            user->opem_tm = user->end_tm = time(NULL); //设置截止时间为注册时间

            printf("%s 会员信息注册成功\n", user->name_m);

            break;
        }

        FILE *fp = fopen("./members/info_m", "ab");
        if (fp == NULL)
        {
            fprintf(stderr, "打开用户信息文件失败\n");
            return;
        }
        fwrite(user, sizeof(members), 1, fp);
        add_mem(l, user);
        fclose(fp);
        
        printf("是否继续注册会员信息(y/Y.继续): \n");
        is_continue = getchar();
        if (is_continue != 'y' || is_continue != 'Y')
            break;
    }
}


void change_password_m(list l, members* data)
{
    char check_ps[33] = "";     //输入密码
    char tmp1[33] = "";         //输入密码的md5值
    char result_ps[33] = "";    //新密码的md5值
    char input_ps1[33] = "";    //一次输入新密码
    char input_ps2[33] = "";    //二次输入新密码
    int is_found = 0;
    unsigned int times = 0;

    while(1)
    {
        system("clear");
        printf("请输入原密码:");
        password_in(check_ps);
        md5_data(check_ps, strlen(check_ps), tmp1);
        if(strcmp(tmp1, data->passwd_m) != 0)
        {
            times++;
            if(times == 3)
            {
                printf("连续三次密码错误，请重新验证\n");
                return ;
            }
            printf("密码错误，请重试(还有%d次机会)\n", 3 - times);
            sleep(1);
            continue;
        }
        system("clear");
        printf("验证成功！\n\n");
        break;
    }

    while (1)   //用户名检测完毕，进入密码注册
    {
        printf("请输入新密码: ");
        password_in(input_ps1);

        printf("再次输入确认:");
        password_in(input_ps2);

        if (strcmp(input_ps1, input_ps2) != 0)
        {
            printf("两次输入密码不同，请重新输入\n");
            sleep(2);
            system("clear");
        }
        else
            break;
    }

    md5_data(input_ps1, strlen(input_ps1), result_ps);

    is_found = traverse_m(l, data, visit_m_by_id);

    FILE* fp = fopen("./members/info_m", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
    }

    strcpy(data->passwd_m, result_ps);    //将md5处理后的密码复制给结构体中密码
    fseek(fp, is_found * sizeof(members), SEEK_SET);   //文件位置指针往回移动一个结构体
    fwrite(data, sizeof(members), 1, fp);

    system("clear");
    printf("修改密码成功！\n");
    fclose(fp);
    sleep(1);
}