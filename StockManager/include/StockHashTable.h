#ifndef STOCKHASHTABLE_H
#define STOCKHASHTABLE_H
#include <iostream>
#include <list>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#define BUCKETS 10

class StockHashTable{
    struct StockPriceData{
        std::string date;
        double close;
        uint32_t volume;
        double open;
        double high;
        double low;

        StockPriceData() : date(""), close(0.0f), volume(0), open(0.0f), high(0.0f), low(0.0f) {}

        StockPriceData(std::string date, double close, uint32_t volume, double open, double high, double low) 
            : date(date), close(close), volume(volume), open(open), high(high), low(low) {}

    };

    struct StockData{
        std::string name;
        std::string isin;
        std::string ticker;
        std::vector<StockPriceData> PriceData;

        StockData() : name(""), isin(""), ticker(""), PriceData() {}

        StockData(std::string name, std::string isin, std::string ticker)
            : name(std::move(name)), isin(std::move(isin)), ticker(std::move(ticker)), PriceData() {}

        StockData(std::string name, std::string isin, std::string ticker, std::vector<StockPriceData> PriceData)
            : name(std::move(name)), isin(std::move(isin)), ticker(std::move(ticker)), PriceData(std::move(PriceData)) {}
    };

    private:

    public:
        std::list<StockData> stockMap[BUCKETS];
        StockHashTable();
        bool isEmpty() const;
        int hashFunction(const std::string& isin);
        void addStock(std::string name, std::string isin, std::string ticker);
        void deleteStock(std::string isin);
        void importData(const std::string& filename, const std::string& name);
        void searchStock(std::string value);
        void plotStock(const std::string& isin);
        void saveToFile(const std::string& filename);
        void loadFromFile(const std::string& filename);
        void printHashTable();
};

#endif