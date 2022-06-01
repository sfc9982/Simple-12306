#include "stdafx.h"


//判断是否为管理员账户，无法打开文件返回-1，是返回1，否返回0
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
            puts("登录成功！");
            return 1;
        }
    }

    free(user);
    fclose(fp);

    return 0;
}

//增加账户信息，无法打开返回-1，成功返回1，账户已存在返回0（更改密码）
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


//更该所有用户的密码，文件无法打开返回-1，删除成功返回1，失败返回0
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
            if (i == index)
                continue;
            //printf("look: %s %s %d\n", user[i].account, user[i].password, user[i].permission_level);
            fwrite(user + i, sizeof(UserAccount), 1, fp);
        }

        fclose(fp);
    }

    free(user);

    return index != -1 ? 1 : 0;
}


//显示系统内所有账户信息，无法打开返回-1
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
    printf("%-12s %-12s %-2s\n", "账号", "密码", "权限");
    printf("--------------------------------------\n");
    for (int i = 0; i < size; i++)
    {
        fread(own + i, sizeof(UserAccount), 1, fp);
        if (own[i].permission_level)
            printf("%-10s %-10s %-2s\n", own[i].account, own[i].password, "管理员");
        else
            printf("%-10s %-10s %-2s\n", own[i].account, own[i].password, "用户");
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

    printf("1) 记录管理\n");
    printf("2) 用户管理\n");
    printf("3) 销售车次\n");
    printf("4) 销售统计\n");
    printf("0) 退出登陆\n");

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

    printf("1) 添加车次\n");
    printf("2) 批量添加\n");
    printf("3) 查看车次\n");
    printf("4) 查询站点\n");
    printf("5) 清理车次\n");
    printf("0) 返回上级\n");

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

    printf("输入车次id\n>> ");
    scanf("%s", id);

    TicketsList p = FindTicketsByID(head, atoi(id));
    if (p)
    {
        printf("该车次已存在\n");
        DisplayTicketsInfo((Tickets *) p);
        printf("输入增添数量\n>> ");
        int add_cnt = 0;
        scanf("%d", &add_cnt);
        if (add_cnt < 0) add_cnt = 0;
        p->tickets.quantity += add_cnt;

        TraverseTicketsList(head, DisplayTicketsInfo);

        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("添加完成\n");
    } else
    {
        Tickets train_add;
        train_add.id = atoi(id);
        printf("输入车次名称\n>> ");
        scanf("%s", train_add.name);
        printf("输入车次始发站\n>> ");
        scanf("%s", train_add.station_from);
        printf("输入车次终点站\n>> ");
        scanf("%s", train_add.station_dest);
        printf("输入车型\n>> ");
        scanf("%s", train_add.manufacturer);
        printf("输入车次座位数量\n>> ");
        scanf("%d", &train_add.quantity);

        AddTicketsToList(head, train_add);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("添加完成\n");
        DisplayTicketsInfo(&train_add);
    }

    system("pause");
    StockManagement();
}

void BatchedStock(TicketsList head)
{
    FILE *fp = fopen("ExternalData\\added_tickets.txt", "r");
    if (fp)
    {
        ImportTicketsFromFile(head, fp);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        fclose(fp);
        printf("导入成功\n");
    } else
    {
        printf("打开待导入文件失败\n");
    }
    system("pause");
    StockManagement();
}

void LookOverStock(TicketsList head)
{
    printf("----------------------------------------------------------------------------------------------\n");
    printf("%-5s %-14s %-23s %-18s %-19s %-11s %-5s\n", "ID", "名称", "始发站", "终点站", "动车型号", "票价", "余量");
    printf("----------------------------------------------------------------------------------------------\n");
    TraverseTicketsList(head, DisplayTicketsInfo);
    printf("----------------------------------------------------------------------------------------------\n");
    system("pause");
    StockManagement();
}


