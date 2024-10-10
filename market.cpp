#include "market.h"
#include<iostream>
#include<fstream>
#include<sstream>
extern int reader(int time);
extern int trader(std::string* message);

market::market(int argc, char** argv)
{
    std::ifstream inputFile ("output.txt");
    std::string line;
    while (std::getline(inputFile,line)) {
        if (line.compare("TL") == 0) {
            continue;
        }
        //std::cout<<"bruh\n";
        //std::cout<<line;
        if (line.compare("!@") == 0 || line.compare("Successfully Initiated!") == 0) {
            break;
        }
        lines.push_back(line);
    }

    int n = lines.size();
    std::string bruh[n];
    for (int i=0; i<n; i++) {
        bruh[i] = lines[i];
    }

    if (argc == 2) {
        //std::cout<<"yaha pohoche\r\n";
        //std::cout<<argv[0]<<"\r\n";
        if (argv[0][0] == '.' && argv[0][1] == '/' && argv[0][2] == 't' &&argv[0][3] == 'r' && argv[0][4] == 'a' && argv[0][5] == 'd' && argv[0][6] == 'e' && argv[0][7] == 'r') {
            //while (!lines.empty()) {
              //  lines.pop_back();
            //}
        }
    }

}

int timeOfExpiry (std::string s, std::string r) {
    int t1 = std::stoi(s);
    int t2 = std::stoi(r);
    return t1+t2;
}

