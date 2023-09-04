#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "list.h"
#include "password.h"
#include "members.h"
#include "clerk.h"
#include "boss.h"
#include "arrive.h"


void list_main()
{
    int check = 0;
    do
    {
        check = is_boss();
    }while(check != 1);
    
    list l = create_m();
    vector v = create_cl(20);
    vec a = create_a(50);

    //会员信息链表创建及从文件中读取节点数据域信息
    FILE* fp1 = fopen("./members/info_m", "rb+");
    if(NULL == fp1)
    {
        perror("open file failed");
        return ;
    }
    members* new_m = (members*)calloc(1, sizeof(members));

    while(fread(new_m, sizeof(members), 1, fp1))
    {
        add_mem(l, new_m);
    }
    fclose(fp1);

    //店员信息顺序表创建及从文件中读取元素信息
    FILE* fp2 = fopen("./clerks/info_cl", "rb+");
    if(NULL == fp2)
    {
        perror("open file failed");
        return ;
    }
    clerks* new_cl = (clerks*)calloc(1, sizeof(clerks));

    while(fread(new_cl, sizeof(clerks), 1, fp2))
    {
        push_back(v, new_cl);
    }
    fclose(fp2);
    //到店信息顺序表
    FILE* fp3 = fopen("./arrival/time_arr", "rb+");
    if(NULL == fp3)
    {
        perror("open file failed");
        return ;
    }
    arrive* new_a = (arrive*)calloc(1, sizeof(arrive));

    while(fread(new_a, sizeof(arrive), 1, fp3))
    {
        add_vec(a, new_a);
    }
    fclose(fp3);

    while(1)    // l(会员信息链表)  v(店员信息顺序表)
    {
        int user = 0;
        system("clear");

        printf("<:欢迎使用商品信息管理系统:>\n\n\n");
        printf("       < 1.会员 >       \n");
        printf("       < 2.店员 >       \n");
        printf("       < 9.店长 >       \n");
        printf("       < 0.退出 >       \n\n");
        printf("\n<请选择您的身份>1.顾客 2.店员 9.店长 0.退出\n");
        printf(">:");
        
        scanf("%d", &user);
        while(getchar() != '\n') ;
        
        switch(user)
        {
            case 1:
                list_mem(l, a);
                break;
            case 2:
                list_clerk(l, v, a);
                break;
            case 9:
                list_boss(l, v, a);
                break;
            case 0:
                printf("谢谢使用，再见!\n");
                sleep(1);
                system("clear");
                return;
            default:
                printf("选择错误，请重新选择\n");
        }
    }
}


void list_mem(list l, vec a)
{
    while(1)
    {
        system("clear");

        printf( "\n\n\n"
                "           欢迎光临黎生乒乓球馆             \n\n\n"
                "                                            \n"
                "                @@@@@@@                     \n"
                "              @@@@@@@@@@@                   \n"
                "             @@@@@@@@@@@@@                  \n"
                "            @@@@@@@@@@@@@@@                 \n"
                "            @@@@@@@@@@@@@@@                 \n"
                "            @@@@@@@@@@@@@@@     @@          \n"
                "             @@@@@@@@@@@@@      @@          \n"
                "              @@@@@@@@@@                    \n"
                "               @@@@@@@                      \n"
                "              @@@                           \n"
                "             @@@                            \n"
                "            @@@                             \n"
                "                                            \n");
        
        printf("\n\n            < 按回车键继续 >\n\n\n");
        
        char ch;
        while(ch = getchar() != '\n') ;

        system("clear");
        members* p = (members*)calloc(1, sizeof(members));
        p = log_in_mem(l);
        if(NULL == p)   //验证失败
            continue;
        arr_m_in(a, p);

        while(1)
        {
            int chos = 0;
            printf("\n\n                                       \n");
            printf("         < 会员模式 >                   \n");
            printf("         < 1.查看当前信息 >             \n");
            printf("         < 2.查询剩余时间/次数 >           \n");
            printf("         < 3.查看门店活动 >        \n");
            printf("         < 4.查看用户手册 >        \n");
            printf("         < 5.修改密码 >                 \n");
            printf("         < 6.查看到店信息 >                 \n");
            printf("         < 0.注销 >                     \n");
            printf("         < 9.返回主菜单(需要店长权限) >    \n\n");
            printf("请选择 >:");
            scanf("%d", &chos);
            while(getchar() != '\n') ;

            switch (chos)
            {
                case 0:
                    system("clear");
                    free(p);            //退出释放当前用户的数据信息指针
                    printf("感谢您的使用,下次再见哦!\n");
                    sleep(2);
                    break;
                case 1:
                    system("clear");
                    show_mem(p);
                    printf("\n\n< 回车键返回 >\n");
                    char ch;
                    while(ch = getchar() != '\n') ;
                    system("clear");
                    break;
                case 2:
                    system("clear");
                    if(p->type_m == '1')
                    {
                        printf("剩余次数:%d\n", p->times);
                        printf("\n\n回车返回\n");
                        getchar();
                        system("clear");
                    }
                    else
                        watch_remain_tm(p);
                    break;
                case 3:
                    system("clear");
                    look_activity();
                    break;
                case 4: 
                    system("clear");
                    look_manual();
                    break;
                case 5:    
                    system("clear");
                    change_password_m(l, p);
                    break;
                case 6:    
                    system("clear");
                    char wanna[33] = "";
                    strcpy(wanna, p->name_m);
                    traverse_a(a, wanna, visit_a_name);
                    break;
                case 9:
                    system("clear");
                    int ex = is_boss();
                    if(ex == 1)
                        return ;
                    else
                        break;
                default:
                    printf("选择错误 请重试\n");
            }

            if(chos == 0)
                break;
        }
    }
}


