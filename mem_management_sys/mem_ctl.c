#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "members.h"


// 创建一个空链表（创建头节点）
list create_m(void)
{
    list l = (list)malloc(sizeof(struct _list));

    node* n = (node*)malloc(sizeof(node));
    
    if(NULL == n)
    {
        fprintf(stderr, "malloc error\n");
        return NULL;
    }
    n->next = NULL;
    l->head = l->tail = n;
    l->size_ml = 0;

    return l;
}

// 销毁某个链表（释放链表中所有节点）
void destroy_m(list l)
{
    node* p = NULL;
    
    while(l->head != NULL)
    {
        p = l->head->next;
        free(l->head);
        l->head = p;
    }

    free(l);
}


// 会员信息尾插
void add_mem(list l, members* new)   
{
    node* n = (node*)malloc(sizeof(node));  // 创建一个新节点

    if(NULL == n)
    {
        fprintf(stderr, "malloc error\n");
        return;
    }

    n->info_m = *new;
    n->next = NULL;

    // 将新节点加入到链表中
    l->tail->next = n;
    l->tail = n;
    l->size_ml++;
}

// 遍历链表
void trav_m(list l, int(*visit)(members*))
{
    node* p = l->head->next;  // 跳过头节点

    // 遍历链表所有节点
    while(p != NULL)
    {
        if(!visit(&(p->info_m)))
            break;

        p = p->next;
    }    
}

int show_mem(members* info)
{
    if(info->is_del == 0)
    {
        if(info->type_m == '1')
            printf("%s %s %s %s 普通会员 剩余次数:%d\n", info->id_m, info->name_m, info->sex_m, 
            info->phone_m, info->times);
        
        if(info->type_m == '2')
        {   
            time_t now_time = time(NULL);
            time_t re_time = info->end_tm - now_time;
            if(re_time < 0)
                printf("%s %s %s %s 高级会员 剩余时间:0天0时0分\n", info->id_m, info->name_m, info->sex_m, info->phone_m);
            else
                printf("%s %s %s %s 高级会员 剩余时间:%ld天%ld时%ld分\n", info->id_m, info->name_m, info->sex_m, 
            info->phone_m, re_time / 86400, re_time % 86400 / 3600, re_time % 86400 % 3600 / 60);
        }
    }
    return 1;
}


// 查询、删除或修改会员信息
int traverse_m(list l, members* new_data, int(*visit)(members*, members*))
{
    node* p = l->head->next;
    unsigned int fnd = 0;
    int is_find = 0;

    while(p != NULL)    // 遍历链表找到目标节点执行visit函数
    {    
        if(!visit(&p->info_m, new_data))
        {
            is_find = 1;
            break;
        }
        fnd++;
        p = p->next;
    }
    if(is_find == 0) 
        return -1;

    return fnd;
}


int visit_m_by_id_show(members* data, members* wanna)
{
    if(strcmp(data->id_m, wanna->id_m) == 0)
    {
        show_mem(data);
        return 0;
    }
    return 1;
}

int visit_m_by_name_show(members* data, members* wanna)
{
    if(strstr(data->name_m, wanna->name_m) != NULL)
    {
        show_mem(data);
    }
    return 1;
}

int visit_m_by_phone_show(members* data, members* wanna)
{
    if(strcmp(data->phone_m, wanna->phone_m) == 0)
    {
        show_mem(data);
        return 0;
    }
    return 1;
}

int visit_m_by_type_show(members* data, members* wanna)
{
    if(data->type_m == wanna->type_m)
    {
        show_mem(data);
    }
    return 1;
}

int visit_m_by_card_show(members* data, members* wanna)
{
    if(strcmp(data->card_m, wanna->card_m) == 0)
    {
        show_mem(data);
        return 0;
    }
    return 1;
}

int visit_m_by_id(members* a, members* b)
{
    if(strcmp(a->id_m, b->id_m) == 0)
        return 0;
    return 1;
}

int visit_m_by_phone(members* a, members* b)
{
    if(strcmp(a->phone_m, b->phone_m) == 0)
        return 0;
    return 1;
}

int visit_m_by_card(members* a, members* b)
{
    if(strcmp(a->card_m, b->card_m) == 0)
        return 0;
    return 1;
}


