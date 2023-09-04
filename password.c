#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static int getch(void);

//密码键入 参数为指针 用来存放输出的密码字符串
void password_in(char *passwd)
{
    int i = 0;

    fflush(stdout);

    while (1)
    {
        char c = getch();
        if (c == '\n')
        {
            break;
        }
        
        if(c == 127)
        {
            if(i > 0)
            {
                printf("\b \b");
                fflush(stdout);
                i--;
            }
        }
        else
        {
            passwd[i++] = c;
            printf("*");
            fflush(stdout);
        }
    }
    passwd[i] = '\0';
    printf("\n");
}

static int getch(void)
{
    struct termios old_term, new_term;
    int ch;

    // 获取终端设置
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;

    // 设置终端模式为不回显字符
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    // 读取字符
    ch = getchar();

    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

    return ch;
}

//判断卡号合理性
int is_card_num(char* num)
{
    char* tmp = num;
    int i = 0;
    
    if(strlen(tmp) != 14)        //卡号长度为14
        return 0;   
    if(*(tmp) < 'A' || *(tmp) > 'Z')  //卡号第一位必须为字母(大写)
        return 0;
    tmp++;
    for(i = 0; i < 13; i++) //后13位字符串中没有非数字字符
    {
        if(*tmp < '0' || *tmp > '9')
            return 0;
        tmp++;
    }
    return 1;
}

//判断手机号合理性
int is_phone_num(char* num)
{
    char* tmp = num;
    int i = 0;
    
    if(strlen(tmp) != 11)        //手机号长度为11
        return 0;   
    if(*(tmp) != '1')       //手机号第一位必须为 ‘1’
        return 0;
    if(*(tmp + 1) == '0' || *(tmp + 1) == '1' || *(tmp + 1) == '2') //第二位不能为 ‘0’ ‘1’ ‘2’
        return 0;
    for(i = 0; i < 11; i++) //11位字符串中没有非数字字符
    {
        if(*tmp < '0' || *tmp > '9')
            return 0;
        tmp++;
    }
    return 1;
}


int is_id_num(char* num)        //判断身份证号合理性
{
    char* tmp = num;
    char ch[6] = "42010";
    int i = 0;
    
    if(strlen(tmp) != 18)        //身份证号长度为18
        return 0;   
    if(strncmp(tmp, ch, 5))
        return 0;
    
    tmp += 5;
    for(i = 0; i < 12; i++) //前17位字符串中没有非数字字符
    {
        if(*tmp < '0' || *tmp > '9')
            return 0;
        tmp++;
    }
    return 1;
}
