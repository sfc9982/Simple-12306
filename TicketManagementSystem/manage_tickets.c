#include "stdafx.h"

const char TICKETS_LIST_PATH[30] = "Data\\goods_list.txt";

//初始化商品链表 创建带头结点的链表
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

//遍历商品链表，进行某种操作
void TraverseTicketsList(TicketsList head, void(*Fun)(Tickets *))
{
    TicketsList p = head->next;
    while (p)
    {
        Fun(&p->goods);
        p = p->next;
    }
}


//输出一个商品的信息
void DisplayTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-15s %-15s %-15s %-5d\n", goods->id, goods->name, goods->buying_price, goods->selling_price,
           goods->manufacturer, goods->quantity);
}

//显示一个商品基本信息
void DisplayBasicTicketsInfo(Tickets *goods)
{
    printf("%-5d %-12s %-15s %-15s %-5d\n", goods->id, goods->name, goods->selling_price, goods->manufacturer,
           goods->quantity);
}


//在商品链表中添加一条商品信息，原有此商品数量合并，返回1
//原没有，在链表末尾添加，返回0
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


//在商品链表中查找商品序号为id的商品，该商品数量减少quantity
//成功返回1，商品数量不够返回0，无此id商品返回-1
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


//在商品链表中查找商品序号为id的商品，该商品数量增加quantity
//成功返回1，无此id商品返回-1
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


//清除商品链表中为数量为0的商品
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


//从文件中导入商品数据
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

//将系统内商品数据导出到文件
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


//有ID在商品链表中查找商品，查找到返回链表所在位置，否则返回NULL
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


