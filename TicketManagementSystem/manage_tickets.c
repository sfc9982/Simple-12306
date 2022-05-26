#include "stdafx.h"

const char TICKETS_LIST_PATH[30] = "Data\\tickets_list.txt";

// 初始化列车链表 创建带头结点的链表
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

//遍历列车链表，进行某种操作
void TraverseTicketsList(TicketsList head, void(*Func)(Tickets *))
{
    TicketsList p = head->next;
    while (p)
    {
        Func(&p->tickets);
        p = p->next;
    }
}


// 输出一个列车的信息
void DisplayTicketsInfo(Tickets *tickets)
{
    printf("%-5d %-12s %-20s %-20s %-12s %-10.2lf %-5d\n", tickets->id, tickets->name, tickets->station_from,
           tickets->station_dest,
           tickets->manufacturer, tickets->price, tickets->quantity);
}

// 显示一个列车基本信息
void DisplayBasicTicketsInfo(Tickets *tickets)
{
    printf("%-5d %-12s %-20s %-20s %-10.2lf %-5d\n", tickets->id, tickets->name, tickets->station_from,
           tickets->station_dest,
           tickets->price, tickets->quantity);
}


// 在列车链表中添加一条列车信息，原有此列车数量合并，返回1
// 原没有，在链表末尾添加，返回0
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


// 在列车链表中查找列车序号为id的列车，该列车座位数量减少quantity
// 成功返回1，列车数量不够返回0，无此id列车返回-1
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


// 在列车链表中查找列车序号为id的列车，该列车数量增加quantity
// 成功返回1，无此id列车返回-1
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


// 清除列车链表中为数量为0的列车
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


// 从文件中导入列车数据
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

// 将系统内列车数据导出到文件
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


// 有ID在列车链表中查找列车，查找到返回链表所在位置，否则返回NULL
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