class entry {
    public:
        int price;
        int quantities;
        int indexOfTrade;
};

    int compare (entry a, entry b) { //returns 1 if a > b, 2 if a < b, 0 if a = b
        if (a.price > b.price) {
            return 1;
        }
        else if (a.price < b.price) {
            return 2;
        }
        else {
            if (a.indexOfTrade > b.indexOfTrade) {
                return 2;
            }
            else if (a.indexOfTrade < b.indexOfTrade) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
        
void swap (entry &a, entry &b) {
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

entry *nullBuy(){
    entry *t = new entry;
    t->price = -500;
    t->quantities = 0;
    t->indexOfTrade = -1;
    return t;
}

class buyHeap { // keep max price here

    public:
        std::vector<entry> arr;
        
        bool isEmpty() {
            return arr.empty();
        }

        entry *returnMax () {
            if (arr.empty()) {
                return nullBuy();
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
            while (i > 0 && compare(arr[i],arr[parent(i)]) == 1) {
                swap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (compare(arr[left(i)], arr[i]) == 1) {//if left(i) > i
                    swap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (compare(arr[left(i)], arr[right(i)]) != 1) {
                    index = right(i);
                }
                if (compare(arr[i],arr[index]) != 1) {
                    swap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        entry *deleteMax () {
            swap(arr[0],arr[arr.size() - 1]);
            entry *t = &arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }

};

bool isStringInVec (std::string a, std::vector<std::string> b) {
    for (int i=0; i<b.size(); i++) {
        if (a.compare(b[i]) == 0) {
            return true;
        }
    }
    return false;
}

entry *nullSell (){
    entry *t = new entry;
    t->price = 999999;
    t->quantities = 0;
    t->indexOfTrade = -1;
    return t;
}

class sellHeap {
    public:
        std::vector<entry> arr;

        bool isEmpty () {
            return arr.empty();
        }

        entry *returnMin () {
            if (arr.empty()) {
                return nullSell();
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
            while (i > 0 && compare(arr[i],arr[parent(i)]) == 2) {
                swap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (compare(arr[left(i)], arr[i]) == 2) {//if left(i) < i
                    swap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (compare(arr[left(i)], arr[right(i)]) != 2) {
                    index = right(i);
                }
                if (compare(arr[i],arr[index]) != 2) {
                    swap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        entry *deleteMin () {
            swap(arr[0],arr[arr.size() - 1]);
            entry *t = &arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }

};

void printBuy (buyHeap b) {
    for (int i=0; i<b.arr.size(); i++) {
        std::cout<<b.arr[i].price<<"-"<<b.arr[i].quantities<<" ";
    }
    std::cout<<"\r\n";
}


void printSell (sellHeap b) {
    for (int i=0; i<b.arr.size(); i++) {
        std::cout<<b.arr[i].price<<"-"<<b.arr[i].quantities<<" ";
    }
    std::cout<<"\r\n";
}

int convertPrice (std::string s) {
    if (s[0] != '$') {
        return -1;
    }
    else {
        std::string t;
        for (int i=1; i<s.size(); i++) {
            t += s[i];
        }
        return std::stoi(t);
    }
}

int convertQuantity (std::string s) {
    if (s[0] != '#') {
        return -1;
    }
    else {
        std::string t;
        for (int i=1; i<s.size(); i++) {
            t+=s[i];
        }
        return std::stoi(t);
    }
}

class portfolio {

    public:
        std::string companyName;
        int numBuys;
        int numSells;
        int netTransfer;

    portfolio(std::string bruh) {
        companyName = bruh;
        numBuys = 0;
        numSells = 0;
        netTransfer = 0;
    }

    bool compare (std::string tmp) {
        return (companyName.compare(tmp) == 0);
    }

};
bool vecStringComp (std::vector<std::string> a, std::vector<std::string> b) {
    std::vector<std::string> stocksA;
    std::vector<std::string> stocksB;
    std::vector<std::string> quantsA;
    std::vector<std::string> quantsB;
    for (int i=0; i<a.size(); i++) {
        if (i%2 == 0) {
            stocksA.push_back(a[i]);
        }
        else {
            quantsA.push_back(a[i]);
        }
    }
    for (int i=0; i<b.size(); i++) {
        if (i%2 == 0) {
            stocksB.push_back(b[i]);
        }
        else {
            quantsB.push_back(b[i]);
        }
    }
    if (a.size() != b.size()) {
        return false;
    }
    for (int i=0; i<stocksA.size(); i++) {
        std::string sus = stocksA[i];
        bool match = false;
        for (int j=0; j<stocksB.size(); j++) {
            if (sus.compare(stocksB[j]) == 0 && quantsA[i].compare(quantsB[j]) == 0) {
                match = true;
            }
        }
        if (!match) {
            return false;
        }
    }
    return true;
}

bool areSame (std::string a, std::string b) {
    std::istringstream iss(a);
    std::vector<std::string> tokensA;
    std::string token;
    while (iss >> token) {
        tokensA.push_back(token);
    }
    std::istringstream issTwo(b);
    std::vector<std::string> tokensB;
    std::string tB;
    while (issTwo >> tB) {
        tokensB.push_back(tB);
    }

    if (tokensA.size() != tokensB.size()) {
        return false;
    }

    if (tokensA.size() == 1) {
        return (token.compare(tB) == 0);
    }

    else {
        return vecStringComp (tokensA,tokensB);
    }
    
}

void market::start()
{
    std::vector<std::string> timeStamp;
    std::vector<std::string> company;
    std::vector<std::string> buyOrSell;
    std::vector<std::string> stockName;
    std::vector<std::string> price;
    std::vector<std::string> maxQuantity;
    std::vector<std::string> expiryTime;
    std::vector<bool> isExpired;
    std::vector<std::string> buyingComps;
    std::vector<std::string> sellingComps;
    std::vector<int> purchasingPrice;
    std::vector<int> numSharesPurchased;

    std::vector<std::string> uniqueStocks; 
    std::vector<sellHeap> uniqueSells;
    std::vector<buyHeap> uniqueBuys;

	for (int i=0; i<lines.size(); i++) {
        std::istringstream iss(lines[i]);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        timeStamp.push_back(tokens[0]);
        company.push_back(tokens[1]);
        buyOrSell.push_back(tokens[2]);

        expiryTime.push_back(tokens[tokens.size() - 1]);
        tokens.pop_back();
        maxQuantity.push_back(tokens[tokens.size() - 1]);
        tokens.pop_back();
        price.push_back(tokens[tokens.size() - 1]);
        tokens.pop_back();

        isExpired.push_back(false);

        std::string temp;
        for (int i=3; i<tokens.size(); i++) {
            temp.append(tokens[i]);
            temp+=' ';
        }
        stockName.push_back(temp);
        //std::cout<<"stock name is "<<stockName[stockName.size() - 1]<<"\r\n";

        
        for (int j=0; j<i; j++) {
            if (!isExpired[j] && expiryTime[j].compare("-1") != 0 && timeOfExpiry(expiryTime[j],timeStamp[j]) < std::stoi(timeStamp[i])) {
                // remove this shit

                isExpired[j] = true;
            }
        }
       // continue;
        int matchIndex = -1;
        for (int i=0; i<uniqueStocks.size(); i++) {
            //what if they're LCs?
            // some func comp karke which compares :)

            if (areSame(stockName[stockName.size() - 1], uniqueStocks[i])) {
                matchIndex = i;
            }
        }
        
        if (matchIndex == -1) {
            uniqueStocks.push_back(stockName[stockName.size() - 1]);
            //Also create a priority queue for this
            sellHeap s;
            uniqueSells.push_back(s);
            buyHeap b;
            uniqueBuys.push_back(b);

            if (buyOrSell[buyOrSell.size() - 1].compare("SELL") == 0) {
                entry t;
                t.price = convertPrice(price[price.size() - 1]);
                t.quantities = convertQuantity(maxQuantity[maxQuantity.size() - 1]);
                t.indexOfTrade = price.size() - 1;
                uniqueSells[uniqueSells.size() - 1].insert(t);
            }
            else {
                entry t;
                t.price = convertPrice(price[price.size() - 1]);
                t.quantities = convertQuantity(maxQuantity[maxQuantity.size() - 1]);
                t.indexOfTrade = price.size() - 1;
                uniqueBuys[uniqueBuys.size() - 1].insert(t);                
            }
            //continue;
        }
        else {
        
            if (buyOrSell[buyOrSell.size() - 1].compare("SELL") == 0) { 
                // push the price of this one in sell pq of match index
                entry t;
                t.price = convertPrice(price[price.size() - 1]);
                t.quantities = convertQuantity(maxQuantity[maxQuantity.size() - 1]);
                t.indexOfTrade = price.size() - 1;                
                uniqueSells[matchIndex].insert(t);
            }
            else {
                entry t;
                t.price = convertPrice(price[price.size() - 1]);
                t.quantities = convertQuantity(maxQuantity[maxQuantity.size() - 1]);
                t.indexOfTrade = price.size() - 1;
                uniqueBuys[matchIndex].insert(t);
                //push price of this one in buy pq
            }
            
            //continue;
            std::string interestingStock = uniqueStocks[matchIndex];
            entry *topSell = uniqueSells[matchIndex].returnMin();
            entry *topBuy = uniqueBuys[matchIndex].returnMax();
            bool isDedSell = true;
            bool isDedBuy = true;
            int sellIndex = buyOrSell.size() - 1;
            int buyIndex = buyOrSell.size() - 1;
            bool looper = true;
            
            // now check if either are expired, if so, delete them and try again
            while (looper) {
                //break;  
                isDedBuy = true;
                isDedSell = true;
                sellIndex = buyOrSell.size() - 1;
                buyIndex = buyOrSell.size() - 1;
                topSell = uniqueSells[matchIndex].returnMin();
                topBuy = uniqueBuys[matchIndex].returnMax();
                
                while (isDedSell) {
                    if (uniqueSells[matchIndex].isEmpty()) {
                        looper = false;
                        break;
                    }
                    //first obtain index j that matches with our conditions, buyOrSell[j] = SELL, stock = stock, price = topSell
                    //for (int j=0; j<sellIndex; j++) {
                        //if (buyOrSell[j].compare("SELL") == 0 && stockName[j].compare(interestingStock) == 0 && convertPrice(price[j]) == topSell.price) {
                        //    sellIndex = j;
                        //}
                    //}

                    sellIndex = topSell->indexOfTrade;

                    if (isExpired[sellIndex] && sellIndex != isExpired.size() - 1) {
                        topSell = uniqueSells[matchIndex].deleteMin();
                        topSell = uniqueSells[matchIndex].returnMin();
                        if (topSell->price == 999999) {
                            break;
                        }
                        isDedSell = true;
                    }
                    else {
                        isDedSell = false;
                    }
                }
            
                while (isDedBuy) {
                    if (uniqueBuys[matchIndex].isEmpty()) {
                        looper = false;
                        break;
                    }

                    buyIndex = topBuy->indexOfTrade;

                    if (isExpired[buyIndex]) {
                        topBuy = uniqueBuys[matchIndex].deleteMax();
                        topBuy = uniqueBuys[matchIndex].returnMax();
                        if (topBuy->price == -500) {
                            break;
                        }
                        isDedBuy = true;
                    }
                    else {
                        isDedBuy = false;
                    }
                }
                //break;
                if (!looper) {
                    break;
                }
                // if price in buy pq >= price in sell pq (there is a dude trying to buy for higher price than another dude trying to sell for)
                //then accept the trade
                //else reject
                if (topBuy->price >= topSell->price) {
                    //break;
                    //Check quantities. Do the sale. Eliminate one which is lesser in quantity.
                    bool isSellLower = false;
                    int quantity = topBuy->quantities;
                    if (quantity > topSell->quantities) {
                        quantity = topSell->quantities;
                        isSellLower = true;
                    }

                    std::string pps = price[buyIndex];
                    if (buyIndex > sellIndex) {
                        pps = price[sellIndex];
                    }

                    buyingComps.push_back(company[buyIndex]);
                    sellingComps.push_back(company[sellIndex]);
                    purchasingPrice.push_back(convertPrice(pps));
                    numSharesPurchased.push_back(quantity);

                    std::cout<<company[buyIndex]<<" purchased "<<quantity<<" share of "<<uniqueStocks[matchIndex]<<"from "<<company[sellIndex]<<" for "<<pps<<"/share\r\n";


                    if (isSellLower) {
                        uniqueSells[matchIndex].deleteMin();
                        topBuy->quantities -= quantity;
                    }
                    else {
                        uniqueBuys[matchIndex].deleteMax();
                        topSell->quantities -= quantity;
                        
                        if (topSell->quantities == 0) {
                            uniqueSells[matchIndex].deleteMin();
                        }
                    }
                    //break;
                }
                else {
                    //std::cout<<"buy heap is \r\n";
                    //printBuy(uniqueBuys[matchIndex]);
                    //std::cout<<"sell heap is \r\n";
                    //printSell(uniqueSells[matchIndex]);
                    looper = false;
                }
            }
        }
    }

    std::cout<<"\r\n";
    std::cout<<"---End of Day---\r\n";
    int totalMoneys = 0;
    int numShares = 0;
    for (int i=0; i<purchasingPrice.size(); i++) {
        totalMoneys += purchasingPrice[i]*numSharesPurchased[i];
        numShares += numSharesPurchased[i];
    }
    std::cout<<"Total Amount of Money Transferred: $"<<totalMoneys<<"\r\n";
    std::cout<<"Number of Completed Trades: "<<purchasingPrice.size()<<"\r\n";
    std::cout<<"Number of Shares Traded: "<<numShares<<"\r\n";

    //now for comps lol ye kaise karenge lmao

    //idea: traverse through buyingCompanies and pick up comps with same name, then just ooga booga
    //what if a company is only selling? fuck.

    //new idea: make a list of unique companies that participated in trading.... thats really all I can do lmao
    //but lets do this sexily. we'll do a map or some shizz (class bitch)

    std::vector<portfolio> listOfPortfolios;

    for (int i=0; i<buyingComps.size(); i++) {
        std::string sus = buyingComps[i];
        bool isInList = false;
        for (int j=0; j<listOfPortfolios.size(); j++) {
            if (listOfPortfolios[j].compare(sus)) {
                listOfPortfolios[j].numBuys += numSharesPurchased[i];
                listOfPortfolios[j].netTransfer -= numSharesPurchased[i]*purchasingPrice[i];
                isInList = true;
            }
        }
        if (!isInList) {
            portfolio t(sus);
            t.numBuys += numSharesPurchased[i];
            t.netTransfer -= numSharesPurchased[i]*purchasingPrice[i];
            listOfPortfolios.push_back(t);
        }
    }

    for (int i=0;  i<sellingComps.size(); i++) {
        std::string sus = sellingComps[i];
        bool isInList = false;
        for (int j=0; j<listOfPortfolios.size(); j++) {
            if (listOfPortfolios[j].compare(sus)) {
                listOfPortfolios[j].numSells += numSharesPurchased[i];
                listOfPortfolios[j].netTransfer += numSharesPurchased[i]*purchasingPrice[i];
                isInList = true;
            }
        }
        if (!isInList) {
            portfolio t(sus);
            t.numSells += numSharesPurchased[i];
            t.netTransfer += numSharesPurchased[i]*purchasingPrice[i];
            listOfPortfolios.push_back(t);
        
        }
    }

    std::vector<std::string> portfolioComps;
    for (int i=0; i<listOfPortfolios.size(); i++) {
        portfolioComps.push_back(listOfPortfolios[i].companyName);
    }

    for (int i=0; i<company.size(); i++) {  
        if (!isStringInVec(company[i],portfolioComps)) {
            std::cout<<company[i]<<" bought 0 and sold 0 for a net transfer of $0\r\n";
            portfolioComps.push_back(company[i]);
        } 
    }

    for (int i=0; i<listOfPortfolios.size(); i++) {
        std::cout<<listOfPortfolios[i].companyName<<" bought "<<listOfPortfolios[i].numBuys<<" and sold "<<listOfPortfolios[i].numSells<<" for a net transfer of $"<<listOfPortfolios[i].netTransfer<<"\r\n";
    }

}
