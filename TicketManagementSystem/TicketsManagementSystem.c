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
    printf("           Ʊ��Ԥ��ϵͳ   V1.0          \n\n");
    printf("            Author: sfc9982           \n\n");
    printf("--------------------------------------\n\n");
    getchar();
}

void DisplayExit()
{
    system("cls");
    printf("--------------------------------------\n\n");
    printf("               �ټ���                  \n\n");
    printf("--------------------------------------\n\n");
    getchar();
}

void LogIn()
{
    system("cls");
    int op = -1;
    printf("1) ����Ա��½\n");
    printf("2) �û���½\n");
    printf("0) �˳�\n");
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
        case -1:
            printf("Error: Input Error!\n");
            break;
        default:
            DisplayExit();
    }
}

int getPassword(char *passwd, int size)
{
    int c;
    int n = 0;

    do
    {
        c = getch();
        if (c != '\n' && c != '\r')
        {
            putchar('*'); // �ڱλ���
            passwd[n++] = (char) c;
        }
    } while (c != '\n' && c != '\r' && n < (size - 1)); // ��������ƽ̨��β����CRLF�� size-1Ϊ'\0'Ԥ��λ�ã���ֹ�ڴ����
    passwd[n] = '\0';
    return n;
}

void AdminLogIn()
{
    char account[MAX_USERNAME_LENGTH];
    char *password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));

    system("cls"); // ����

    printf("����Ա��½\n\n");

    printf("�˺�\n>> ");
    scanf("%s", account);

    printf("����\n>> ");
    getPassword(password, MAX_PASSWORD_LENGTH); // �޻����������룬��ֹ������Ϣй¶

    if (IsAdminAccount(account, password))
    {
        strset(password, 0x0);
        free(password); // ��ϴ��ʱ�ͷ��ڴ棬��ֹ��������й©���������ڴ���Ƭ����
        AdminInitMenu();
    } else
    {
        printf("\n�˺Ż��������\n");
        system("pause");
        LogIn();
    }
}

void UserLogIn()
{
    char account[MAX_USERNAME_LENGTH];
    char *password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));

    system("cls");

    printf("�û���½\n\n");

    printf("�˺�\n>> ");
    scanf("%s", account);

    printf("����\n>> ");
    getPassword(password, MAX_PASSWORD_LENGTH);

    if (IsSalesmanAccount(account, password))
    {
        strset(password, 0x0);
        free(password);
        SalesmanInitMenu();
    } else
    {
        printf("\n�˺Ż��������\n");
        system("pause");
        LogIn();
    }
}