void list_clerk(list l, vector v, vec a)
{   
    while(1)
    {   
        system("clear");
        clerks* p = (clerks*)calloc(1, sizeof(clerks));
        p = log_in_clerk(v);
        arr_cl_in(a, p);
        
        while(1)
        {
            system("clear");
            int chos = 0;
            printf("    %s  今天也要努力工作哦！\n\n", p->name_cl);
            printf("         < 营业模式 >                           \n");
            printf("         < 1.添加会员用户 >                    \n");
            printf("         < 2.会员续卡 >                         \n");
            printf("         < 3.查询会员信息 >                     \n");
            printf("         < 4.会员销卡 >                     \n");
            printf("         < 5.查看当前店员信息 >                 \n");
            printf("         < 6.会员信息一览 >                     \n");
            printf("         < 7.修改店员密码 >                     \n");
            printf("         < 8.到店信息一览 >                     \n");
            printf("         < 0.注销 >                            \n");
            printf("         < 9.返回主菜单(需要店长权限) >       \n\n");
            printf("\n请选择 >:");
            scanf("%d", &chos);
            while(getchar() != '\n') ;

            switch (chos)
            {
                case 0:
                    system("clear");
                    printf("工作结束啦，明天也要元气满满哦!\n");
                    sleep(2);
                    break;
                case 1: 
                    system("clear");
                    sign_up_mem(l);          //会员注册
                    break;
                case 2:
                    system("clear");
                    add_time(l);             //会员续卡 
                    break;
                case 3:
                    system("clear");
                    list_search_m(l);        //查询会员信息
                    break;
                case 4:
                    system("clear");
                    del_m(l);
                    break;
                case 5:
                    system("clear");
                    show_cl(p);            //打印当前店员信息
                    printf("\n\n\n         < 回车退出 >\n");
                    while(getchar() != '\n') ;
                    break;
                case 6:
                    system("clear");
                    list_catch_all(l);
                    break;
                case 7:
                    system("clear");
                    change_password_cl(v, p);
                    break;
                case 8:
                    system("clear");
                    traverse_a(a, p->name_cl, visit_a_clerks);
                    printf("\n\n         < 回车退出 >\n");
                    while(getchar() != '\n') ; 
                    break;
                case 9:
                    system("clear");
                    int ex = is_boss();
                    if(ex == 1)
                        return ;
                    else
                        break;
                default:
                    printf("选择错误 请重试\n");
                    break;
            }

            if(chos == 0)
                break;
        }
    }
}



