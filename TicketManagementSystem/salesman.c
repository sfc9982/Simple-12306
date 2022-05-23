#include "stdafx.h"

//�ж��Ƿ�Ϊ����Ա�˻����޷����ļ�����-1���Ƿ���1���񷵻�0
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
    printf("%-5s %-12s %-20s %-20s %-5s\n", "ID", "���", "ʼ��վ", "�յ�վ", "��λ����");
    TraverseTicketsList(head, DisplayBasicTicketsInfo);
    printf("--------------------------------------------------------------------------------\n");
}


//������Ʒ����������Ʒ�����ۼ�¼
void SoldTickets(TicketsList head)
{
    int id, cnt;
    TicketsList found_goods;
    while (1)
    {
        system("cls");
        DisplaySoldTickets(head);
        printf("\n��������۵���ƷID (����-1�˳�)\n>");
        scanf("%d", &id);

        if (id < 0) break;

        found_goods = FindTicketsByID(head, id);
        if (found_goods)
        {
            DisplayBasicTicketsInfo(&found_goods->goods);
            printf("������������\n>");
            scanf("%d", &cnt);
            if (cnt <= 0)
            {
                printf("��Ʊ��������\n");
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
                        printf("�ó��γ�Ʊ��治��\n");
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
                    printf("���۳ɹ�\n");
                    system("pause");
                    continue;
                } else
                {
                    if (goods_fp) fclose(goods_fp);
                    if (records_fp) fclose(records_fp);
                    printf("����ϵͳ����ʧ��\n");
                    system("pause");
                    break;
                }

            }
        } else
        {
            printf("�޴���Ʒ\n");
            system("pause");
            continue;
        }
    }

}


void SalesmanInitMenu()
{
    system("cls");

    int op = -1;

    printf("1) ���۳�Ʊ\n");
    printf("2) ��ѯ��Ʊ\n");
    printf("0) �˳���½\n");

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
        printf("���Ϊ��\n");
        system("pause");
        SalesmanInitMenu();
    }

    int op = -1;

    printf("1) ����������ѯ\n");
    printf("2) �����Ͳ�ѯ\n");
    printf("3) �ۺϲ�ѯ\n");
    printf("0) �����ϼ�\n");

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
    printf("%-5s %-12s %-6s %-15s %-5s\n", "ID", "����", "�ۼ�", "��������", "����");
    TraverseTicketsList(queried_goods, DisplayBasicTicketsInfo);
    printf("----------------------------------------------------\n");
}

void SalesmanLookUpTicketsByName(TicketsList head)
{
    char goods_name_prefix[MAX_LABEL_LENGTH] = {0};
    //char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = { 0 };

    printf("������Ʒ���ƻ�����ǰ׺\n>> ");
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

    printf("������Ʒ���������ƻ�����������ǰ׺\n>> ");
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

    printf("������Ʒ���ƻ�����ǰ׺\n>> ");
    scanf("%s", goods_name_prefix);
    printf("������Ʒ���������ƻ�����������ǰ׺\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_goods = QueryTicketsByNameAndManufacturer(head, goods_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_goods);
    DeleteTicketsList(queried_goods);
    system("pause");
    SalesmanLookUpTickets(head);
}

