#include "stdafx.h"

//判断是否为销售员账户，无法打开文件返回-1，是返回1，否返回0
int IsSalesmanAccount(char *account, char *password)
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
            && user[i].permission_level == 0)
            return 1;
    }

    free(user);
    fclose(fp);

    return 0;
}

void DisplaySoldTickets(TicketsList head)
{
    printf("--------------------------------------------------------------------------------\n");
    printf("%-5s %-14s %-23s %-18s %-11s %-5s\n", "ID", "班次", "始发站", "终点站", "票价", "座位余量");
    printf("--------------------------------------------------------------------------------\n");
    TraverseTicketsList(head, DisplayBasicTicketsInfo);
    printf("--------------------------------------------------------------------------------\n");
}


//购买车次，并更新车次，销售记录
void SoldTickets(TicketsList head)
{
    int id, cnt;
    TicketsList found_tickets;
    while (1)
    {
        system("cls");
        DisplaySoldTickets(head);
        printf("\n输入待销售的车次ID (输入-1退出)\n>");
        scanf("%d", &id);

        if (id < 0) break;

        // found_tickets为符合条件车票构成的链表
        found_tickets = FindTicketsByID(head, id);
        if (found_tickets)
        {
            DisplayBasicTicketsInfo(&found_tickets->tickets);
            printf("输入销售数量\n>");
            scanf("%d", &cnt);
            if (cnt <= 0)
            {
                printf("车票数量有误\n");
                system("pause");
                continue;
            } else
            {
                FILE *tickets_fp = OpenTicketsFile("w");
                FILE *records_fp = OpenRecordsFile("a");
                if (tickets_fp && records_fp)
                {
                    if (!ReduceTicketsQuantity(head, id, cnt))
                    {
                        printf("该车次车票库存不足\n");
                        system("pause");;
                        continue;
                    }
                    ExportTicketsToFile(head, tickets_fp);

                    SoldTicketsRecord record;

                    record.id = found_tickets->tickets.id;
                    strcpy(record.name, found_tickets->tickets.name);
                    strcpy(record.station_from, found_tickets->tickets.station_from);
                    strcpy(record.station_dest, found_tickets->tickets.station_dest);
                    record.price = found_tickets->tickets.price;
                    record.sold_quantity = cnt;
                    record.date = GetCurTime();

                    AppendARecordToFile(record, records_fp);
                    printf("销售成功\n");
                    system("pause");
                    continue;
                } else
                {
                    if (tickets_fp)
                        fclose(tickets_fp);
                    if (records_fp)
                        fclose(records_fp);
                    printf("连接系统数据失败\n");
                    system("pause");
                    break;
                }

            }
        } else
        {
            printf("无此车次\n");
            system("pause");
            continue;
        }
    }

}


void SalesmanInitMenu()
{
    system("cls");

    int op = -1;

    printf("1) 购买车票\n");
    printf("2) 信息查询\n");
    printf("0) 退出登陆\n");

    printf(">> ");
    scanf("%d", &op);

    TicketsList head = InitTicketsList();
    ImportTicketsFromFile(head, OpenTicketsFile("r"));
    if (op == 1)
    {
        SoldTickets(head);
        DeleteTicketsList(head);
        SalesmanInitMenu();
    } else if (op == 2)
        SalesmanLookUpTickets(head);
    else
    {
        DeleteTicketsList(head);
        LogIn();
    }

}


void SalesmanLookUpTickets(TicketsList head)
{
    system("cls");

    if (head->next == NULL)
    {
        printf("库存为空\n");
        system("pause");
        SalesmanInitMenu();
    }

    int op = -1;

    printf("1) 按车次名查询\n");
    printf("2) 按车型查询\n");
    printf("3) 综合查询\n");
    printf("0) 返回上级\n");

    scanf("%d", &op);

    if (op == 1)
        SalesmanLookUpTicketsByName(head);
    else if (op == 2)
        SalesmanLookUpTicketsByManufacturer(head);
    else if (op == 3)
        SalesmanLookUpTicketsByNameAndManufacturer(head);
    else
    {
        SalesmanInitMenu();
    }
}

void ShowQueriedTicketsListToSalesman(TicketsList queried_tickets)
{
    printf("---------------------------------------------------------------------------------\n");
    printf("%-5s %-14s %-23s %-18s %-11s %s\n", "ID", "车次", "始发站", "终点站", "票价", "车票余量");
    printf("---------------------------------------------------------------------------------\n");
    TraverseTicketsList(queried_tickets, DisplayBasicTicketsInfo);
    printf("---------------------------------------------------------------------------------\n");
}

void SalesmanLookUpTicketsByName(TicketsList head)
{
    char tickets_name_prefix[MAX_LABEL_LENGTH] = {0};
    //char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = { 0 };

    printf("输入车次名称或名称前缀\n>> ");
    scanf("%s", tickets_name_prefix);
    TicketsList queried_tickets = QueryTicketsByName(head, tickets_name_prefix);
    ShowQueriedTicketsListToSalesman(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    SalesmanLookUpTickets(head);
}

void SalesmanLookUpTicketsByManufacturer(TicketsList head)
{
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入车次车型或车型前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_tickets = QueryTicketsByManufacturer(head, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    SalesmanLookUpTickets(head);
}

void SalesmanLookUpTicketsByNameAndManufacturer(TicketsList head)
{
    char tickets_name_prefix[MAX_LABEL_LENGTH] = {0};
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入车次名称或名称前缀\n>> ");
    scanf("%s", tickets_name_prefix);
    printf("输入车次车型或车型前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_tickets = QueryTicketsByNameAndManufacturer(head, tickets_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    SalesmanLookUpTickets(head);
}

