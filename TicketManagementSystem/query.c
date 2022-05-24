#include "stdafx.h"

// ���ճ�Ʊ��ǰ׺��ѯ��Ʊ�������Ʊ������Ϣ
TicketsList QueryTicketsByName(TicketsList head, char *name)
{
    return QueryTicketsByNameAndManufacturer(head, name, "");
}

// ���ճ���ǰ׺��ѯ��Ʊ�������Ʊ������Ϣ
TicketsList QueryTicketsByManufacturer(TicketsList head, char *manufacturer)
{
    return QueryTicketsByNameAndManufacturer(head, "", manufacturer);
}

// ���ճ�Ʊ���ͳ���ǰ׺��ѯ��Ʊ�������Ʊ������Ϣ
TicketsList QueryTicketsByNameAndManufacturer(TicketsList head, char *goods_name, char *manufacturer)
{
    if (head->next == NULL)
        return NULL;


    TicketsList queried_goods = InitTicketsList();
    TicketsList p = head->next;
    while (p)
    {
        char goods_name_prefix[MAX_LABEL_LENGTH] = {0};
        char manufacturer_prefix[MAX_TRAIN_TYPE_LENGTH] = {0};
        strncpy(goods_name_prefix, p->goods.name, strlen(goods_name));
        strncpy(manufacturer_prefix, p->goods.manufacturer, strlen(manufacturer));
        if (strcmp(goods_name_prefix, goods_name) == 0 && strcmp(manufacturer_prefix, manufacturer) == 0)
        {
            TicketsList newNode = (TicketsList) malloc(sizeof(struct TicketsListNode));
            newNode->goods = p->goods;
            newNode->next = queried_goods->next;
            queried_goods->next = newNode;
        }
        p = p->next;
    }

    return queried_goods;
}


void QuerySoldRecordsByDate(RecordsList head, SoldDate start, SoldDate end)
{
    if (head->next == NULL)
    {
        printf("���ۼ�¼Ϊ��\n");
        return;
    }

    RecordsList p = head->next;
    printf("---------------------------------------------------------------------------------------\n");
    printf("%-5s %-12s %-20s %-20s %3s %5s %15s\n", "ID", "����", "ʼ��վ", "�յ�վ", "Ʊ��", "������", "����ʱ��");
    while (p)
    {
        if (CompareDate(p->record.date, start) >= 0 && CompareDate(p->record.date, end) <= 0)
        {
            printf("%-5d %-12s %-20s %-20s %-5.2lf %-6d %4d\\%02d\\%02d %02d:%02d:%02d\n", p->record.id, p->record.name,
                   p->record.station_from, p->record.station_dest, p->record.price, p->record.sold_quantity,
                   p->record.date.year,
                   p->record.date.month, p->record.date.day, p->record.date.hour, p->record.date.min,
                   p->record.date.second);
        }
        p = p->next;
    }

    printf("---------------------------------------------------------------------------------------\n");

}
