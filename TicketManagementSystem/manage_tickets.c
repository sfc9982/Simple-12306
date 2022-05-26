#include "stdafx.h"

const char TICKETS_LIST_PATH[30] = "Data\\tickets_list.txt";

// ��ʼ���г����� ������ͷ��������
TicketsList InitTicketsList()
{
    TicketsList head = (TicketsList) malloc(sizeof(struct TicketsListNode));
    head->next = NULL;

    return head;
}

void DeleteTicketsList(TicketsList head)
{
    TicketsList next;

    while (head)
    {
        next = head->next;
        free(head);
        head = next;
    }
}

//�����г���������ĳ�ֲ���
void TraverseTicketsList(TicketsList head, void(*Func)(Tickets *))
{
    TicketsList p = head->next;
    while (p)
    {
        Func(&p->tickets);
        p = p->next;
    }
}


// ���һ���г�����Ϣ
void DisplayTicketsInfo(Tickets *tickets)
{
    printf("%-5d %-12s %-20s %-20s %-12s %-10.2lf %-5d\n", tickets->id, tickets->name, tickets->station_from,
           tickets->station_dest,
           tickets->manufacturer, tickets->price, tickets->quantity);
}

// ��ʾһ���г�������Ϣ
void DisplayBasicTicketsInfo(Tickets *tickets)
{
    printf("%-5d %-12s %-20s %-20s %-10.2lf %-5d\n", tickets->id, tickets->name, tickets->station_from,
           tickets->station_dest,
           tickets->price, tickets->quantity);
}


// ���г����������һ���г���Ϣ��ԭ�д��г������ϲ�������1
// ԭû�У�������ĩβ��ӣ�����0
int AddTicketsToList(TicketsList head, Tickets tickets)
{
    int id = tickets.id;
    TicketsList p = head->next;
    while (p)
    {
        if (p->tickets.id == id)
        {
            p->tickets.quantity += tickets.quantity;
            return 1;
        }
        p = p->next;
    }

    TicketsList newNode = (TicketsList) malloc(sizeof(struct TicketsListNode));
    newNode->tickets = tickets;
    newNode->next = head->next;
    head->next = newNode;

    return 0;
}


// ���г������в����г����Ϊid���г������г���λ��������quantity
// �ɹ�����1���г�������������0���޴�id�г�����-1
int ReduceTicketsQuantity(TicketsList head, int id, int quantity)
{
    TicketsList p = head->next;
    while (p)
    {
        if (p->tickets.id == id)
        {
            if (p->tickets.quantity < quantity)
                return 0;
            p->tickets.quantity -= quantity;
            return 1;
        }
        p = p->next;
    }
    return -1;
}


// ���г������в����г����Ϊid���г������г���������quantity
// �ɹ�����1���޴�id�г�����-1
int IncreaseTicketsQuantity(TicketsList head, int id, int quantity)
{
    TicketsList p = head->next;
    while (p)
    {
        if (p->tickets.id == id)
        {
            p->tickets.quantity += quantity;
            return 1;
        }
        p = p->next;
    }
    return -1;
}


// ����г�������Ϊ����Ϊ0���г�
void RemoveZeroQuantityTickets(TicketsList head)
{
    TicketsList pre = head;
    TicketsList p = head->next;
    while (p)
    {
        if (p->tickets.quantity == 0)
        {
            TicketsList tmp = p;
            pre->next = tmp->next;
            free(tmp);
            p = pre->next;
        } else
        {
            p = p->next;
            pre = pre->next;
        }
    }
}


// ���ļ��е����г�����
void ImportTicketsFromFile(TicketsList head, FILE *fp)
{
    Tickets tickets;
    while (!feof(fp))
    {
        fscanf(fp, "%d %s %s %s %s %lf %d\n", &tickets.id, tickets.name, tickets.station_from,
               tickets.station_dest, tickets.manufacturer, &tickets.price, &tickets.quantity);
        AddTicketsToList(head, tickets);
    }
    fclose(fp);
}

FILE *OpenTicketsFile(char *mod)
{
    return fopen(TICKETS_LIST_PATH, mod);
}

// ��ϵͳ���г����ݵ������ļ�
void ExportTicketsToFile(TicketsList head, FILE *fp)
{
    Tickets train;
    TicketsList p = head->next;
    while (p)
    {
        train = p->tickets;
        fprintf(fp, "%d %s %s %s %s %.2lf %d\n", train.id, train.name, train.station_from,
                train.station_dest, train.manufacturer, train.price, train.quantity);
        p = p->next;
    }
    fclose(fp);
}


// ��ID���г������в����г������ҵ�������������λ�ã����򷵻�NULL
TicketsList FindTicketsByID(TicketsList head, int id)
{
    if (id < 0)
        return NULL;

    TicketsList p = head->next;
    while (p)
    {
        if (p->tickets.id == id)
            return p;
        p = p->next;
    }

    return NULL;
}


