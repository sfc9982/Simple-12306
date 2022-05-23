#include "stdafx.h"


//�ж��Ƿ�Ϊ����Ա�˻����޷����ļ�����-1���Ƿ���1���񷵻�0
int IsAdminAccount(char *account, char *password)
{
    FILE *fp = fopen("Data\\user.dat", "rb");

    if (fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp) / sizeof(UserAccount);
    fseek(fp, 0, SEEK_SET);

    UserAccount *user = (UserAccount *) malloc(size * sizeof(UserAccount));

    for (int i = 0; i < size; i++)
    {
        fread(user + i, sizeof(UserAccount), 1, fp);
        if (strcmp(account, user[i].account) == 0 && strcmp(password, user[i].password) == 0
            && user[i].permission_level == 1)
        {
            puts("Login Success!");
            return 1;
        }
    }

    free(user);
    fclose(fp);

    return 0;
}

//�����˻���Ϣ���޷��򿪷���-1���ɹ�����1���˻��Ѵ��ڷ���0���������룩
int AddAccount(UserAccount *newInfo)
{
    FILE *fp = fopen("Data\\user.dat", "rb+");

    if (fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp) / sizeof(UserAccount);
    fseek(fp, 0, SEEK_SET);

    int index = -1;
    UserAccount *user = (UserAccount *) malloc((size + 1) * sizeof(UserAccount));
    for (int i = 0; i < size; i++)
    {
        fread(user + i, sizeof(UserAccount), 1, fp);
        if (strcmp(user[i].account, newInfo->account) == 0)
        {
            return 0;
        }
    }

    user[size] = *newInfo;
    fseek(fp, 0, SEEK_SET);

    for (int i = 0; i <= size; i++)
    {
        fwrite(user + i, sizeof(UserAccount), 1, fp);
    }

    free(user);
    fclose(fp);
    return 1;

}


//���������û������룬�ļ��޷��򿪷���-1��ɾ���ɹ�����1��ʧ�ܷ���0
int DeleteAccount(char *account)
{
    FILE *fp = fopen("Data\\user.dat", "rb+");

    if (fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp) / sizeof(UserAccount);
    fseek(fp, 0, SEEK_SET);

    UserAccount *user = (UserAccount *) malloc(size * sizeof(UserAccount));
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        fread(user + i, sizeof(UserAccount), 1, fp);
        if (strcmp(user[i].account, account) == 0)
        {
            index = i;
        }

    }
    fclose(fp);

    if (index != -1)
    {
        fp = fopen("Data\\user.dat", "wb+");

        for (int i = 0; i < size; i++)
        {
            if (i == index) continue;
            //printf("look: %s %s %d\n", user[i].account, user[i].password, user[i].permission_level);
            fwrite(user + i, sizeof(UserAccount), 1, fp);
        }

        fclose(fp);
    }

    free(user);

    return index != -1 ? 1 : 0;
}


//��ʾϵͳ�������˻���Ϣ���޷��򿪷���-1
int DisplayAccountInfo()
{
    FILE *fp = fopen("Data\\user.dat", "rb");

    if (fp == NULL)
        return -1;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp) / sizeof(UserAccount);
    fseek(fp, 0, SEEK_SET);

    UserAccount *own = (UserAccount *) malloc(size * sizeof(UserAccount));
    printf("--------------------------------------\n");
    printf("%-10s %-10s  %-2s\n", "�˺�", "����", "Ȩ��");
    for (int i = 0; i < size; i++)
    {
        fread(own + i, sizeof(UserAccount), 1, fp);
        if (own[i].permission_level)
            printf("%-10s %-10s %-2s\n", own[i].account, own[i].password, "����Ա");
        else
            printf("%-10s %-10s %-2s\n", own[i].account, own[i].password, "�û�");
    }
    printf("--------------------------------------\n");

    free(own);
    fclose(fp);

    return 1;
}


