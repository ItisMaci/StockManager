#include "StockManager.h"

StockManager::StockManager(){
}

void StockManager::Start()
{
    StockHashTable stockTable;
    bool isRunning = true;
    while(isRunning){
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        menuRender();
        isRunning  = menuInput(stockTable);
        system("pause");
        getchar();
    }
}

void StockManager::menuRender(){
    std::cout << "+-------------------------------+\n";
    std::cout << "|      STOCK MANAGEMER          |\n";
    std::cout << "+-------------------------------+\n";
    std::cout << "|  1. ADD     - Add Stock       |\n";
    std::cout << "|  2. DEL     - Delete Stock    |\n";
    std::cout << "|  3. IMPORT  - Import Data     |\n";
    std::cout << "|  4. SEARCH  - Find Stock      |\n";
    std::cout << "|  5. PLOT    - Show Course     |\n";
    std::cout << "|  6. SAVE    - Save Hashtable  |\n";
    std::cout << "|  7. LOAD    - Load Hashtable  |\n";
    std::cout << "|  0. QUIT    - Quit Program    |\n";
    std::cout << "+-------------------------------+\n";
    std::cout << "[INPUT] Enter your choice [0-7]: ";
}

bool StockManager::menuInput(StockHashTable& stockTable){
    char input;
    bool isRunning = true;
    input = std::cin.get();
    switch (input)
    {
    case '1':
        add(stockTable);
        break;
    case '2':
        del(stockTable);
        break;
    case '3':
        import(stockTable);
        break;
    case '4':
        search(stockTable);
        break;
    case '5':
        plot(stockTable);
        break;
    case '6':
        save(stockTable);
        break;
    case '7':
        load(stockTable);
        break;
    case '#':
        stockTable.printHashTable();
        break;
    case '0':
        isRunning = false;
        break;
    default:
        break;
    }

    return isRunning;
}

void StockManager::add(StockHashTable& stockTable)
{
    std::string name;
    std::string isin;
    std::string ticker;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[INPUT] Enter stock name: " << std::endl;
    std::getline(std::cin, name);

    std::cout << "[INPUT] Enter stock ISIN: " << std::endl;
    std::getline(std::cin, isin);

    std::cout << "[INPUT] Enter stock ticker: " << std::endl;
    std::getline(std::cin, ticker);

    if(name.empty() || isin.empty() || ticker.empty()){
        std::cout << "[WARNING] Please fill out all fields";
        return;
    }
    
    stockTable.addStock(name, isin, ticker);
}

void StockManager::del(StockHashTable& stockTable)
{
    std::string isin;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[INPUT] Enter stock ISIN: " << std::endl;
    
    if (!std::getline(std::cin, isin) || isin.empty()) {
        std::cout << "[WARNING] ISIN cannot be empty!" << std::endl;
        return;
    }

    stockTable.deleteStock(isin);
}

void StockManager::import(StockHashTable& stockTable)
{
    std::string name;
    std::string filename;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[INPUT] Enter stock name: " << std::endl;
    if (!std::getline(std::cin, name) || name.empty()) {
        std::cout << "[WARNING] Name cannot be empty!" << std::endl;
        return;
    }

    std::cout << "[INPUT] Enter filename: " << std::endl;
    if (!std::getline(std::cin, filename) || filename.empty()) {
        std::cout << "[WARNING] Filename cannot be empty!" << std::endl;
        return;
    }

    stockTable.importData(filename, name);
}

void StockManager::search(StockHashTable& stockTable)
{
    std::string value;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[INPUT] Enter stock name or ticker: " << std::endl;
    std::getline(std::cin, value);

    if(value.empty()){
        std::cout << "[WARNING] Please fill out all fields";
        return;
    }

    stockTable.searchStock(value);
}

void StockManager::plot(StockHashTable& stockTable)
{
    std::string isin;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "[INPUT] Enter stock ISIN: " << std::endl;
    
    if (!std::getline(std::cin, isin) || isin.empty()) {
        std::cout << "[WARNING] ISIN cannot be empty!" << std::endl;
        return;
    }

    stockTable.plotStock(isin);
}
void StockManager::save(StockHashTable& stockTable)
{
    stockTable.saveToFile("HashTable.txt");
}

void StockManager::load(StockHashTable& stockTable)
{
    stockTable.loadFromFile("HashTable.txt");
}