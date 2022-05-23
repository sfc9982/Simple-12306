#include "stdafx.h"

const char TICKETS_LIST_PATH[30] = "Data\\goods_list.txt";

//��ʼ���г����� ������ͷ��������
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
void TraverseTicketsList(TicketsList head, void(*Fun)(Tickets *))
{
    TicketsList p = head->next;
    while (p)
    {
        Fun(&p->goods);
        p = p->next;
    }
}


//���һ���г�����Ϣ
void DisplayTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-20s %-20s %-15s %-5d\n", goods->id, goods->name, goods->buying_price, goods->selling_price,
           goods->manufacturer, goods->quantity);
}

//��ʾһ���г�������Ϣ
void DisplayBasicTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-20s %-20s %-5d\n", goods->id, goods->name, goods->buying_price, goods->selling_price,
           goods->quantity);
}


//���г����������һ���г���Ϣ��ԭ�д��г������ϲ�������1
//ԭû�У�������ĩβ��ӣ�����0
int AddTicketsToList(TicketsList head, Tickets goods)
{
    int id = goods.id;
    TicketsList p = head->next;
    while (p)
    {
        if (p->goods.id == id)
        {
            p->goods.quantity += goods.quantity;
            return 1;
        }
        p = p->next;
    }

    TicketsList newNode = (TicketsList) malloc(sizeof(struct TicketsListNode));
    newNode->goods = goods;
    newNode->next = head->next;
    head->next = newNode;

    return 0;
}


//���г������в����г����Ϊid���г������г���λ��������quantity
//�ɹ�����1���г�������������0���޴�id�г�����-1
int ReduceTicketsQuantity(TicketsList head, int id, int quantity)
{
    TicketsList p = head->next;
    while (p)
    {
        if (p->goods.id == id)
        {
            if (p->goods.quantity < quantity)
                return 0;
            p->goods.quantity -= quantity;
            return 1;
        }
        p = p->next;
    }
    return -1;
}


//���г������в����г����Ϊid���г������г���������quantity
//�ɹ�����1���޴�id�г�����-1
int IncreaseTicketsQuantity(TicketsList head, int id, int quantity)
{
    TicketsList p = head->next;
    while (p)
    {
        if (p->goods.id == id)
        {
            p->goods.quantity += quantity;
            return 1;
        }
        p = p->next;
    }
    return -1;
}


//����г�������Ϊ����Ϊ0���г�
void RemoveZeroQuantityTickets(TicketsList head)
{
    TicketsList pre = head;
    TicketsList p = head->next;
    while (p)
    {
        if (p->goods.quantity == 0)
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


//���ļ��е����г�����
void ImportTicketsFromFile(TicketsList head, FILE *fp)
{
    Tickets goods;
    while (!feof(fp))
    {
        fscanf(fp, "%d %s %s %s %s %d\n", &goods.id, goods.name, goods.buying_price,
               goods.selling_price, goods.manufacturer, &goods.quantity);
        AddTicketsToList(head, goods);
    }
    fclose(fp);
}

FILE *OpenTicketsFile(char *mod)
{
    return fopen(TICKETS_LIST_PATH, mod);
}

//��ϵͳ���г����ݵ������ļ�
void ExportTicketsToFile(TicketsList head, FILE *fp)
{
    Tickets goods;
    TicketsList p = head->next;
    while (p)
    {
        goods = p->goods;
        fprintf(fp, "%d %s %s %s %s %d\n", goods.id, goods.name, goods.buying_price,
                goods.selling_price, goods.manufacturer, goods.quantity);
        p = p->next;
    }
    fclose(fp);
}


//��ID���г������в����г������ҵ�������������λ�ã����򷵻�NULL
TicketsList FindTicketsByID(TicketsList head, int id)
{
    if (id < 0) return NULL;

    TicketsList p = head->next;
    while (p)
    {
        if (p->goods.id == id)
            return p;
        p = p->next;
    }

    return NULL;
}


