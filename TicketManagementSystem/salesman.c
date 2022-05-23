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
    printf("%-5s %-12s %-20s %-20s %-5s\n", "ID", "班次", "始发站", "终点站", "座位余量");
    TraverseTicketsList(head, DisplayBasicTicketsInfo);
    printf("--------------------------------------------------------------------------------\n");
}


//购买商品，并更新商品，销售记录
void SoldTickets(TicketsList head)
{
    int id, cnt;
    TicketsList found_goods;
    while (1)
    {
        system("cls");
        DisplaySoldTickets(head);
        printf("\n输入待销售的商品ID (输入-1退出)\n>");
        scanf("%d", &id);

        if (id < 0) break;

        found_goods = FindTicketsByID(head, id);
        if (found_goods)
        {
            DisplayBasicTicketsInfo(&found_goods->goods);
            printf("输入销售数量\n>");
            scanf("%d", &cnt);
            if (cnt <= 0)
            {
                printf("车票数量有误\n");
                system("pause");
                continue;
            } else
            {
                FILE *goods_fp = OpenTicketsFile("w");
                FILE *records_fp = OpenRecordsFile("a");
                if (goods_fp && records_fp)
                {
                    if (!ReduceTicketsQuantity(head, id, cnt))
                    {
                        printf("该车次车票库存不足\n");
                        system("pause");;
                        continue;
                    }
                    ExportTicketsToFile(head, goods_fp);

                    SoldTicketsRecord record;
                    record.id = found_goods->goods.id;
                    strcpy(record.name, found_goods->goods.name);
                    strcpy(record.station_dest, found_goods->goods.station_dest);
                    record.sold_quantity = cnt;
                    strcpy(record.station_from, found_goods->goods.station_from);
                    record.date = GetNowDate();

                    AppendARecordToFile(record, records_fp);
                    printf("销售成功\n");
                    system("pause");
                    continue;
                } else
                {
                    if (goods_fp) fclose(goods_fp);
                    if (records_fp) fclose(records_fp);
                    printf("连接系统数据失败\n");
                    system("pause");
                    break;
                }

            }
        } else
        {
            printf("无此商品\n");
            system("pause");
            continue;
        }
    }

}


void SalesmanInitMenu()
{
    system("cls");

    int op = -1;

    printf("1) 销售车票\n");
    printf("2) 查询车票\n");
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

void ShowQueriedTicketsListToSalesman(TicketsList queried_goods)
{
    printf("----------------------------------------------------\n");
    printf("%-5s %-12s %-6s %-15s %-5s\n", "ID", "名称", "售价", "生产厂商", "余量");
    TraverseTicketsList(queried_goods, DisplayBasicTicketsInfo);
    printf("----------------------------------------------------\n");
}

void SalesmanLookUpTicketsByName(TicketsList head)
{
    char goods_name_prefix[MAX_LABEL_LENGTH] = {0};
    //char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = { 0 };

    printf("输入商品名称或名称前缀\n>> ");
    scanf("%s", goods_name_prefix);
    TicketsList queried_goods = QueryTicketsByName(head, goods_name_prefix);
    ShowQueriedTicketsListToSalesman(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    SalesmanLookUpTickets(head);
}

void SalesmanLookUpTicketsByManufacturer(TicketsList head)
{
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入商品生产商名称或生产商名称前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_goods = QueryTicketsByManufacturer(head, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    SalesmanLookUpTickets(head);
}

void SalesmanLookUpTicketsByNameAndManufacturer(TicketsList head)
{
    char goods_name_prefix[MAX_LABEL_LENGTH] = {0};
    char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};

    printf("输入商品名称或名称前缀\n>> ");
    scanf("%s", goods_name_prefix);
    printf("输入商品生产商名称或生产商名称前缀\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_goods = QueryTicketsByNameAndManufacturer(head, goods_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    SalesmanLookUpTickets(head);
}

