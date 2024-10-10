#include <iostream>
#include <atomic>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

class entry {
    public:
        int price;
        int quantities;
        int indexOfTrade;
};


void mySwap (entry &a, entry &b) {
            a.price += b.price;
            b.price = a.price - b.price;
            a.price = a.price - b.price;
            a.quantities += b.quantities;
            b.quantities = a.quantities - b.quantities;
            a.quantities = a.quantities - b.quantities;
            a.indexOfTrade += b.indexOfTrade;
            b.indexOfTrade = a.indexOfTrade - b.indexOfTrade;
            a.indexOfTrade = a.indexOfTrade - b.indexOfTrade;                         
        }

int newCompare (entry a, entry b) { //returns 1 if a > b, 2 if a < b, 0 if a = b
    if (a.price > b.price) {
        return 1;
    }
    else if (a.price < b.price) {
        return 2;
    }
    else {
        return 0;
    }
}

entry *newNullBuy(){
    entry *t = new entry;
    t->price = -500;
    t->quantities = 0;
    t->indexOfTrade = -1;
    return t;
}

class newBuyHeap { // keep max price here

    public:
        std::vector<entry> arr;
        
        bool isEmpty() {
            return arr.empty();
        }

        entry *returnMax () {
            if (arr.empty()) {
                return newNullBuy();
            }
            return &arr[0];
        }

        int parent(int i) {
            return (i-1)/2;
        } 

        int left (int i) {
            return 2*i + 1;
        } 
        
        int right (int i) {
            return 2*i + 2;
        }

