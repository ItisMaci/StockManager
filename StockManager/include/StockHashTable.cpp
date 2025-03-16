#include "StockHashTable.h"

StockHashTable::StockHashTable(){}

int StockHashTable::hashFunction(const std::string& isin){
    const int p = 31;
    const int M = BUCKETS;

    int hashValue = 0;
    int pPow = 1;

    for (char c : isin) {
        hashValue = (hashValue + (c - '0' + 1) * pPow) % M;
        pPow = (pPow * p) % M;
    }

    return hashValue;
}

void StockHashTable::addStock(std::string name, std::string isin, std::string ticker){
    int hashValue = hashFunction(isin);
    auto& cell = stockMap[hashValue];

    for(auto& stock : cell){
        if(stock.isin == isin){
            std::cout << "[WARNING] Stock already exists." << std::endl;
            return;
        }
    }

    cell.emplace_back(name, isin, ticker);
    std::cout << "Added stock!" << std::endl;
}

void StockHashTable::deleteStock(std::string isin){
    int hashValue = hashFunction(isin);
    auto& cell = stockMap[hashValue];

    for(auto stock = cell.begin(); stock != cell.end();){
        if(stock->isin == isin){
            stock = cell.erase(stock);
            std::cout << "[INFO] Stock removed." << std::endl;
            return;
        }
        else{
            stock++;
        }
    }

    std::cout << "[WARNING] Stock does not exist." << std::endl;
}


void StockHashTable::importData(const std::string& filename, const std::string& name) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    std::vector<StockPriceData> priceHistory;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, closeStr, volumeStr, openStr, highStr, lowStr;

        std::getline(ss, date, ',');
        std::getline(ss, closeStr, ',');
        std::getline(ss, volumeStr, ',');
        std::getline(ss, openStr, ',');
        std::getline(ss, highStr, ',');
        std::getline(ss, lowStr, ',');

        // Remove '$' from prices
        double close = std::stod(closeStr.substr(1));
        double open = std::stod(openStr.substr(1));
        double high = std::stod(highStr.substr(1));
        double low = std::stod(lowStr.substr(1));
        uint32_t volume = std::stoul(volumeStr);

        priceHistory.emplace_back(date, close, volume, open, high, low);
    }

    file.close();

    // Find stock in the hash table
    for (auto& list : stockMap) {
        for (auto& stock : list) {
            if (stock.name == name) {
                stock.PriceData = std::move(priceHistory);
                std::cout << "[INFO] Imported data for " << name << std::endl;
                return;
            }
        }
    }

    std::cerr << "[WARNING] Stock not found: " << name << std::endl;
}

void StockHashTable::searchStock(std::string value){
    bool found = false;

    for(auto& list : stockMap){
        for(auto&stock : list){
            if(stock.name == value || stock.ticker == value){
                std::cout << "| " << stock.name << " - " << stock.isin<< " - " << stock.ticker << std::endl;
                if(stock.PriceData.empty()){
                    std::cout << "Stock Price Data is Empty" << std::endl;
                }
                else{
                    int width = 10;
                    
                    std::cout << "| " << std::left << std::setw(8) << "Date" << std::right << std::setw(width) << stock.PriceData.front().date << std::endl;
                    std::cout << "| " << std::left << std::setw(8) << "Close" << std::right << std::setw(width) << stock.PriceData.front().close << std::endl;
                    std::cout << "| " << std::left << std::setw(8) << "Volume" << std::right << std::setw(width) << stock.PriceData.front().volume << std::endl;
                    std::cout << "| " << std::left << std::setw(8) << "High" << std::right << std::setw(width) << stock.PriceData.front().high << std::endl;
                    std::cout << "| " << std::left << std::setw(8) << "Low" << std::right << std::setw(width) << stock.PriceData.front().low << std::endl;
                }
                found = true;
                break;
            }
        }
        if(found) break;
    }
}

void StockHashTable::plotStock(const std::string& isin){
    int hashValue = hashFunction(isin);
    auto& cell = stockMap[hashValue];

    for(auto&stock : cell){
        if(stock.isin == isin){
            std::cout << "| " << stock.name << " - " << stock.isin<< " - " << stock.ticker << std::endl;
            if(stock.PriceData.empty()){
                std::cout << "Stock Price Data is Empty" << std::endl;
            }
            else{
                int width = 20;
                std::cout << "| " << std::left << std::setw(10) << "Date" << std::right << std::setw(width) << "Closing Price" << std::endl;
                for(auto stockData : stock.PriceData){
                    std::cout << "| " << std::left << std::setw(8) << stockData.date << std::right << std::setw(width) << stockData.close << std::endl;
                }
            }
            break;
        }
    }
}

void StockHashTable::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& bucket : stockMap) {
        for (const auto& stock : bucket) {
            // Save StockData (Stock Name, ISIN, Ticker)
            file << stock.name << "," << stock.isin << "," << stock.ticker << "\n";

            // Save StockPriceData (Date, Close, Volume, Open, High, Low)
            for (const auto& price : stock.PriceData) {
                file << price.date << "," << price.close << "," << price.volume << ","
                     << price.open << "," << price.high << "," << price.low << "\n";
            }

            // Separate stocks with an empty line for clarity
            file << "\n";
        }
    }

    file.close();
    std::cout << "[INFO] Stock data saved to " << filename << std::endl;
}

void StockHashTable::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "[ERROR] Could not open file for reading: " << filename << std::endl;
        return;
    }

    // Clear the hash table before loading new data
    for (int i = 0; i < BUCKETS; i++) {
        stockMap[i].clear();
    }

    std::string line;
    StockData currentStock;

    while (std::getline(file, line)) {
        if (line.empty()) {
            // Insert stock into hash table if it has data
            if (!currentStock.isin.empty()) {
                int hashValue = hashFunction(currentStock.isin);
                stockMap[hashValue].push_back(currentStock);
                currentStock = StockData(); // Reset for next stock
            }
            continue;
        }

        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() == 3) {
            // It's a stock (Name, ISIN, Ticker)
            currentStock = StockData(fields[0], fields[1], fields[2]);
        } 
        else if (fields.size() == 6) {
            // It's price data (Date, Close, Volume, Open, High, Low)
            StockPriceData priceData(fields[0], std::stod(fields[1]), std::stoul(fields[2]),
                                     std::stod(fields[3]), std::stod(fields[4]), std::stod(fields[5]));
            currentStock.PriceData.push_back(priceData);
        }
    }

    // Add last stock (if any)
    if (!currentStock.isin.empty()) {
        int hashValue = hashFunction(currentStock.isin);
        stockMap[hashValue].push_back(currentStock);
    }

    file.close();
    std::cout << "[INFO] Stock data loaded from " << filename << std::endl;
}


// For testing purposes
void StockHashTable::printHashTable()
{
    for(auto& list : stockMap){
        std::cout << "[]";
        for(auto& stock : list){
            std::cout << " -> | " << stock.name 
                        << " - " << stock.isin
                        << " - " << stock.ticker
                        << " |";
        }
        std::cout << std::endl;
    }
}