void LookUpTickets(TicketsList head)
{
    system("cls");

    if (head->next == NULL)
    {
        printf("记录为空\n");
        system("pause");
        StockManagement();
    }

    int op = -1;

    printf("1) 按车次名查询\n");
    printf("2) 按动车车型查询\n");
    printf("3) 综合查询\n");
    printf("0) 返回上级\n");

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


void ShowQueriedTicketsListToAdmin(TicketsList queried_tickets)
{
    printf("--------------------------------------------------------------------------------\n");
    printf("%-5s %-12s %-20s %-20s %-15s %-5s\n", "ID", "名称", "始发站", "终点站", "动车型号", "座位余量");
    printf("--------------------------------------------------------------------------------\n");
    TraverseTicketsList(queried_tickets, DisplayTicketsInfo);
    printf("--------------------------------------------------------------------------------\n");
}

void LookUpTicketsByName(TicketsList head)
{
    char tickets_name_prefix[MAX_LABEL_LENGTH] = {0};
    //char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = { 0 };

    printf("输入车次名称或名称前缀\n>> ");
    scanf("%s", tickets_name_prefix);
    TicketsList queried_tickets = QueryTicketsByName(head, tickets_name_prefix);
    ShowQueriedTicketsListToAdmin(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    LookUpTickets(head);
}

void LookUpTicketsByManufacturer(TicketsList head)
{
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入车次生产商名称或生产商名称前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_tickets = QueryTicketsByManufacturer(head, manufacturer_prefix);
    ShowQueriedTicketsListToAdmin(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    LookUpTickets(head);
}

void LookUpTicketsByNameAndManufacturer(TicketsList head)
{
    char tickets_name_prefix[MAX_LABEL_LENGTH] = {0};
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入车次名称或名称前缀\n>> ");
    scanf("%s", tickets_name_prefix);
    printf("输入车次生产商名称或车型前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_tickets = QueryTicketsByNameAndManufacturer(head, tickets_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToAdmin(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    LookUpTickets(head);
}


void ClearStock(TicketsList head)
{
    char op[10];
    printf("是否删除记录内数量为0的车次 (Y 或 N)\n");
    scanf("%s", op);

    if (op[0] == 'Y' || op[0] == 'y')
    {
        RemoveZeroQuantityTickets(head);
        ExportTicketsToFile(head, OpenTicketsFile("w"));
        printf("清理记录成功\n");
        system("pause");
    }

    StockManagement();
}

void UserManagement()
{
    system("cls");
    int op = -1;

    printf("1) 浏览用户\n");
    printf("2) 添加用户\n");
    printf("3) 删除用户\n");
    printf("0) 返回上级\n");

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

int createPassword(char *passwd, int size)
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

void AddUser()
{
    system("cls");
    UserAccount ua;
    char *password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));
    char *confirm_password = (char *) calloc(MAX_PASSWORD_LENGTH, sizeof(char));
    printf("账号\n>> ");
    scanf("%s", ua.account);

    retry_passwd:

    printf("密码\n>> ");
    createPassword(password, MAX_PASSWORD_LENGTH);
    printf("确认密码\n>> ");
    createPassword(confirm_password, MAX_PASSWORD_LENGTH);

    if (strcmp(ua.password, confirm_password) != 0)
    {
        printf("两次输入的密码不一致，请重新输入\n");
        system("pause");
        system("cls");
        goto retry_passwd;
    } else
    {
        strcpy(ua.password, password);
        free(password);
        free(confirm_password);
    }
    char level[15] = {0};
    while (strcmp(level, "0") && strcmp(level, "1"))
    {
        printf("用户权限(0-用户  1-管理员)\n>> ");
        scanf("%s", level);
    }

    ua.permission_level = atoi(level);
    int ret = AddAccount(&ua);

    if (ret == 1)
    {
        printf("\n添加用户成功\n");
        printf("--------------------------------------\n");
        printf("%-12s %-12s  %-2s\n", "账号", "密码", "权限");
        printf("--------------------------------------\n");
        if (ua.permission_level)
            printf("%-10s %-10s %-2s\n", ua.account, ua.password, "管理员");
        else
            printf("%-10s %-10s %-2s\n", ua.account, ua.password, "用户");
        printf("--------------------------------------\n");


    } else if (ret == 0)
    {
        printf("该用户已存在\n");
    } else
    {
        printf("添加失败\n");
    }

    system("pause");
    UserManagement();
}


void DeleteUser()
{
    system("cls");
    DisplayAccountInfo();
    char account[15];

    printf("\n输入待删除用户的账号\n>> ");
    scanf("%s", account);

    if (strcmp(account, "admin") == 0)
    {
        printf("超级用户, 不能删除\n");
    } else
    {
        int ret = DeleteAccount(account);
        if (ret == 1)
        {
            printf("删除成功\n");
        } else if (ret == -1)
        {
            printf("无法删除\n");
        } else
        {
            printf("该用户不存在\n");
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

    printf("1) 单天记录\n");
    printf("2) 多天记录\n");
    printf("3) 多天统计\n");
    printf("0) 返回上级\n");

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

    printf("依次输入年 月 日\n>> ");

    scanf("%d %d %d", &year, &month, &day);

    if (year >= 1970 && year <= 9999 && month >= 1 && month <= 12 && day >= 1 && day <= 31)
    {
        SoldDate start = {year, month, day, 0, 0, 0};
        SoldDate end = {year, month, day, 23, 59, 59};

        QuerySoldRecordsByDate(head, start, end);
    } else
    {
        printf("输入有误\n");
    }
    system("pause");
    SoldStatistics();
}


void DaysSoldRecords(RecordsList head)
{
    int start_year, start_month, start_day;
    int end_year, end_month, end_day;

    printf("输入查询开始日期 年 月 日\n>> ");
    scanf("%d %d %d", &start_year, &start_month, &start_day);

    printf("输入查询结束日期 年 月 日\n>> ");
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
        printf("输入有误\n");
    }
    system("pause");
    SoldStatistics();
}


void DaysSoldStatistics(RecordsList head)
{
    int start_year, start_month, start_day;
    int end_year, end_month, end_day;
    int min_sold_cnt = 0, min_earnings = 0;

    printf("输入查询开始日期 年 月 日\n>> ");
    scanf("%d %d %d", &start_year, &start_month, &start_day);

    printf("输入查询结束日期 年 月 日\n>> ");
    scanf("%d %d %d", &end_year, &end_month, &end_day);

    printf("输入最小销量\n>> ");
    scanf("%d", &min_sold_cnt);

    printf("输入最小销售额\n>> ");
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
        printf("输入有误\n");
    }
    system("pause");
    SoldStatistics();
}
