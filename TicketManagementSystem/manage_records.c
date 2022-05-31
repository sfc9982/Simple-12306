#include "stdafx.h"

const char TICKETS_SALES_RECORD_PATH[50] = "Data\\sold_tickets_list.txt";

// 初始化销售记录裂变，创建带头结点的链表
RecordsList InitRecordsList()
{
    RecordsList head = (RecordsList) malloc(sizeof(struct RecordsListNode));
    head->next = NULL;

    return head;
}


void DeleteRecordsList(RecordsList head)
{
    RecordsList next;

    while (head)
    {
        next = head->next;
        free(head); // 生命周期末释放内存
        head = next;
    }
}

// 遍历销售记录链表，进行某种操作
void TraverseRecordsList(RecordsList head, void(*Func)(SoldTicketsRecord *))
{
    RecordsList p = head->next;
    while (p)
    {
        Func(&p->record);
        p = p->next;
    }
}

// 输出一个销售记录的信息
void DisplayARecordInfo(SoldTicketsRecord *record)
{
    printf("%d %s %s %s %d %d-%d-%d-%d:%d:%d\n", record->id, record->name, record->station_from,
           record->station_dest, record->sold_quantity, record->date.year, record->date.month, record->date.day,
           record->date.hour, record->date.min, record->date.second);
}

// 在销售记录链表尾部增加一条销售记录信息
void AddRecordToList(RecordsList head, SoldTicketsRecord record)
{
    RecordsList newNode = (RecordsList) malloc(sizeof(struct RecordsListNode));
    newNode->record = record;
    newNode->next = NULL;

    RecordsList p = head;
    while (p->next)
        p = p->next;

    p->next = newNode;
}

FILE *OpenRecordsFile(char *mod)
{
    return fopen(TICKETS_SALES_RECORD_PATH, mod);
}

// 向文件中新增加一条销售记录数据
void AppendARecordToFile(SoldTicketsRecord record, FILE *fp)
{
    fprintf(fp, "%d %s %s %s %d %.2lf %d-%d-%d-%d:%d:%d\n", record.id, record.name, record.station_from,
            record.station_dest, record.sold_quantity, record.price, record.date.year, record.date.month,
            record.date.day,
            record.date.hour, record.date.min, record.date.second);

    fclose(fp);

}

// 从文件中导入销售记录数据
void ImportRecordsFromFile(RecordsList head, FILE *fp)
{
    SoldTicketsRecord record;
    while (!feof(fp))
    {
        fscanf(fp, "%d %s %s %s %d %lf %d-%d-%d-%d:%d:%d\n", &record.id, record.name, record.station_from,
               record.station_dest, &record.sold_quantity, &record.price, &record.date.year, &record.date.month,
               &record.date.day,
               &record.date.hour, &record.date.min, &record.date.second);
        AddRecordToList(head, record);
    }

    fclose(fp);
}

// 获取销售时间
SoldDate GetCurTime()
{
    SoldDate date;
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);

    date.year = tm_now->tm_year + 1900;
    date.month = tm_now->tm_mon + 1;
    date.day = tm_now->tm_mday;
    date.hour = tm_now->tm_hour;
    date.min = tm_now->tm_min;
    date.second = tm_now->tm_sec;

    return date;
}