void list_search_m(list l)
{
    while(1)
    {
        system("clear");
        int choose = 0;
        members* new = (members*)calloc(1, sizeof(members));

        printf("       查询信息          \n");
        printf("      < 1.ID >          \n");
        printf("      < 2.姓名 >        \n");
        printf("      < 3.手机号 >       \n");
        printf("      < 4.会员类型 >      \n");
        printf("      < 5.会员卡号 >      \n");
        printf("      < 0.退出 >          \n\n");
        printf("\n请选择>:");
        scanf("%d", &choose);
        while(getchar() != '\n') ;
        char ch;

        switch (choose)
        {
            case 0:
                return;
            case 1:
                while(1)
                {
                    system("clear");
                    printf("请输入ID:");
                    scanf("%6s", new->id_m);
                    while(getchar() != '\n');
                    if(traverse_m(l, new, visit_m_by_id_show) < 0)
                        printf("未找到符合条件的信息\n");
                    sleep(1);
                    printf("\n\n是否继续查找ID(y/Y.继续): ");
                    ch = getchar();
                    if(ch == 'y' || ch == 'Y')
                        continue;
                    else
                        break;
                }
                break;
            case 2:
                while(1)
                {
                    system("clear");
                    printf("请输入姓名:");
                    scanf("%s", new->name_m);
                    while(getchar() != '\n');
                    if(traverse_m(l, new, visit_m_by_name_show) < 0)
                        printf("未找到符合条件的信息\n");
                    sleep(1);
                    printf("\n\n是否继续查找姓名(y/Y.继续): ");
                    ch = getchar();
                    if(ch == 'y' || ch == 'Y')
                        continue;
                    else
                        break;
                }
                break;
            case 3:
                while(1)
                {
                    system("clear");
                    printf("请选择要查询的手机号:");
                    scanf("%11s", new->phone_m);
                    while(getchar() != '\n');   
                    if(traverse_m(l, new, visit_m_by_phone_show) < 0)
                        printf("未找到符合条件的信息\n");
                    sleep(1);
                    printf("\n\n是否继续查找手机号(y/Y.继续): ");
                    ch = getchar();
                    if(ch == 'y' || ch == 'Y')
                        continue;
                    else
                        break;
                }
                break;
            case 4:
                while(1)
                {
                    system("clear");
                    printf("请选择要查询的类型(1.普通会员  2.高级会员):");
                    scanf("%c", &new->type_m);
                    while(getchar() != '\n') ;
                    if(traverse_m(l, new, visit_m_by_type_show) < 0)
                        printf("未找到符合条件的信息\n");
                    sleep(1);
                    printf("\n\n是否继续查找类型(y/Y.继续): ");
                    ch = getchar();
                    if(ch == 'y' || ch == 'Y')
                        continue;
                    else
                        break;
                }
                break;
            case 5:
                while(1)
                {
                    system("clear");
                    printf("请输入会员卡号:");
                    scanf("%14s", new->card_m);
                    while(getchar() != '\n');
                    if(traverse_m(l, new, visit_m_by_card_show) < 0)
                        printf("未找到符合条件的信息\n");
                    sleep(1);
                    printf("\n\n是否继续查找卡号(y/Y.继续): ");
                    ch = getchar();
                    if(ch == 'y' || ch == 'Y')
                        continue;
                    else
                        break;
                }
                break;        
            default:
                printf("选择错误，请重试\n");
                sleep(1);
                break;
        }
        while(getchar() != '\n') ;
    }
}


void list_catch_all(list l)
{
    int choose = 0;
    system("clear");
    trav_m(l, show_mem);
    printf("\n\n           < 1.ID排序 >\n");
    printf("           < 2.卡号排序 >\n");
    printf("           < 3.类型排序 >\n");
    printf("           < 0.退出 >\n");
    printf(">:");
    scanf("%d%*c", &choose);

    switch(choose)
    {
        case 0:
            return ;
        case 1:
            my_sort_m(l, cmp_m_id);
            trav_m(l, show_mem);
            printf("\n\n回车键继续\n");
            while(getchar() != '\n');
            break;
        case 2:
            my_sort_m(l, cmp_m_card);
            trav_m(l, show_mem);
            printf("\n\n回车键继续\n");
            while(getchar() != '\n');
            break;
        case 3:
            my_sort_m(l, cmp_m_type);
            trav_m(l, show_mem);
            printf("\n\n回车键继续\n");
            while(getchar() != '\n');
            break;
        default:
            printf("选择错误，请重试\n");
            sleep(1);
            break;
    }
}


void list_boss(list l, vector v, vec a)
{
    while(1)
    {
        system("clear");
        
        int choose = 0;
        printf(" 店长 今天也要努力工作哦！\n\n");
        printf("         < 店长模式 >                   \n");
        printf("         < 1.查看会员信息 >                \n");
        printf("         < 2.查看店员信息 >                \n");
        printf("         < 3.店员密码找回 >             \n");
        printf("         < 4.会员密码找回 >             \n");
        printf("         < 5.添加店员 >             \n");
        printf("         < 6.店员离职 >              \n");
        printf("         < 7.修改密码>                  \n");
        printf("         < 8.查看全员到店信息>           \n");
        printf("         < 0.注销 >                    \n");
        printf("         < 9.初始化系统(需要店长权限) >    \n");
        printf("请选择 >:");
        scanf("%d", &choose);
        while(getchar() != '\n') ;

        switch (choose)
        {
            case 0:
                system("clear");
                int ex = is_boss();
                if(ex == 1)
                {   
                    printf("店长模式退出，下次再见哦\n"); 
                    sleep(2);
                    return ;
                }
                else
                    break;
            case 1: 
                system("clear");
                trav_m(l, show_mem);
                printf("\n\n          回车返回\n\n");
                while(getchar() != '\n');
                break;
            case 2:
                system("clear");
                trav_cl(v, show_cl);
                printf("\n\n          回车返回\n\n");
                while(getchar() != '\n');
                break;
            case 3:
                system("clear");
                re_cl_password(v);
                break;
            case 4:
                system("clear");
                re_m_password(l);
                break;
            case 5:
                system("clear");
                sign_up_clerk(v);
                break;
            case 6:
                system("clear");
                del_cl(v);
                break;
            case 7:
                system("clear");
                change_password_boss();
                break;
            case 8:
                system("clear");
                trav_a(a, show_a);
                printf("\n\n          回车返回\n\n");
                while(getchar() != '\n');
                break;    
            case 9:
                system("clear");
                initial_sys();
            default:
                printf("选择错误 请重试\n");
                break;
        }
    }
}


