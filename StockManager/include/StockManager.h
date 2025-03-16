#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H
#include <iostream>
#include <string>
#include <limits>
#include "StockHashTable.h"

class StockManager
{
    private:
        
    public:
        StockManager();
        void Start();
        void menuRender();
        bool menuInput(StockHashTable& stockTable);
        void add(StockHashTable& stockTable);
        void del(StockHashTable& stockTable);
        void import(StockHashTable& stockTable);
        void search(StockHashTable& stockTable);
        void plot(StockHashTable& stockTable);
        void save(StockHashTable& stockTable);
        void load(StockHashTable& stockTable);
    };

#endif