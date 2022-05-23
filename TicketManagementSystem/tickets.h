#define MAX_PASSWORD_LENGTH 15
#define MAX_USERNAME_LENGTH 15

#ifndef TICKETS_H
#define TICKETS_H

#define MAXTICKETSNAME 30
#define MAXMANUFACTURERNAME 50


typedef struct {
    int id;
    char name[MAXTICKETSNAME];
    double buying_price;
    double selling_price;
    char manufacturer[MAXMANUFACTURERNAME];
    int quantity;
} Tickets;

typedef struct TicketsListNode *TicketsList;
struct TicketsListNode {
    Tickets goods;
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
    char name[MAXTICKETSNAME];
    double buying_price;
    double selling_price;
    int sold_quantity;
    SoldDate date;
} SoldTicketsRecord;

typedef struct RecordsListNode *RecordsList;
struct RecordsListNode {
    SoldTicketsRecord record;
    RecordsList next;
};

typedef struct {
    char account[15];
    char password[15];
    int permission_level;
} UserAccount;


#endif // !TICKETS_H