        void insert (entry t) {
            arr.push_back(t);
            int i = arr.size() - 1;
            while (i > 0 && newCompare(arr[i],arr[parent(i)]) == 1) {
                mySwap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (newCompare(arr[left(i)], arr[i]) == 1) {//if left(i) > i
                    mySwap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (newCompare(arr[left(i)], arr[right(i)]) != 1) {
                    index = right(i);
                }
                if (newCompare(arr[i],arr[index]) != 1) {
                    mySwap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        entry *deleteMax () {
            mySwap(arr[0],arr[arr.size() - 1]);
            entry *t = &arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }

        newBuyHeap() {
            std::vector<entry> t;
            arr = t;
        }

};

bool newIsStringInVec (std::string a, std::vector<std::string> b) {
    for (int i=0; i<b.size(); i++) {
        if (a.compare(b[i]) == 0) {
            return true;
        }
    }
    return false;
}

entry *newNullSell (){
    entry *t = new entry;
    t->price = 999999;
    t->quantities = 0;
    t->indexOfTrade = -1;
    return t;
}

void newSwap (int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}

size_t partition(std::vector<int>& vec, size_t low, size_t high) {
    int pivot = vec[high];
    size_t i = low - 1;

    for (size_t j = low; j < high; ++j) {
        if (vec[j] <= pivot) {
            ++i;
            newSwap(vec[i], vec[j]);
        }
    }

    newSwap(vec[i + 1], vec[high]);
    return i + 1;
}


int quickSelect(std::vector<int>& vec, size_t low, size_t high, size_t k) {
    if (low <= high) {
        size_t partitionIndex = partition(vec, low, high);

        if (partitionIndex == k) {
            return vec[partitionIndex];
        } else if (partitionIndex < k) {
            return quickSelect(vec, partitionIndex + 1, high, k);
        } else {
            return quickSelect(vec, low, partitionIndex - 1, k);
        }
    }

    // This should not happen if k is within a valid range.
    throw std::out_of_range("Invalid index");
}

int findMedian(std::vector<int>& vec) {
    size_t n = vec.size();
    size_t k = n / 2;

    // Find the median index (n/2) using QuickSelect.
    return quickSelect(vec, 0, n - 1, k);
}

class medianStorage {
    public:
        
        std::vector<int> arr;

        void insert (int x) {
            arr.push_back(x);
        }

        int returnMedian () {
            return findMedian(arr);
        }

        medianStorage() {
            std::vector<int> t;
            arr = t;
        }
        medianStorage(int a) {
            std::vector<int> t;
            arr = t;
            arr.push_back(a);
        }
};

class newSellHeap {
    public:
        std::vector<entry> arr;

        bool isEmpty () {
            return arr.empty();
        }

        entry *returnMin () {
            if (arr.empty()) {
                return newNullSell();
            }
            return &arr[0];
        }

        int parent(int i) {
            return (i-1)/2;
        } 

        int left (int i) {
            return 2*i + 1;
        } 
        
        int right (int i) {
            return 2*i + 2;
        }

        void insert (entry t) {
            arr.push_back(t);
            int i = arr.size() - 1;
            while (i > 0 && newCompare(arr[i],arr[parent(i)]) == 2) {
                mySwap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (newCompare(arr[left(i)], arr[i]) == 2) {//if left(i) < i
                    mySwap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (newCompare(arr[left(i)], arr[right(i)]) != 2) {
                    index = right(i);
                }
                if (newCompare(arr[i],arr[index]) != 2) {
                    mySwap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        entry *deleteMin () {
            mySwap(arr[0],arr[arr.size() - 1]);
            entry *t = &arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }
        newSellHeap () {
            std::vector<entry> t;
            arr = t;
        }
};



extern std::atomic<int> commonTimer;
extern std::mutex printMutex;  // Declare the mutex for printing

class pqueue{
public:
    std::vector<std::string> elements;
    size_t front;  
    size_t rear;   
    size_t capacity = 5;

    pqueue(size_t size) : elements(size), front(0), rear(0), capacity(size) {}


    void enqueue(std::string value) {
        if ((rear + 1) % capacity == front) {
            dequeue();
        }
        elements[rear] = value;
        rear = (rear + 1) % capacity;
    }

    void dequeue() {
        if (front == rear) {
            return;
        }
        front = (front + 1) % capacity;
    }

};

void processLine (std::string &t, bool &bruh) {
    if (t[t.size() - 1] == '\n') {
        t.resize(t.size() - 1);
    }
    else {
        bruh = true;
    }
}

bool isItUnique (std::string a, std::vector<std::string> b) {
    if (b.empty()) {
        return true;
    }
    bool u = false;
    for (int i=0; i<b.size(); i++) {
        if (a.compare(b[i]) == 0) {
            u = true;
            return true;
        }
    }
    return false;
}

int convertMoney (std::string money) {
    if (money[0] != '$') {
        return -1;
    }
    else {
        std::string t;
        for (int i=1; i<money.size(); i++) {
            t += money[i];
        }
        return std::stoi(t);
    }
}

bool newVecStringComp (std::vector<std::string> tokensA, std::vector<std::string> tokensB) {
    std::vector<std::string> quantsA;
    std::vector<std::string> quantsB;
    std::vector<std::string> stonksA;
    std::vector<std::string> stonksB;

    for (int i=0; i<tokensA.size(); i++) {
        if (i%2 == 0) {
            stonksA.push_back(tokensA[i]);
        }
        else {
            quantsA.push_back(tokensA[i]);
        }
    }

    for (int i=0; i<tokensB.size(); i++) {
        if (i%2 == 0) {
            stonksB.push_back(tokensB[i]);
        }
        else {
            quantsB.push_back(tokensB[i]);
        }
    }

    if (tokensA.size() != tokensB.size()) {
        return false;
    }

    for (int i=0; i<stonksA.size(); i++) {
        std::string sus = stonksA[i];
        bool match = false;
        for (int j=0; j<stonksB.size(); j++) {
            if (sus.compare(stonksB[j]) == 0 && quantsA[i].compare(quantsB[j]) == 0) {
                match = true;
            }
        }
        if (!match) {
            return false;
        }
    }

    return true;

}

bool linCombComp (std::string a, std::string b) {
    std::istringstream TA(a);
    std::vector<std::string> tokensA;
    std::string token;
    while (TA >> token) {
        tokensA.push_back(token);
    }
    std::istringstream B(b);
    std::vector<std::string> tokensB;
    std::string token2;
    while (B >> token2) {
        tokensB.push_back(token2);
    }
    if (tokensA.size() != tokensB.size()) {
        return false;
    }
    if (tokensA.size() == 1) {
        return (a.compare(b) == 0);
    }
    else {
        return newVecStringComp(tokensA,tokensB);
    }
}

int findMatchIndex (std::string a, std::vector<std::string> b) {
    int ind = -1;
    for (int i=0; i<b.size(); i++) {
        if (linCombComp(a,b[i])) {
            ind = i;
            return ind;
        }
    }
    return ind;
}

int convertQuant (std::string a) {
    if (a[0] != '#') {
        return -1;
    }
    else {
        std::string b;
        for (int i=1; i<a.size(); i++) {
            b += a[i];
        }
        return std::stoi(b);
    }
}

int reader(int time)
{

    //std::cout<<"bruh\r\n";
    bool isOngoing = true;
    //std::cout << t << " Harry SELL AMD $1 #32 3" << std::endl;
    //int multiplicity = 1;
    int counter = 0;
    int bruhCount = 0;
    //std::vector<std::string> lines;

    std::vector<newBuyHeap> activeBuys; //activeBuys[i][j] denotes the jth price for buying ith stock
    std::vector<newSellHeap> activeSells;//activeSells[i][j] denotes jth price for selling ith stock
    std::vector<std::string> uniqueStocks;
    std::vector<int> worth;
    std::vector<medianStorage> newWorth; //this will make worth obsolete :)
    std::vector<std::string> stocks;
    std::vector<std::string> money;
    std::vector<std::string> buyOrSell;
    std::vector<std::string> netQuant;
    std::vector<std::vector<int>> buyQuants;
    std::vector<std::vector<int>> sellQuants;

    //medianStorage commonMedian;

    const std::string name = "22b0934_22b0953";
    std::string sell = "SELL";
    std::string buy = "BUY";
    int prevLineSize = 0;
    while (isOngoing) {
        //counter ++;

        bruhCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::vector<std::string> lines;
        std::ifstream inputFile ("output.txt");
        std::string line;
        while (std::getline(inputFile,line)) {
            if (line.compare("TL") == 0) {
                continue;
            }
            if (line.compare("\0") == 0 || line.compare("\r\n") == 0 || line.compare("\n") == 0 || line.compare("!@") == 0 || line.compare("@!") == 0 || line.compare("Successfully Initiated!") == 0 || line.size() < 1) {
                    return 1;
                    isOngoing = false;
                    break;
                    
            }
            bool u = false;
            processLine(line,u);
            //if (u) {
              //  return 1;
            //}
            lines.push_back(line);
        }
        //if (counter != 0) {
        //    std::cout<<"lines size is "<<lines.size()<<" and counter is "<<counter<<"\r\n";
            //numLinesPrinted++;
        int numLinesPrinted = 0;
        if (prevLineSize >= lines.size()) {
           // std::cout<<prevLineSize<<" "<<lines.size()<<"\r\n";
           // numLinesPrinted++;
            return 1;
        }
        prevLineSize = lines.size();
        if (!isOngoing) {
            break;
        }
        //int numLinesPrinted = 0;
        int timeEstimate = commonTimer.load() + 1;
        
        //process :)
        for (int i=counter; i<lines.size(); i++) {

            std::istringstream iss (lines[i]);
            std::vector<std::string> tokens;
            std::string token;
            while (iss >> token) {
            tokens.push_back(token);
            }
            
            //std::cout<<tokens[0]<<" ";
            int timeStamp = std::stoi(tokens[0]);
            int expiry = std::stoi(tokens[tokens.size() - 1]);
            tokens.pop_back();
            netQuant.push_back(tokens[tokens.size() - 1]);
            tokens.pop_back();
            money.push_back(tokens[tokens.size() - 1]);
            tokens.pop_back();

            if (timeStamp + expiry < timeEstimate && expiry != -1) {
                continue;
            }

            int expires = -1;
            if (expiry != -1) {
                expires = timeStamp + expiry - commonTimer.load();
            }

            buyOrSell.push_back(tokens[2]);
            std::string stonk;
            for (int j=3; j<tokens.size(); j++) {
                stonk.append(tokens[j]);
                stonk.append(" ");
            }
            stocks.push_back(stonk);

            //if (stocks[stocks.size() - 1].compare("GE ")==0) {
            //    std::cout<<"bruh "<<findMatchIndex("GE ",uniqueStocks)<<" and i is "<<i<<"\r\n";
            //}

            bool isSell = false;

            if (buyOrSell[buyOrSell.size() - 1].compare(sell) == 0) {
                isSell = true;
            } 

            if (findMatchIndex(stocks[stocks.size() - 1],uniqueStocks) == -1) {
                newBuyHeap yeStock;
                newSellHeap bruh;
                activeBuys.push_back(yeStock);
                activeSells.push_back(bruh);
                uniqueStocks.push_back(stocks[stocks.size() - 1]);
                worth.push_back(convertMoney(money[money.size() - 1]));
                medianStorage t(convertMoney(money[money.size() - 1]));
                int tradeVal = convertMoney(money[money.size() - 1]);
                for (int i=1; i<convertQuant(netQuant[netQuant.size() - 1]); i++) {
                    t.insert(tradeVal);
                }
                //commonMedian.insert(convertMoney(money[money.size() - 1]));
                newWorth.push_back(t);
                //std::cout<<commonTimer.load()<<" "<<name<<" ";
                if (isSell) {
                    //std::cout<<commonTimer.load()<<" "<<name<<" "<<buy<<" "<<stocks[stocks.size() - 1]<<money[money.size() - 1]<<" "<<netQuant[netQuant.size() - 1]<<" 0\r\n";
                    entry t;
                    t.price = convertMoney(money[money.size() - 1]);
                    t.quantities = convertQuant(netQuant[netQuant.size() - 1]);
                    t.indexOfTrade = stocks.size()  - 1;
                    activeSells[activeSells.size() - 1].insert(t); 
                }
                else {
                    //std::cout<<commonTimer.load()<<" "<<name<<" "<<sell<<" "<<stocks[stocks.size() - 1]<<money[money.size() - 1]<<" "<<netQuant[netQuant.size() - 1]<<" 0\r\n";
                    entry t;
                    t.price = convertMoney(money[money.size() - 1]);
                    t.quantities = convertQuant(netQuant[netQuant.size() - 1]);
                    t.indexOfTrade = stocks.size()  - 1;
                    activeBuys[activeBuys.size() - 1].insert(t); 
                }

                //numLinesPrinted++;
                //if (uniqueStocks[uniqueStocks.size() - 1].compare("GE ") == 0) {
                //    std::cout<<"dekh bhai yaha toh pohoche\r\n";
                //    return 1;
                //}
                continue;
            }
            else {
                int index = findMatchIndex (stocks[stocks.size() - 1],uniqueStocks);
                //push it in the median storage
                //cancellation checking i.e. has the market already dealt with this one?
                if (isSell) {

                    int tmpStorage = convertQuant(netQuant[netQuant.size() - 1]);

                    int tradeVal = convertMoney(money[money.size() - 1]);

                    if (tradeVal <= activeBuys[index].returnMax()->price) {
                        //it's already been traded. lets update activeBuys[index] shit.
                        int currStockQuant = convertQuant(netQuant[netQuant.size() - 1]);
                        int wohStockQuant = activeBuys[index].returnMax()->quantities;

                        if (currStockQuant < wohStockQuant) {
                            //update wohStockQuant
                            wohStockQuant -= currStockQuant;
                            for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                            activeBuys[index].returnMax()->quantities = wohStockQuant;
                            currStockQuant = 0;
                            //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            continue;
                        }
                        while (currStockQuant >= wohStockQuant && currStockQuant > 0 && tradeVal <= activeBuys[index].returnMax()->price) {
                            //cancel activeBuys[index] ka maxWallah
                            entry *t = activeBuys[index].deleteMax();
                            currStockQuant -= wohStockQuant;
                            wohStockQuant = activeBuys[index].returnMax()->quantities;
                            //could be wrong here... but I think thats it...                
                            //u are wrong, we keep trading.
                        }
                        if (currStockQuant <= 0) {
                            for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                            //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            continue;
                        }
                        netQuant[netQuant.size() - 1] = "#";
                        netQuant[netQuant.size() - 1].append(std::to_string(currStockQuant));
                    }
                    // well if its being sold below our worth of the stock. Let's take it :)
                    if (tradeVal < newWorth[index].returnMedian()) {
                        //we take it and update out worth
                        int toPrint = commonTimer.load();
                        //if (toPrint > timeStamp) {
                          //  toPrint = timeStamp;
                        //}
                        std::cout<<toPrint<<" "<<name<<" "<<buy<<" "<<stocks[stocks.size() - 1]<<money[money.size() - 1]<<" "<<netQuant[netQuant.size() - 1]<<" "<<expires<<"\r\n";
                        worth[index] = tradeVal;
                        numLinesPrinted++;
                                                    for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                        
                        //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                    }
                    else {
                        // add it to the list of active sells
                        entry t;
                        t.price = tradeVal;
                        t.indexOfTrade = stocks.size() - 1;
                        t.quantities = convertQuant(netQuant[netQuant.size() - 1]);
                        activeSells[index].insert(t);
                       
                                                   for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                        //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                    }
                }
                else {
                    int tmpStorage = convertQuant(netQuant[netQuant.size() - 1]);
                    int tradeVal = convertMoney(money[money.size() - 1]);
                    if (tradeVal >= activeSells[activeSells.size() - 1].returnMin()->price) {
                        
                        //already trades
                        int currStockQuant = convertQuant(netQuant[netQuant.size() - 1]);
                        int wohStockQuant = activeSells[index].returnMin()->quantities;
                        if (currStockQuant < wohStockQuant) {
                            wohStockQuant -= currStockQuant;
                            activeSells[index].returnMin()->quantities = wohStockQuant;
                            currStockQuant = 0;
                                                        for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                            
                            //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            continue;
                        }
                        while (currStockQuant >= wohStockQuant && currStockQuant > 0 && tradeVal >= activeSells[index].returnMin()->price) {
                            entry *t = activeSells[index].deleteMin();
                            currStockQuant -= wohStockQuant;
                            wohStockQuant = activeSells[index].returnMin()->quantities;
                        }
                        if (currStockQuant <= 0) {
                            //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                                                        for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                            
                            continue;
                        }
                        netQuant[netQuant.size() - 1] = "#";
                        netQuant[netQuant.size() - 1].append(std::to_string(currStockQuant));
                    }
                    if (tradeVal > newWorth[index].returnMedian()) {
                        //std::cout<<"median is "<<newWorth[index].returnMedian()<<" ";
                        int toPrint = commonTimer.load();
                       // if (toPrint > timeStamp) {
                         //   toPrint = timeStamp;
                        //}                        
                        std::cout<<toPrint<<" "<<name<<" "<<sell<<" "<<stocks[stocks.size() - 1]<<money[money.size() - 1]<<" "<<netQuant[netQuant.size() - 1]<<" "<<expires<<"\r\n";
                        worth[index] = tradeVal;
                                                    for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                        
                        //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                        numLinesPrinted++;
                    }
                    else {
                        entry t;
                        t.price = tradeVal;
                        t.quantities = convertQuant(netQuant[netQuant.size() - 1]);
                        t.indexOfTrade = stocks.size() - 1;
                        activeBuys[index].insert(t);
                                                    for (int i=0; i<tmpStorage; i++) {
                                newWorth[index].insert(convertMoney(money[money.size() - 1]));
                            }
                        
                        //newWorth[index].insert(convertMoney(money[money.size() - 1]));
                    }
                }
            }

        }
        //std::cout<<"bruh\r\n";
        //numLinesPrinted++;
        counter=lines.size();
        counter+=numLinesPrinted;
    }
    return 1;
}

int trader(std::string *message)
{
    return 1;
}

void* userThread(void* arg)
{
    int thread_id = *(int*)arg;
    while (true) {
        int currentTime;
        {
            currentTime = commonTimer.load();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        int end = reader(currentTime);
        if (end) break;
    }
    return nullptr;
}

void* userTrader(void* arg)
{
    return nullptr;
}