void AdminInitMenu()
{
    system("cls");
    int op = -1;

    printf("1) ��¼����\n");
    printf("2) �û�����\n");
    printf("3) ���۳���\n");
    printf("4) ����ͳ��\n");
    printf("0) �˳���½\n");

    printf(">> ");
    scanf("%d", &op);

    if (op == 1)
        StockManagement();
    else if (op == 2)
        UserManagement();
    else if (op == 3)
        SoldManagement();
    else if (op == 4)
        SoldStatistics();
    else
        LogIn();
}


void StockManagement()
{
    system("cls");
    int op = -1;

    printf("1) ��ӳ���\n");
    printf("2) �������\n");
    printf("3) �鿴����\n");
    printf("4) ��ѯվ��\n");
    printf("5) ������\n");
    printf("0) �����ϼ�\n");

    printf(">> ");
    scanf("%d", &op);

    TicketsList head = InitTicketsList();
    ImportTicketsFromFile(head, OpenTicketsFile("r"));

    if (op == 1)
        AddTicketsToStock(head);
    else if (op == 2)
        BatchedStock(head);
    else if (op == 3)
        LookOverStock(head);
    else if (op == 4)
        LookUpTickets(head);
    else if (op == 5)
        ClearStock(head);
    else
    {
        DeleteTicketsList(head);
        AdminInitMenu();
    }

}


void AddTicketsToStock(TicketsList head)
{
    system("cls");
    char id[20];

    printf("���복��id\n>> ");
    scanf("%s", id);

    TicketsList p = FindTicketsByID(head, atoi(id));
    if (p)
    {
        printf("�ó����Ѵ���\n");
        DisplayTicketsInfo(p);
        printf("������������\n>> ");
        int add_cnt = 0;
        scanf("%d", &add_cnt);
        if (add_cnt < 0) add_cnt = 0;
        p->goods.quantity += add_cnt;

        TraverseTicketsList(head, DisplayTicketsInfo);

        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("������\n");
    } else
    {
        Tickets goods;
        goods.id = atoi(id);
        printf("���복������\n>> ");
        scanf("%s", goods.name);
        printf("���복�ν���\n>> ");
        scanf("%lf", &goods.buying_price);
        printf("���복���յ�վ\n>> ");
        scanf("%lf", &goods.selling_price);
        printf("���복��������\n>> ");
        scanf("%s", goods.manufacturer);
        printf("���복������\n>> ");
        scanf("%d", &goods.quantity);

        AddTicketsToList(head, goods);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("������\n");
        DisplayTicketsInfo(&goods);
    }

    system("pause");
    StockManagement();
}

void BatchedStock(TicketsList head)
{
    FILE *fp = fopen("ExternalData\\added_goods.txt", "r");
    if (fp)
    {
        ImportTicketsFromFile(head, fp);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        fclose(fp);
        printf("����ɹ�\n");
    } else
    {
        printf("�򿪴������ļ�ʧ��\n");
    }
    system("pause");
    StockManagement();
}

void LookOverStock(TicketsList head)
{
    printf("-------------------------------------------------------\n");
    printf("%-5s %-12s %-6s %-6s %-15s %-5s\n", "ID", "����", "ʼ��վ", "�յ�վ", "�����ͺ�", "����");
    TraverseTicketsList(head, DisplayTicketsInfo);
    printf("-------------------------------------------------------\n");
    system("pause");
    StockManagement();
}


void LookUpTickets(TicketsList head)
{
    system("cls");

    if (head->next == NULL)
    {
        printf("��¼Ϊ��\n");
        system("pause");
        StockManagement();
    }

    int op = -1;

    printf("1) ����������ѯ\n");
    printf("2) ���������Ҳ�ѯ\n");
    printf("3) �ۺϲ�ѯ\n");
    printf("0) �����ϼ�\n");

    scanf("%d", &op);

    if (op == 1)
        LookUpTicketsByName(head);
    else if (op == 2)
        LookUpTicketsByManufacturer(head);
    else if (op == 3)
        LookUpTicketsByNameAndManufacturer(head);
    else
    {
        StockManagement();
    }
}


