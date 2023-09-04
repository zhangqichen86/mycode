#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "clerk.h"
#include "password.h"
#include "md5.h"


clerks* log_in_clerk(vector v)
{   //登陆函数密码校验 返回值1表示用户名密码正确 0为错误 -1为文件错误
    int passtime = 0;
    char result_ps[33] = "";
    char passwd1[33] = "";
    int is_found = 0;

    clerks* userin = (clerks*)calloc(1, sizeof(clerks));
    clerks* who = (clerks*)calloc(1, sizeof(clerks));

    while(1)
    {   
        char str2[33] = "";
        printf("请输入ID或手机号(或刷卡):");
        scanf("%s", str2);
        while (getchar() != '\n') ;
        system("clear");
        
        if(strlen(str2) == 14)
        {
            strcpy(userin->card_cl, str2);
            is_found = traverse_cl(v, userin, visit_cl_by_card);
            
            if(is_found != -1)
            {
                printf("刷卡成功！欢迎 %s 店员!\n", userin->name_cl);
                return v->info_c + is_found;
            }            

            printf("员工卡错误,请联系店长\n");
            sleep(1);
            system("clear");
            continue;
        }

        if(strlen(str2) == 11)
        {
            strcpy(userin->phone_cl, str2);
            is_found = traverse_cl(v, userin, visit_cl_by_phone);

            if(is_found == -1)
            {
                printf("输入的手机号未注册，请重试\n\n");
                continue;
            }
        }

        if(strlen(str2) == 8)
        {
            strcpy(userin->id_cl, str2);
            is_found = traverse_cl(v, userin, visit_cl_by_num);
            
            if(is_found == -1)
            {
                printf("输入的工号未注册，请重试\n\n");
                continue;
            }
        }

        if(strlen(str2) != 14 && strlen(str2) != 11 && strlen(str2) != 8)
        {
            printf("输入格式错误，无法识别\n\n");
            continue;
        }

        who = v->info_c + is_found;
        while(1)
        {
            printf("请输入密码:");
            password_in(passwd1);    //调用password_in函数输入密码

            md5_data(passwd1, strlen(passwd1), result_ps);

            if(strcmp(result_ps, who->passwd_cl) == 0)
            {
                system("clear");
                printf("登陆成功！\n");
                free(userin);
                sleep(1);
                return who;
            }
            else
            {
                system("clear");
                passtime++;
                printf("密码不正确...请重试, 还有%d次机会\n", (3 - passtime));    
            }
            if(passtime == 3)
            {
                printf("三次密码输入错误，请重新登陆\n");  
                break;
            }    
        }
    }
}


