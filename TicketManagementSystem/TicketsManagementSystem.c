#include "stdafx.h"

#define VER "Beta 4.0"

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
    printf("        票务预订系统     " VER "        \n\n");
    printf("            Author: sfc9982           \n\n");
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
    printf("1) 管理员登陆\n");
    printf("2) 用户登陆\n");
    printf("0) 退出\n");
    printf(">> ");

    scanf("%d", &op);
    switch (op)
    {
        case 1:
            AdminLogIn();
            break;
        case 2:
            UserLogIn();
            break;
        case 0:
            DisplayExit();
            break;
        default:
            printf("Error: Input Error!\n");
            break;
    }
}

int getPassword(char *passwd, int size)
{
    int c;
    int n = 0;
    clock_t start_t;
    bool bFirst = true;
    const int TIMEOUT = 1;
    do
    {
        start_t = clock();
        while (true)
        {
            if (kbhit())
            {
                break;
            }
            if (bFirst == false && (clock() - start_t) / CLOCKS_PER_SEC >= TIMEOUT)
            {
                printf("\b*");
                break;
            }
        }
        c = getch();
        if (c != '\n' && c != '\r')
        {
            if (c == '\b')
            {
                if (n <= 0)
                    continue;
                printf("\b \b"); // 退格功能，两个\b负责删除当前和前一个字符
                n--;
                if (n == 0)
                    bFirst = 1;

            } else
            {
                passwd[n++] = (char) c;
                if (n >= 1)
                {
                    if (bFirst)
                    {
                        bFirst = 0;
                        printf("%c", c);
                    } else
                    {
                        printf("\b \b*%c", c);
                    }
                } // 助记
                else
                {
                    putchar('*'); // 遮蔽回显
                }
            }
        }

    } while (c != '\n' && c != '\r' && n < (size - 1)); // 不是所有平台行尾都是CRLF， size-1为'\0'预留位置，防止内存溢出
    passwd[n] = '\0';
    puts("\b*");
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
        strset(password, 0x0);
        free(password); // 清洗后及时释放内存，防止敏感数据泄漏，并减少内存碎片产生
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
        strset(password, 0x0);
        free(password);
        SalesmanInitMenu();
    } else
    {
        printf("\n账号或密码错误\n");
        system("pause");
        LogIn();
    }
}
