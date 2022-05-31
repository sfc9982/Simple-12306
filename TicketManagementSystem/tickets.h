#define MAX_PASSWORD_LENGTH 15
#define MAX_USERNAME_LENGTH 15

#ifndef TICKETS_H
#define TICKETS_H

#define MAX_LABEL_LENGTH 30
#define MAX_TRAIN_TYPE_LENGTH 50
#define MAX_STATION_NAME_LENGTH 100 // Unicode,UTF-8中一个中文字符占据3个字节，所以长度应为中文长度的三倍以上

typedef struct {
    int id;
    char name[MAX_LABEL_LENGTH];
    char station_from[MAX_STATION_NAME_LENGTH];
    char station_dest[MAX_STATION_NAME_LENGTH];
    char manufacturer[MAX_TRAIN_TYPE_LENGTH];
    double price;
    int quantity;
} Tickets;


typedef struct TicketsListNode *TicketsList;
struct TicketsListNode {
    Tickets tickets;
    TicketsList next;
};

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int second;
} SoldDate;


typedef struct {
    int id;
    char name[MAX_LABEL_LENGTH];
    char station_from[MAX_STATION_NAME_LENGTH];
    char station_dest[MAX_STATION_NAME_LENGTH];
    double price;
    int sold_quantity;
    SoldDate date;
} SoldTicketsRecord;

typedef struct RecordsListNode *RecordsList;
struct RecordsListNode {
    SoldTicketsRecord record;
    RecordsList next;
};

typedef struct {
    char account[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int permission_level;
} UserAccount;


#endif // !TICKETS_H