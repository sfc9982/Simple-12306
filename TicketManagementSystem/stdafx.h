#ifndef STDAFX_H
#define STDAFX_H

#include "tickets.h"
#include "uthash.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PASSWORD_LENGTH 15
#define MAX_USERNAME_LENGTH 15

//TicketsManagementSystem.c
void DisplayInit();

void DisplayExit();

void LogIn();

void AdminLogIn();

void UserLogIn();


//manage_tickets.c
TicketsList InitTicketsList();

void DeleteTicketsList(TicketsList head);

int AddTicketsToList(TicketsList head, Tickets tickets);

void TraverseTicketsList(TicketsList head, void(*Fun)(Tickets *));

void DisplayTicketsInfo(Tickets *tickets);

void DisplayBasicTicketsInfo(Tickets *tickets);

int IncreaseTicketsQuantity(TicketsList head, int id, int quantity);

int ReduceTicketsQuantity(TicketsList head, int id, int quantity);

void ImportTicketsFromFile(TicketsList head, FILE *fp);

void ExportTicketsToFile(TicketsList head, FILE *fp);

void RemoveZeroQuantityTickets(TicketsList head);

TicketsList FindTicketsByID(TicketsList head, int id);

FILE *OpenTicketsFile(char *mod);


//manage_records.c
RecordsList InitRecordsList();

void DeleteRecordsList(RecordsList head);

void TraverseRecordsList(RecordsList head, void(*Fun)(SoldTicketsRecord *));

void DisplayARecordInfo(SoldTicketsRecord *record);

void AppendARecordToFile(SoldTicketsRecord record, FILE *fp);

void AddRecordToList(RecordsList head, SoldTicketsRecord record);

void ImportRecordsFromFile(RecordsList head, FILE *fp);

SoldDate GetCurTime();

FILE *OpenRecordsFile(char *mod);


//admin_user.c
int IsAdminAccount(char *account, char *password);

int AddAccount(UserAccount *user);

int DeleteAccount(char *account);

int DisplayAccountInfo();

void AdminInitMenu();

void StockManagement();

void AddTicketsToStock(TicketsList head);

void BatchedStock(TicketsList head);

void LookOverStock(TicketsList head);

void LookUpTickets(TicketsList head);

void ShowQueriedTicketsListToAdmin(TicketsList queried_tickets);

void LookUpTicketsByName(TicketsList head);

void LookUpTicketsByManufacturer(TicketsList head);

void LookUpTicketsByNameAndManufacturer(TicketsList head);

void ClearStock(TicketsList head);

void UserManagement();

void LookOverUser();

void AddUser();

void DeleteUser();

void SoldManagement();

void SoldStatistics();

void SingleDaySoldRecords(RecordsList head);

void DaysSoldRecords(RecordsList head);

void DaysSoldStatistics(RecordsList head);


//salesman.c
int IsSalesmanAccount(char *account, char *password);

void DisplaySoldTickets(TicketsList head);

void SoldTickets(TicketsList head);

void SalesmanInitMenu();

void SalesmanLookUpTickets(TicketsList head);

void ShowQueriedTicketsListToSalesman(TicketsList queried_tickets);

void SalesmanLookUpTicketsByName(TicketsList head);

void SalesmanLookUpTicketsByManufacturer(TicketsList head);

void SalesmanLookUpTicketsByNameAndManufacturer(TicketsList head);


//query.c
TicketsList QueryTicketsByName(TicketsList head, char *name);

TicketsList QueryTicketsByManufacturer(TicketsList head, char *manufacturer);

TicketsList QueryTicketsByNameAndManufacturer(TicketsList head, char *tickets_name, char *manufacturer);

void QuerySoldRecordsByDate(RecordsList head, SoldDate start, SoldDate end);


//statistics.c
int CompareDate(SoldDate a, SoldDate b);

void SoldStatisticsByDate(RecordsList head, SoldDate start, SoldDate end, int min_sold_cnt, int min_earnings);


#endif // !STDAFX_H