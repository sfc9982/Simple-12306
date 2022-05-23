#include "stdafx.h"

const char TICKETS_LIST_PATH[30] = "Data\\goods_list.txt";

//��ʼ����Ʒ���� ������ͷ��������
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

//������Ʒ��������ĳ�ֲ���
void TraverseTicketsList(TicketsList head, void(*Fun)(Tickets *))
{
    TicketsList p = head->next;
    while (p)
    {
        Fun(&p->goods);
        p = p->next;
    }
}


//���һ����Ʒ����Ϣ
void DisplayTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-15s %-15s %-15s %-5d\n", goods->id, goods->name, goods->buying_price, goods->selling_price,
           goods->manufacturer, goods->quantity);
}

//��ʾһ����Ʒ������Ϣ
void DisplayBasicTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-15s %-15s %-5d\n", goods->id, goods->name, goods->selling_price, goods->manufacturer,
           goods->quantity);
}


//����Ʒ���������һ����Ʒ��Ϣ��ԭ�д���Ʒ�����ϲ�������1
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


//����Ʒ�����в�����Ʒ���Ϊid����Ʒ������Ʒ��������quantity
//�ɹ�����1����Ʒ������������0���޴�id��Ʒ����-1
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


//����Ʒ�����в�����Ʒ���Ϊid����Ʒ������Ʒ��������quantity
//�ɹ�����1���޴�id��Ʒ����-1
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


//�����Ʒ������Ϊ����Ϊ0����Ʒ
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


//���ļ��е�����Ʒ����
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

//��ϵͳ����Ʒ���ݵ������ļ�
void ExportTicketsToFile(TicketsList head, FILE *fp)
{
    Tickets goods;
    TicketsList p = head->next;
    while (p)
    {
        goods = p->goods;
        fprintf(fp, "%d %s %.2lf %.2lf %s %d\n", goods.id, goods.name, goods.buying_price,
                goods.selling_price, goods.manufacturer, goods.quantity);
        p = p->next;
    }
    fclose(fp);
}


//��ID����Ʒ�����в�����Ʒ�����ҵ�������������λ�ã����򷵻�NULL
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


