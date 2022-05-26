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
    printf("%-5s %-12s %-20s %-20s %-10s %-5s\n", "ID", "���", "ʼ��վ", "�յ�վ", "Ʊ��", "��λ����");
    printf("--------------------------------------------------------------------------------\n");
    TraverseTicketsList(head, DisplayBasicTicketsInfo);
    printf("--------------------------------------------------------------------------------\n");
}


//���򳵴Σ������³��Σ����ۼ�¼
void SoldTickets(TicketsList head)
{
    int id, cnt;
    TicketsList found_tickets;
    while (1)
    {
        system("cls");
        DisplaySoldTickets(head);
        printf("\n��������۵ĳ���ID (����-1�˳�)\n>");
        scanf("%d", &id);

        if (id < 0) break;

        // found_ticketsΪ����������Ʊ���ɵ�����
        found_tickets = FindTicketsByID(head, id);
        if (found_tickets)
        {
            DisplayBasicTicketsInfo(&found_tickets->tickets);
            printf("������������\n>");
            scanf("%d", &cnt);
            if (cnt <= 0)
            {
                printf("��Ʊ��������\n");
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
                        printf("�ó��γ�Ʊ��治��\n");
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
                    printf("���۳ɹ�\n");
                    system("pause");
                    continue;
                } else
                {
                    if (tickets_fp)
                        fclose(tickets_fp);
                    if (records_fp)
                        fclose(records_fp);
                    printf("����ϵͳ����ʧ��\n");
                    system("pause");
                    break;
                }

            }
        } else
        {
            printf("�޴˳���\n");
            system("pause");
            continue;
        }
    }

}


void SalesmanInitMenu()
{
    system("cls");

    int op = -1;

    printf("1) ����Ʊ\n");
    printf("2) ��Ϣ��ѯ\n");
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

void ShowQueriedTicketsListToSalesman(TicketsList queried_tickets)
{
    printf("---------------------------------------------------------------------------------\n");
    printf("%-5s %-12s %-20s %-20s %-5s\n", "ID", "����", "ʼ��վ", "�յ�վ", "��Ʊ����");
    TraverseTicketsList(queried_tickets, DisplayBasicTicketsInfo);
    printf("---------------------------------------------------------------------------------\n");
}

void SalesmanLookUpTicketsByName(TicketsList head)
{
    char tickets_name_prefix[MAX_LABEL_LENGTH] = {0};
    //char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = { 0 };

    printf("���복�����ƻ�����ǰ׺\n>> ");
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

    printf("���복�γ��ͻ���ǰ׺\n>> ");
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

    printf("���복�����ƻ�����ǰ׺\n>> ");
    scanf("%s", tickets_name_prefix);
    printf("���복�γ��ͻ���ǰ׺\n>> ");
    scanf("%s", manufacturer_prefix);
    TicketsList queried_tickets = QueryTicketsByNameAndManufacturer(head, tickets_name_prefix, manufacturer_prefix);
    ShowQueriedTicketsListToSalesman(queried_tickets);
    DeleteTicketsList(queried_tickets);
    system("pause");
    SalesmanLookUpTickets(head);
}

