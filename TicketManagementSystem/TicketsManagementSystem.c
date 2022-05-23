#include "stdafx.h"

int main()
{
    DisplayInit();
    LogIn();

    system("pause");

    return 0;
}


void DisplayInit()
{
    system("cls");
    printf("--------------------------------------\n\n");
    printf("           票务预订系统  V1.0           \n\n");
    printf("        Author: sfc9982 (陈宇翱)       \n\n");
    printf("--------------------------------------\n\n");
    getchar();
}

void DisplayExit()
{
    system("cls");
    printf("--------------------------------------\n\n");
    printf("               再见！                  \n\n");
    printf("--------------------------------------\n\n");
    getchar();
}

void LogIn()
{
    system("cls");
    int op = -1;
    printf("1) 管理员登陆   \n");
    printf("2) 用户登陆    \n");
    printf("0) 退出        \n");
    printf(">> ");

    scanf("%d", &op);
    if (op == 1)
        AdminLogIn();
    else if (op == 2)
        UserLogIn();
    else if (op == -1)
        printf("Error: Input Error!\n");
    else
        DisplayExit();
}

int getPassword(char *passwd, int size)
{
    int c;
    int n = 0;

//    printf("Please Input password:");

    do
    {
        c = getch();
        if (c != '\n' && c != '\r')
        {
            putchar('*');
            passwd[n++] = (char) c;
        }
    } while (c != '\n' && c != '\r' && n < (size - 1)); // 不是所有平台行尾都是CRLF， size-1为'\0'预留位置，防止内存溢出
    passwd[n] = '\0';
    return n;
}

void AdminLogIn()
{
    char account[MAX_USERNAME_LENGTH];
    char *password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));

    system("cls"); // 清屏

    printf("管理员登陆\n\n");

    printf("账号\n>> ");
    scanf("%s", account);

    printf("密码\n>> ");
    getPassword(password, MAX_PASSWORD_LENGTH); // 无回显输入密码，防止敏感信息泄露

    if (IsAdminAccount(account, password))
    {
        free(password); // 及时释放内存，防止敏感数据泄漏，并减少内存碎片产生
        AdminInitMenu();
    } else
    {
        printf("\n账号或密码错误\n");
        system("pause");
        LogIn();
    }
}

void UserLogIn()
{
    char account[MAX_USERNAME_LENGTH];
    char *password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));

    system("cls");

    printf("用户登陆\n\n");

    printf("账号\n>> ");
    scanf("%s", account);

    printf("密码\n>> ");
    getPassword(password, MAX_PASSWORD_LENGTH);

    if (IsSalesmanAccount(account, password))
    {
        free(password);
        SalesmanInitMenu();
    } else
    {
        printf("\n账号或密码错误\n");
        system("pause");
        LogIn();
    }
}