void ShowQueriedTicketsListToAdmin(TicketsList queried_goods)
{
    printf("-------------------------------------------------------\n");
    printf("%-5s %-12s %-6s %-6s %-15s %-5s\n", "ID", "����", "ʼ��վ", "�յ�վ", "�����ͺ�", "����");
    TraverseTicketsList(queried_goods, DisplayTicketsInfo);
    printf("-------------------------------------------------------\n");
}

void LookUpTicketsByName(TicketsList head)
{
    char goods_name_prefix[MAXTICKETSNAME] = {0};
    //char manufacturer_prefix[MAXMANUFACTURERNAME] = { 0 };

    printf("���복�����ƻ�����ǰ׺\n>> ");
    scanf("%s", goods_name_prefix);
    TicketsList queried_goods = QueryTicketsByName(head, goods_name_prefix);
    ShowQueriedTicketsListToAdmin(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    LookUpTickets(head);
}

void LookUpTicketsByManufacturer(TicketsList head)
{
    char manufacturer_prefix[MAXMANUFACTURERNAME] = {0};

    printf("���복�����������ƻ�����������ǰ׺\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_goods = QueryTicketsByManufacturer(head, manufacturer_prefix);
    ShowQueriedTicketsListToAdmin(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    LookUpTickets(head);
}

void LookUpTicketsByNameAndManufacturer(TicketsList head)
{
    char goods_name_prefix[MAXTICKETSNAME] = {0};
    char manufacturer_prefix[MAXMANUFACTURERNAME] = {0};

    printf("���복�����ƻ�����ǰ׺\n>> ");
    scanf("%s", goods_name_prefix);
    printf("���복�����������ƻ�����������ǰ׺\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_goods = QueryTicketsByNameAndManufacturer(head, goods_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToAdmin(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    LookUpTickets(head);
}


void ClearStock(TicketsList head)
{
    char op[10];
    printf("�Ƿ�ɾ����¼������Ϊ0�ĳ��� (Y �� N)\n");
    scanf("%s", op);

    if (op[0] == 'Y' || op[0] == 'y')
    {
        RemoveZeroQuantityTickets(head);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("�����¼�ɹ�\n");
        system("pause");
    }

    StockManagement();
}

void UserManagement()
{
    system("cls");
    int op = -1;

    printf("1) ����û�\n");
    printf("2) ����û�\n");
    printf("3) ɾ���û�\n");
    printf("0) �����ϼ�\n");

    printf(">> ");
    scanf("%d", &op);

    if (op == 1)
        LookOverUser();
    else if (op == 2)
        AddUser();
    else if (op == 3)
        DeleteUser();
    else
        AdminInitMenu();
}


void LookOverUser()
{
    system("cls");
    DisplayAccountInfo();
    system("pause");
    UserManagement();
}


void AddUser()
{
    system("cls");
    UserAccount ua;
    printf("�˺�\n>> ");
    scanf("%s", ua.account);
    printf("����\n>> ");
    scanf("%s", ua.password);

    char level[15] = {0};
    while (strcmp(level, "0") && strcmp(level, "1"))
    {
        printf("�û�Ȩ��(0-�ۻ�Ա  1-����Ա)\n>> ");
        scanf("%s", level);
    }

    ua.permission_level = atoi(level);
    int ret = AddAccount(&ua);

    if (ret == 1)
    {
        printf("\n����û��ɹ�\n");
        printf("--------------------------------------\n");
        printf("%-10s %-10s  %-2s\n", "�˺�", "����", "Ȩ��");
        if (ua.permission_level)
            printf("%-10s %-10s %-2s\n", ua.account, ua.password, "����Ա");
        else
            printf("%-10s %-10s %-2s\n", ua.account, ua.password, "�ۻ�Ա");
        printf("--------------------------------------\n");


    } else if (ret == 0)
    {
        printf("���û��Ѵ���\n");
    } else
        printf("���ʧ��\n");

    system("pause");
    UserManagement();
}


void DeleteUser()
{
    system("cls");
    DisplayAccountInfo();
    char account[15];

    printf("\n�����ɾ���û����˺�\n>> ");
    scanf("%s", account);

    if (strcmp(account, "admin") == 0)
    {
        printf("�����û�, ����ɾ��\n");
    } else
    {
        int ret = DeleteAccount(account);
        if (ret == 1)
        {
            printf("ɾ���ɹ�\n");
        } else if (ret == -1)
        {
            printf("�޷�ɾ��\n");
        } else
        {
            printf("���û�������\n");
        }
    }

    system("pause");
    UserManagement();
    return;
}


void SoldManagement()
{
    TicketsList head = InitTicketsList();
    ImportTicketsFromFile(head, OpenTicketsFile("r"));
    SoldTickets(head);
    DeleteTicketsList(head);
    AdminInitMenu();
}


void SoldStatistics()
{
    system("cls");
    RecordsList head = InitRecordsList();
    ImportRecordsFromFile(head, OpenRecordsFile("r"));

    int op = -1;

    printf("1) �����¼\n");
    printf("2) �����¼\n");
    printf("3) ����ͳ��\n");
    printf("0) �����ϼ�\n");

    printf(">> ");
    scanf("%d", &op);

    if (op == 1)
        SingleDaySoldRecords(head);
    else if (op == 2)
        DaysSoldRecords(head);
    else if (op == 3)
        DaysSoldStatistics(head);
    else
    {
        AdminInitMenu();
        DeleteRecordsList(head);
    }

}


void SingleDaySoldRecords(RecordsList head)
{
    int year, month, day;

    printf("���������� �� ��\n>> ");

    scanf("%d %d %d", &year, &month, &day);

    if (year >= 1970 && year <= 9999 && month >= 1 && month <= 12 && day >= 1 && day <= 31)
    {
        SoldDate start = {year, month, day, 0, 0, 0};
        SoldDate end = {year, month, day, 23, 59, 59};

        QuerySoldRecordsByDate(head, start, end);
    } else
    {
        printf("��������\n");
    }
    system("pause");
    SoldStatistics();
}


void DaysSoldRecords(RecordsList head)
{
    int start_year, start_month, start_day;
    int end_year, end_month, end_day;

    printf("�����ѯ��ʼ���� �� �� ��\n>> ");
    scanf("%d %d %d", &start_year, &start_month, &start_day);

    printf("�����ѯ�������� �� �� ��\n>> ");
    scanf("%d %d %d", &end_year, &end_month, &end_day);

    if (start_year >= 1970 && end_year >= 1970 && start_year <= 9999 && end_year <= 9999 &&
        start_month >= 1 && end_month >= 1 && start_month <= 12 && end_month <= 12 &&
        start_day >= 1 && end_day >= 1 && start_day <= 31 && end_day <= 31)
    {
        SoldDate start = {start_year, start_month, start_day, 0, 0, 0};
        SoldDate end = {end_year, end_month, end_day, 23, 59, 59};

        QuerySoldRecordsByDate(head, start, end);
    } else
    {
        printf("��������\n");
    }
    system("pause");
    SoldStatistics();
}


void DaysSoldStatistics(RecordsList head)
{
    int start_year, start_month, start_day;
    int end_year, end_month, end_day;
    int min_sold_cnt = 0, min_earnings = 0;

    printf("�����ѯ��ʼ���� �� �� ��\n>> ");
    scanf("%d %d %d", &start_year, &start_month, &start_day);

    printf("�����ѯ�������� �� �� ��\n>> ");
    scanf("%d %d %d", &end_year, &end_month, &end_day);

    printf("������С����\n>> ");
    scanf("%d", &min_sold_cnt);

    printf("������С���۶�\n>> ");
    scanf("%d", &min_earnings);

    if (start_year >= 1970 && end_year >= 1970 && start_year <= 9999 && end_year <= 9999 &&
        start_month >= 1 && end_month >= 1 && start_month <= 12 && end_month <= 12 &&
        start_day >= 1 && end_day >= 1 && start_day <= 31 && end_day <= 31)
    {
        SoldDate start = {start_year, start_month, start_day, 0, 0, 0};
        SoldDate end = {end_year, end_month, end_day, 23, 59, 59};

        SoldStatisticsByDate(head, start, end, min_sold_cnt, min_earnings);
    } else
    {
        printf("��������\n");
    }
    system("pause");
    SoldStatistics();
}
