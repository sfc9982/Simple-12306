#ifndef STDAFX_H
#define STDAFX_H

#include "tickets.h"
#include "uthash.h"

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//SupermarketManagementSystem.c
void DisplayInit();

void DisplayExit();

void LogIn();

void AdminLogIn();

void SalesmanLogIn();


//manage_goods.c
GoodsList InitGoodsList();

void DeleteGoodsList(GoodsList head);

int AddGoodsToList(GoodsList head, Goods goods);

void TraverseGoodsList(GoodsList head, void(*Fun)(Goods *));

void DisplayGoodsInfo(Goods *goods);

void DisplayBasicGoodsInfo(Goods *goods);

int IncreaseGoodsQuantity(GoodsList head, int id, int quantity);

int ReduceGoodsQuantity(GoodsList head, int id, int quantity);

void ImportGoodsFromFile(GoodsList head, FILE *fp);

void ExportGoodsToFile(GoodsList head, FILE *fp);

void RemoveZeroQuantityGoods(GoodsList head);

GoodsList FindGoodsByID(GoodsList head, int id);

FILE *OpenGoodsFile(char *mod);


//manage_records.c
RecordsList InitRecordsList();

void DeleteRecordsList(RecordsList head);

void TraverseRecordsList(RecordsList head, void(*Fun)(SoldGoodsRecord *));

void DisplayARecordInfo(SoldGoodsRecord *record);

void AppendARecordToFile(SoldGoodsRecord record, FILE *fp);

void AddRecordToList(RecordsList head, SoldGoodsRecord record);

void ImportRecordsFromFile(RecordsList head, FILE *fp);

SoldDate GetNowDate();

FILE *OpenRecordsFile(char *mod);


//admin_user.c
int IsAdminAccount(char *account, char *password);

int AddAccount(UserAccount *user);

int DeleteAccount(char *account);

int DisplayAccountInfo();

void AdminInitMenu();

void StockManagement();

void AddGoodsToStock(GoodsList head);

void BatchedStock(GoodsList head);

void LookOverStock(GoodsList head);

void LookUpGoods(GoodsList head);

void ShowQueriedGoodsListToAdmin(GoodsList queried_goods);

void LookUpGoodsByName(GoodsList head);

void LookUpGoodsByManufacturer(GoodsList head);

void LookUpGoodsByNameAndManufacturer(GoodsList head);

void ClearStock(GoodsList head);

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

void DisplaySoldGoods(GoodsList head);

void SoldGoods(GoodsList head);

void SalesmanInitMenu();

void SalesmanLookUpGoods(GoodsList head);

void ShowQueriedGoodsListToSalesman(GoodsList queried_goods);

void SalesmanLookUpGoodsByName(GoodsList head);

void SalesmanLookUpGoodsByManufacturer(GoodsList head);

void SalesmanLookUpGoodsByNameAndManufacturer(GoodsList head);


//query.c
GoodsList QueryGoodsByName(GoodsList head, char *name);

GoodsList QueryGoodsByManufacturer(GoodsList head, char *manufacturer);

GoodsList QueryGoodsByNameAndManufacturer(GoodsList head, char *goods_name, char *manufacturer);

void QuerySoldRecordsByDate(RecordsList head, SoldDate start, SoldDate end);


//statistics.c
int CompareDate(SoldDate a, SoldDate b);

void SoldStatisticsByDate(RecordsList head, SoldDate start, SoldDate end, int min_sold_cnt, int min_earnings);


#endif // !STDAFX_H