void watch_remain_tm(members* data)
{
    time_t now_tm = time(NULL);
    time_t re_tm =  data->end_tm - now_tm;
    time_t day, hour, min;

    if(re_tm > 0)
    {
        day = re_tm / (24 * 60 * 60);
        hour = re_tm % 86400 / 3600;
        min = re_tm % 86400 % 3600 / 60;
        printf("剩余时长为:  %ld天 %ld小时 %ld分钟\n", day, hour, min);
    }

    else
        printf("很抱歉您已超时，请联系店员续卡\n");
}

void my_sort_m(list l, int (*cmp) (const void*, const void*))
{
    // 采用选择排序算法，交换数据域
    node* p = NULL, *q = NULL, *k = NULL;
    members tmp;

    p = l->head->next;

    if(p == NULL) return;

    while(p->next != NULL)
    {
        q = p->next;
        k = p;

        while(q != NULL)
        {
            if(cmp(&(q->info_m), &(k->info_m)) < 0)
                k = q;

            q = q->next;
        }

        if(k != p)
        {
            // 交换两个节点的数据域
            tmp = k->info_m;
            k->info_m = p->info_m;
            p->info_m = tmp;
        }

        p = p->next;
    }
}


int cmp_m_id(const void* a, const void* b)
{
    members* data1 = (members*)a;
    members* data2 = (members*)b;
    return strcmp(data1->id_m, data2->id_m);
}

int cmp_m_card(const void* a, const void* b)
{
    members* data1 = (members*)a;
    members* data2 = (members*)b;
    return strcmp(data1->card_m, data2->card_m);
}

int cmp_m_type(const void* a, const void* b)
{
    members* data1 = (members*)a;
    members* data2 = (members*)b;
    return data1->type_m - data2->type_m;
}

void del_m(list l)
{
    int is_found = 0;
    char str1[30] = "";
    members* n = (members*)calloc(1, sizeof(members));
    while(1)
    {
        printf("请输入需要销卡会员的ID或手机号(或刷卡)\n");
        scanf("%s", str1);

        if (strlen(str1) == 14)
        {
            strcpy(n->card_m, str1);
            is_found = traverse_m(l, n, visit_m_by_card);
            if ((is_found != -1) && n->is_del == 0)
            {
                printf("刷卡成功\n");
                return ;
            }
            printf("会员卡错误或者已销卡\n");
            sleep(1);
            system("clear");
            continue;
        }

        if (strlen(str1) == 11)
        {
            int is_found = 0;

            strcpy(n->phone_m, str1);
            is_found = traverse_m(l, n, visit_m_by_phone);
            // 未找到手机号
            if (is_found == -1)     
            {
                printf("输入的手机号未注册，请重试\n\n");
                continue;
            }
            // 已经删除或者逾期
            if (n->is_del != 0)
            {
                printf("用户已逾期，无需删除\n\n");
                continue;
            }
        }

        if (strlen(str1) == 6)
        {
            strcpy(n->phone_m, str1);
            is_found = traverse_m(l, n, visit_m_by_id);
            // 未找到ID
            if (is_found == -1)
            {
                printf("输入的ID未注册，请重试\n\n");
                continue;
            }
            if (n->is_del != 0)
            {
                printf("用户已销卡，无需删除\n\n");
                continue;
            }
        }

        if (strlen(str1) != 14 && strlen(str1) != 11 && strlen(str1) != 6)
        {
            printf("输入格式错误，无法识别\n\n");
            continue;
        }

        break;
    }
    
    node* p = l->head->next;
    while(is_found > 0)
    {
        p = p->next;
        is_found--;
    }

    printf("用户信息如下:\n");
    show_mem(&p->info_m);
    watch_remain_tm(&p->info_m);
    printf("确定要删除吗(输入“yes”删除):\n");

    char del_really[10] = "";
    scanf("%s", del_really);
    if(strcmp(del_really, "yes") != 0)
    {
        printf("oh no 未删除信息\n");
        sleep(1);
        
        free(p);
        return ;
    }
    else
    {
        p->info_m.is_del = 1;   //删除标志量置 1
        p->info_m.end_tm = 0;
        printf("%s用户销卡成功，我们江湖再见!\n", p->info_m.name_m);
        
        FILE* fp = fopen("./members/info_m", "rb+");
        if(NULL == fp)
        {
            fprintf(stderr, "open file error\n");
        }

        fseek(fp, is_found * sizeof(members), SEEK_SET);

        fwrite(&p->info_m, sizeof(members), 1, fp);

        free(p);
        fclose(fp);
    }
}