void sign_up_clerk(vector v)  //店员信息注册(店员信息初始化)
{
    clerks *userin;
    char tmp[7];
    char is_continue = 0;
    int is_found = 0;

    userin = (clerks*)calloc(1, sizeof(clerks));
    
    while(1)
    {
        while(1)
        {
            system("clear");
            printf("请输入员工号:");
            scanf("%s", userin->num_cl);
            while (getchar() != '\n') ;

            is_found = traverse_cl(v, userin, visit_cl_by_num);
            
            if(strlen(userin->num_cl) != 8)
            {
                printf("工号格式不正确，请输入8位数字\n");
                sleep(1);
                continue;
            }            
            if(is_found != -1)
            {
                printf("工号已被注册,请重新注册，谢谢配合。\n");
                sleep(1);
                continue;
            }
            break;        
        } 

        printf("请输入员工姓名: ");
        scanf("%30s", userin->name_cl);
        while (getchar() != '\n') ;
        
        while(1)
        {
            printf("请输入员工的手机号:");
            scanf("%11s", userin->phone_cl);
            if(!is_phone_num(userin->phone_cl))  //判断格式不正确时重新输入手机号
            {
                system("clear");
                printf("输入手机号格式不正确，请重新输入...\n");
                while(getchar() != '\n') ;
                continue;
            }
            system("clear");

            while(1)
            {
                printf("身份证号:");
                scanf("%18s", userin->id_cl);
                if(!is_id_num(userin->id_cl))
                {
                    printf("身份证号格式错误，请重新输入\n");
                    sleep(1);
                    system("clear");
                    continue;
                }
                else
                    break;
            }

            strncpy(tmp, &userin->phone_cl[5], 6);  //取手机号后六位为默认密码

            md5_data(tmp, strlen(tmp), userin->passwd_cl);   //将6位初始密码通过MD5方式存储
            
            system("clear");
            
            while(getchar() != '\n') ;
            printf("性别: ");
            scanf("%s", userin->sex_cl);
            while(getchar() != '\n') ;

            while(1)
            {
                printf("请输入卡号(首位为大写字母后13位为数字)或扫描员工卡\n");
                scanf("%14s", userin->card_cl);
                while(getchar() != '\n') ;

                system("clear");

                if(!is_card_num(userin->card_cl))
                {
                    printf("卡号异常，请重新输入或刷卡\n");
                    continue;
                }
                else
                    break;
            }
            time(&userin->first_in_cl);   //记录当前时间戳
            printf("%s 员工信息注册成功\n", userin->name_cl);
            break;
        }
        
        FILE* fp = fopen("./clerks/info_cl", "ab");
        if(NULL == fp)
        {
            fprintf(stderr, "open file error\n");
            return;
        }

        fwrite(userin, sizeof(clerks), 1, fp);
        push_back(v, userin);
        fclose(fp);
        
        printf("是否继续注册员工信息(y/Y.继续): \n");
            is_continue = getchar();
        if(is_continue != 'y' || is_continue != 'Y')
            break;
    }

    
}


void change_password_cl(vector v, clerks* data)
{
    char check_ps[33] = "";
    char tmp1[33] = "";
    char tmp2[33] = "";
    char result_ps[33] = "";
    char input_ps1[33] = "";
    char input_ps2[33] = "";
    unsigned int times = 0;
    unsigned int is_found = 0;
    int i;

    clerks* user = (clerks*)calloc(1, sizeof(clerks));
    if(NULL == user)
    {
        fprintf(stderr, "calloc error\n");
        return ;
    }

    for(i = 0; i < v->size + 1; i++)
    {
        if(strcmp(data->num_cl, v->info_c[i].num_cl) == 0)
            break;
    }
    is_found = i;
    *user = v->info_c[i];

    while(1)
    {
        printf("请输入原密码:");
        password_in(check_ps);
        md5_data(check_ps, strlen(check_ps), tmp1);
        if(strcmp(tmp1, user->passwd_cl) != 0)
        {
            times++;
            printf("密码错误，请重试(还有%d次机会)\n", 3 - times);
            
            if(times == 3)
                return ;
            
            sleep(1);
            continue;
        }
        else
        {
            system("clear");
            printf("验证成功！\n\n");
            break;
        }
    }

    while (1)   //用户名检测完毕，进入密码注册
    {
        printf("请输入密码: ");
        password_in(input_ps1);

        strcpy(tmp2, input_ps1);

        printf("再次输入确认:");
        password_in(input_ps2);

        if (strcmp(tmp2, input_ps2) != 0)
        {
            printf("两次输入密码不同，请重新输入\n");
            sleep(2);
            system("clear");
        }
        else
            break;
    }

    md5_data(input_ps1, strlen(input_ps1), result_ps);
    strcpy(user->passwd_cl, result_ps);    //将md5处理后的密码复制给结构体中密码
    
    FILE* fp = fopen("./clerks/info_cl", "rb+");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
    }
    fseek(fp, is_found * sizeof(clerks), SEEK_SET);   //文件位置指针往回移动一个结构体
    fwrite(user, sizeof(clerks), 1, fp);
    fclose(fp);

    system("clear");
    printf("修改密码成功！\n");
    free(user);

}
 