void look_activity()
{
    FILE*fp = fopen("./doc/activity", "rb");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
        return ;
    }

    char str1[300] = "";

    while(fread(str1, sizeof(str1) - 1, 1, fp) != 0)
    {
        system("clear");
        printf("%s\n", str1);
        printf("\n\n1.下一页  0.退出\n");
        char ch = getchar();
        if(ch == '0')
        {
            system("clear");
            fclose(fp);
            break;
        }
    }
    
    printf("浏览完毕, 回车返回\n");
    getchar();
    system("clear");
}


void look_manual()
{
    FILE*fp = fopen("./doc/manual", "rb");
    if(NULL == fp)
    {
        fprintf(stderr, "open file error\n");
        return ;
    }

    char str1[300] = "";

    while(fread(str1, sizeof(str1) - 1, 1, fp) != 0)
    {
        system("clear");
        printf("%s\n", str1);
        printf("\n\n1.下一页  0.退出\n");
        char ch = getchar();
        if(ch == '0')
        {
            system("clear");
            fclose(fp);
            break;
        }
    }
    
    printf("浏览完毕, 回车返回\n");
    getchar();
    system("clear");
}


void add_time(list l)
{
    node* p = (node*)malloc(sizeof(node)); 
    
    while(1)
    {
        int po_infile;

        while(1)
        {
            po_infile = 0;
            int is_found = 0;
            char wanna[30] = "";  
            p = l->head->next;
            printf("请输入会员手机号(或刷卡)\n");
            scanf("%s", wanna);
            while(getchar() != '\n') ;

            if(strlen(wanna) == 14)
            {
                while(p != NULL)
                {
                    if(strcmp(p->info_m.card_m, wanna) == 0)
                    {
                        is_found++;
                        break;
                    }
                    po_infile++;
                    p = p->next;
                }
                if(is_found == 0)
                    printf("很抱歉 没有找到相关信息，请重试\n"); 
            }

            else if(strlen(wanna) == 11)
            {
                while(p != NULL)
                {
                    if(strcmp(p->info_m.phone_m, wanna) == 0)
                    {
                        is_found++;    
                        break;
                    }
                    po_infile++;
                    p = p->next;
                }
                if(is_found == 0)
                    printf("很抱歉,没有找到相关信息,请重试\n"); 
            }

            else
            {
                printf("输入格式不正确，请重试\n");
            }

            if(is_found > 0)    break;
        }

        system("clear");
        if(p->info_m.type_m == '1')
        {
            int increase = 0;
            printf("会员信息如下:\n");
            show_mem(&p->info_m);
            printf("请输入增加的次数: ");
            scanf("%d", &increase);
            p->info_m.times += increase;
            printf(" %s 会员续卡成功", p->info_m.name_m);
        }

        if(p->info_m.type_m == '2')
        {
            time_t incre_time = 0;
            int hour = 0, day = 0;
            printf("会员信息如下:\n");
            show_mem(&p->info_m);
            printf("\n请输入要增加的时间:(_天_小时)：");
            scanf("%d%d", &day, &hour);
            while(getchar() != '\n') ;
            
            incre_time += ((hour * 3600) + (day * 24 * 3600));

            p->info_m.last_tm = time(NULL);
            
            if(p->info_m.last_tm < p->info_m.end_tm)
                p->info_m.end_tm += incre_time;
            else
                p->info_m.end_tm = p->info_m.last_tm + incre_time;
        }
        

        FILE* fp = fopen("./members/info_m", "rb+");
        if(NULL == fp)
        {
            fprintf(stderr, "open file error\n");
        }

        fseek(fp, po_infile * sizeof(members), SEEK_SET);

        fwrite(&p->info_m, sizeof(members), 1, fp);
        fclose(fp);
        
        printf("%s续卡成功\n", p->info_m.name_m);
        printf("\n\n是否继续办理续卡:(y/Y.继续)\n");

        char ch = getchar();
        if(ch != 'y' || ch != 'Y')
            break;
    }
}
