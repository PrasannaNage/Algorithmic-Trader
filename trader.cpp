#include "receiver.h"
#include <vector>
#include <iostream>

void printCombinations(std::vector<int>& arr, std::vector<int>& current, std::vector<std::vector<int>>& varyingWeights, int index, int n) {
    if (index == n) {
        for (int i = 0; i < n; i++) {
            //cout << current[i];
            if (i < n - 1) {
                //cout << ' ';
            }
        }
        //cout << '\n';

        varyingWeights.push_back(current);
        return;
    }

    for (int i = 1; i <= arr[index]; i++) {
        current[index] = i;
        printCombinations(arr, current, varyingWeights, index + 1, n);
    }
}

void vecMul (std::vector<std::string> &quants, int num) {
    for (int i=0; i<quants.size(); i++) {
        std::string t = quants[i];
        int u = std::stoi(t);
        u = u*num;
        t = std::to_string(u);
        quants[i] = t;
    }
}

bool vecStringComp (std::vector<std::string> a, std::vector<std::string> b) {
    //return false;
    if (a.size() == 0 || b.size() == 0) {
        return false;
    }
    if (a.size() != b.size()) {
        return false;
    }
    for (int i=0; i<a.size(); i++) {
        std::string t = a[i];
        bool bruh = false;
        for (int j=0; j<b.size(); j++) {
            if (t.compare(b[j]) == 0) {
                bruh = true;
            }
        }
        if (!bruh) {
            return false;
        }
    }
    return true;
}

void add (std::vector<std::string> &stocks1, std::vector<std::string> stocks2, std::vector<std::string> &quantities1, std::vector<std::string> quantities2) {
    for (int i=0; i<stocks2.size(); i++) {
        std::string analysis = stocks2[i];
        int sameIndex = -1;
        for (int j=0; j<stocks1.size(); j++) {
            if (analysis.compare(stocks1[j]) == 0) {
                sameIndex = j;
            }
        }
        if (sameIndex != -1) {
            int Q1 = std::stoi(quantities1[sameIndex]);
            int Q2 = std::stoi(quantities2[i]);
            std::string bruh = std::to_string(Q1 + Q2);
            quantities1[sameIndex] = bruh;
        }
        else {
            quantities1.push_back(quantities2[i]);
            stocks1.push_back(stocks2[i]);
        }
    }
}

bool check (std::vector<std::string> t) {
    std::string zero = "0";
    bool u = true;
    for (int i=0; i<t.size(); i++) {
        if (t[i].compare(zero) != 0) {
            u = false;
        }
    }
    return u;
}

void generateSubsets(std::vector<int>& set, std::vector<std::vector<int>> &allSubsets) {
    int n = set.size();

    // Number of subsets will be 2^n
    int totalSubsets = 1 << n;
    //std::vector<std::vector<int>> allSubsets;
    for (int i = 0; i < totalSubsets; ++i) {
        std::vector<int> currentSubset;

        // Check every bit of the current subset
        for (int j = 0; j < n; ++j) {
            // If the jth bit is set in i, add set[j] to the current subset
            if (i & (1 << j)) {
                currentSubset.push_back(set[j]);
            }
        }

        allSubsets.push_back(currentSubset);

        // Print the current subset
    }
}

bool isSubsetArb (std::vector<int> subset, std::vector<std::vector<std::string>> stocks, std::vector<std::vector<std::string>> quantities) {
    // add everything to whatever stock 1 is... and then check :P
    for (int i=1; i<subset.size(); i++) {
        add(stocks[subset[0]],stocks[subset[i]],quantities[subset[0]],quantities[subset[i]]);
    }
    return check(quantities[subset[0]]);
}

bool isInVec (std::vector<int> vec, int u) {
    bool v = false;
    for (int i=0; i<vec.size(); i++) {
        if (u == vec[i]) {
            v = true;
        }
    }
    return v;
}

void printVec (std::vector<int> vec) {
    for (int i=0; i<vec.size(); i++) {
        std::cout<<vec[i]<<" ";
    }
    std::cout<<"\r\n";
}

bool isArbitrage (std::vector<std::vector<std::string>> stocks, std::vector<std::vector<std::string>> quantities, std::vector<bool> &indicesOfArbitrage, int calleeIndex, std::vector<std::vector<bool>> &multiArb) {
    // do stuff
    
    int n = stocks.size();
    std::vector<int> sets;
    for (int i=0; i<n; i++) {
        sets.push_back(i);
    }
    std::vector<std::vector<int>> allSubsets;
    generateSubsets(sets,allSubsets);

    for (int i=allSubsets.size()/2 + 1; i<allSubsets.size(); i++) { //we dont want to visit subsets not having current one
        std::vector<int> currSubset = allSubsets[i];
        //printVec(currSubset);
        //std::cout<<"Printing currSubset\r\n";
        //printVec(currSubset);

        // prepare indices of arbitrage
        std::vector<bool> bruh;
        for (int j=0; j<n; j++) {
            bruh.push_back(false);
        }
        for (int j=0; j<n; j++) {
            if (isInVec(currSubset,j)) {
                bruh[j] = true;
            }
        }

        if (isSubsetArb(currSubset,stocks,quantities)) {
            multiArb.push_back(bruh);
        }
    }
    
    if (multiArb.size() != 0) {
        return true;
    }
    return false;
}

bool isItUnique (std::string stockBhai, std::vector<std::string> uniqueStocks) {
    bool u = false;
    for (int i=0; i<uniqueStocks.size(); i++) {
        if (stockBhai.compare(uniqueStocks[i]) == 0) {
            u = true;
        }
    }
    if (u) {
        return false;
    }
    return true;
}

int findMatchIndex (std::string stockBhai, std::vector<std::string> uniqueStocks) {
    for (int i=0; i<uniqueStocks.size(); i++) {
        if (stockBhai.compare(uniqueStocks[i]) == 0) {
            return i;
        }
    }
    return -1; //this will never be returned
}

void mySwap (int &a, int &b) {
    a = a+b;
    b = a-b;
    a = a-b;
}

class buyHeap { // keep max price here

    public:
        std::vector<int> arr;
        bool isEmpty() {
            return arr.empty();
        }

        int returnMax () {
            if (arr.empty()) {
                return -100;
            }
            return arr[0];
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

        void insert (int t) {
            arr.push_back(t);
            int i = arr.size() - 1;
            while (i > 0 && arr[i] > arr[parent(i)]) {
                mySwap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (arr[left(i)] > arr[i]) {//if left(i) > i
                    mySwap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (arr[right(i)] > arr[left(i)]) {
                    index = right(i);
                }
                if (arr[index] > arr[i]) {
                    mySwap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        int deleteMax () {
            mySwap(arr[0],arr[arr.size() - 1]);
            int t = arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }

        void deleteArb (int index) {
            if (index >= arr.size()) {
                return;
            }
            mySwap(arr[index],arr[arr.size() - 1]);
            arr.pop_back();
            heapify(index);
        }

        buyHeap() {
            std::vector<int> t;
            arr = t;
        }
};

class sellHeap {
    public:
        std::vector<int> arr;

        bool isEmpty () {
            return arr.empty();
        }

        int returnMin () {
            if (arr.empty()) {
                return 5000;
            }
            return arr[0];
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

        void insert (int t) {
            arr.push_back(t);
            int i = arr.size() - 1;
            while (i > 0 && arr[i] < arr[parent(i)]) {
                mySwap(arr[i],arr[parent(i)]);
                i = parent(i);
            }
        }

        void heapify(int i) {
            if (left(i) >= arr.size() || i >= arr.size()) {
                return;
            }
            if (right(i) >= arr.size()) {
                if (arr[left(i)] < arr[i]) {//if left(i) < i
                    mySwap(arr[left(i)],arr[i]);
                    return;
                }
                return; 
            }
            else {
                int index = left(i);
                if (arr[right(i)] < arr[left(i)]) {
                    index = right(i);
                }
                if (arr[index] < arr[i]) {
                    mySwap(arr[i],arr[index]);
                    heapify(index);
                }
                else {
                    return;
                }
            }
        }

        int deleteMin () {
            mySwap(arr[0],arr[arr.size() - 1]);
            int t = arr[arr.size() - 1];
            arr.pop_back();
            heapify(0);
            return t;
        }

        void deleteArb (int index) {
            if (index >= arr.size()) {
                return;
            }
            mySwap(arr[index],arr[arr.size() - 1]);
            arr.pop_back();
            heapify(index);
        }

        sellHeap() {
            std::vector<int> t;
            arr = t;
        }
};

int correspondingIndex (int tradeVal, buyHeap activeBuys) { //returns -1 if no correspondance
    std::vector<int> checker = activeBuys.arr;
    for (int i=0; i<checker.size(); i++) {
        if (tradeVal == checker[i]) {
            return i;
        }
    }
    return -1;
}
int correspondingIndexForSell (int tradeVal, sellHeap activeBuys) { //returns -1 if no correspondance
    std::vector<int> checker = activeBuys.arr;
    for (int i=0; i<checker.size(); i++) {
        if (tradeVal == checker[i]) {
            return i;
        }
    }
    return -1;
}

bool newVecStringComp (std::vector<std::string> a, std::vector<std::string> b) {
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
 
void partOne() {

    Receiver rcv;
    bool foundDollar = false;
    std::vector<std::string> words;
    std::string sell = "s#";
    std::string buy = "b#";
    std::string altBuy = "b#$";
    std::string altSell = "s#$";
    std::string bruh;
    while (!foundDollar) {

        std::string tmp = rcv.readIML();
        bruh.append(tmp);
        std::string message = tmp;
        if (tmp.find('$') != std::string::npos) {
            foundDollar = true;
        }
    }

    for (int i=0; i<bruh.length(); i++) {
        if (bruh[i] == '\r' || bruh[i] == '\n') {
            bruh[i] = '*';
        }
    }
        
    std::istringstream iss(bruh);
    std::string word;

    while (std::getline(iss,word,'*')) {
        words.push_back(word);
    }

    std::vector<buyHeap> activeBuys; //activeBuys[i][j] denotes the jth price for buying ith stock
    std::vector<sellHeap> activeSells;//activeSells[i][j] denotes jth price for selling ith stock
    std::vector<std::string> uniqueStocks;
    std::vector<int> worth;
    std::vector<std::string> stocks;
    std::vector<std::string> money;
    std::vector<std::string> buyOrSell;

    for (int i=0; i<words.size(); i++) {
        size_t firstSpace = words[i].find(' ');
        size_t secondSpace = words[i].find(' ',firstSpace + 1);

        std::string part1 = words[i].substr(0, firstSpace);
        std::string part2 = words[i].substr(firstSpace + 1, secondSpace - firstSpace - 1);
        std::string part3 = words[i].substr(secondSpace + 1);

        stocks.push_back(part1);
        money.push_back(part2);
        buyOrSell.push_back(part3);

        if (isItUnique(stocks[stocks.size() - 1], uniqueStocks)) {
            // do shit, no cancellation here :) just directly take stock lol
            std::cout<<stocks[stocks.size() - 1]<<" "<<money[money.size() - 1]<<" ";
            if (buyOrSell[buyOrSell.size() - 1].compare(sell) == 0 || buyOrSell[buyOrSell.size() - 1].compare(altSell) == 0) {
                std::cout<<"b\r\n";
            }
            else {
                std::cout<<"s\r\n";
            }
            buyHeap yeStockWala;
            sellHeap bruh;
            activeBuys.push_back(yeStockWala);
            activeSells.push_back(bruh);
            uniqueStocks.push_back(stocks[stocks.size() - 1]);
            worth.push_back(std::stoi(money[money.size() - 1]));
        }
        else {
            // theres a matching index, lets find it :)
            int index = findMatchIndex(stocks[stocks.size() - 1], uniqueStocks);

            // implement cancellation
            bool isSell;
            if (buyOrSell[buyOrSell.size() - 1].compare(sell) == 0 || buyOrSell[buyOrSell.size() - 1].compare(altSell) == 0) {
                isSell = true;
            }
            else {
                isSell = false;
            }            
            if (isSell) {
                //cancellation for sell
                int tradeVal = std::stoi(money[money.size() - 1]);
                if (tradeVal > activeSells[index].returnMin()) {
                    std::cout<<"No Trade\r\n";
                    continue;
                }
                //sellHeap couldnt cancel it.. check if there is a corresponding buy
                int buyInd = correspondingIndex(tradeVal,activeBuys[index]);
                if (buyInd != -1) {
                    //cancellation.. remove the buy order and output no trade
                    std::cout<<"No Trade\r\n";
                    activeBuys[index].deleteArb(buyInd);
                    continue;
                }
                int sellInd = correspondingIndexForSell(tradeVal,activeSells[index]);
                if (sellInd != -1) {
                    std::cout<<"No Trade\r\n";
                    continue;
                }
                else {
                    //do stuff, if it is below our current worth, we will accept it and worth is updated.
                    if (tradeVal < worth[index]) {
                        std::cout<<stocks[stocks.size() - 1]<<" "<<money[money.size() - 1]<<" b\r\n";
                        worth[index] = tradeVal;
                        activeSells[index].insert(tradeVal);
                        if (activeSells[index].arr.size() > 1) {
                            activeSells[index].arr.pop_back();
                        }
                        continue;
                    }
                    else {
                        // insert it in the sellHeap
                        std::cout<<"No Trade\r\n";
                        activeSells[index].insert(tradeVal);
                        if (activeSells[index].arr.size() > 1) {
                            activeSells[index].arr.pop_back();
                        }
                        continue;
                    }
                }
            }
            else {
                //cancellation for buy
                int tradeVal = std::stoi(money[money.size() - 1]);
                if (tradeVal < activeBuys[index].returnMax()) {
                    std::cout<<"No Trade\r\n";
                    continue;
                }
                //sellHeap couldnt cancel it.. check if there is a corresponding buy
                int buyInd = correspondingIndexForSell(tradeVal,activeSells[index]);
                if (buyInd != -1) {
                    //cancellation.. remove the buy order and output no trade
                    std::cout<<"No Trade\r\n";
                    activeSells[index].deleteArb(buyInd);
                    continue;
                }
                int sellInd = correspondingIndex(tradeVal,activeBuys[index]);
                if (sellInd != -1) {
                    std::cout<<"No Trade\r\n";
                    continue;
                }
                else {
                    //do stuff
                    if (tradeVal > worth[index]) {
                        std::cout<<stocks[stocks.size() - 1]<<" "<<money[money.size() - 1]<<" s\r\n";
                        worth[index] = tradeVal;
                        activeBuys[index].insert(tradeVal);
                        if (activeBuys[index].arr.size() > 1) {
                            activeBuys[index].arr.pop_back();
                        }
                    }
                    else {
                        std::cout<<"No Trade\r\n";
                        activeBuys[index].insert(tradeVal);
                        if (activeBuys[index].arr.size() > 1) {
                            activeBuys[index].arr.pop_back();
                        }
                        continue;
                    }
                }
            }
        }

    }

    rcv.terminate();
}

void partTwo () {
    Receiver rcv;
    bool foundDollar = false;
    std::vector<std::vector<std::string>> stocks;
    std::vector<std::vector<std::string>> quantities;
    std::vector<std::string> moneys;
    std::vector<std::string> buyOrSell;
    std::vector<int> numStocks;

    std::vector<std::vector<std::string>> uniqueLinComb;
    std::vector<int> activeBuys;
    std::vector<int> activeSells;
    std::vector<std::vector<std::string>> iterLinComb;
    std::vector<int> associatedBuys;
    std::vector<int> associatedSells;

    std::string sell = "s#";
    std::string buy = "b#";
    std::string altSell = "s#$";
    std::string altBuy = "b#$";
    std::vector<std::string> words;
    std::string bruh;
    int profit = 0;
    while (!foundDollar) {
        std::string message = rcv.readIML();
        bruh.append(message);
        if (message.find('$') != std::string::npos) {
            foundDollar = true;
        }
    }

    for (int i=0; i<bruh.length(); i++) {
        if (bruh[i] == '\r' || bruh[i] == '\n') {
            bruh[i] = '*';
        }
    }        
    std::istringstream iss(bruh);
    std::string word;
    
    while (std::getline(iss,word,'*')) {
        words.push_back(word);
    }
    
    //for (int i=0; i<words.size(); i++) {
      //  std::cout<<words[i]<<"\r\n";
    //}
    //return;
    int n = words.size();
    bool wasAccepted[n] = {false};
    for (int i=0; i<words.size(); i++) {
        //Lets tokenize ? 
        std::istringstream iss(words[i]);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        bool isSell = false;

        buyOrSell.push_back(tokens[tokens.size() - 1]);
        if (buyOrSell[buyOrSell.size() - 1].compare(sell) == 0 || buyOrSell[buyOrSell.size() - 1].compare(altSell) == 0) {
            isSell = true;
        }

        tokens.pop_back();
        if (isSell) {
            int t = std::stoi(tokens[tokens.size() - 1]);
            t = -t;
            moneys.push_back(std::to_string(t));
        }
        else {
            moneys.push_back(tokens[tokens.size() - 1]);
        }
        tokens.pop_back();

        int n = tokens.size();
        numStocks.push_back(n/2);

        std::vector<std::string> tmp;
        for (int i=0; i<n/2; i++) {
            std::string hi = "hi";
            tmp.push_back(hi);
        }

        stocks.push_back(tmp);
        quantities.push_back(tmp);
        n = stocks.size() - 1;
        
        std::vector<std::string> linCombChecker;

        for (int j=0; j<tokens.size(); j++) {
            linCombChecker.push_back(tokens[j]);
            if (j%2 == 0) {
                stocks[n][j/2] = tokens[j];
            }
            else {
                if (isSell) {
                    int t = std::stoi(tokens[j]);
                    t = -t;
                    quantities[n][j/2] = std::to_string(t);
                }
                else {
                    quantities[n][j/2] = tokens[j];
                }
            }
        }
        
        //if (vecStringComp(stocks[stocks.size() - 1],stocks[stocks.size() - 2]) && vecStringComp(quantities[quantities.size() - 1],quantities[quantities.size() - 2]) && moneys[moneys.size() - 1].compare(moneys[moneys.size() - 2]) == 0) {
          //  stocks.pop_back();
           // quantities.pop_back();
           // moneys.pop_back();
           // buyOrSell.pop_back();
           // continue;
        //}
        iterLinComb.push_back(linCombChecker);
        int index = -1;

        for (int j=0; j<uniqueLinComb.size(); j++) {
            if (newVecStringComp(uniqueLinComb[j],linCombChecker)) {
                index = j;
            }
        }

        if (index != -1) {
            // if its unique, no cancellation, here it's not unique there MAY be cancellation :)
            if (isSell) {
                //cancellation for sell
                if (activeSells[index] >= std::stoi(moneys[moneys.size() - 1])) {
                    std::cout<<"No Trade\r\n";

                    //cancel this trade lol
                    while (stocks[stocks.size() - 1].size() != 0) {
                        stocks[stocks.size() - 1].pop_back();
                    }
                    while (quantities[quantities.size() - 1].size() != 0){
                        quantities[quantities.size() - 1].pop_back();
                    }
                    moneys[moneys.size() - 1] = "0";       
                    
                    continue;
                }
                else  if (activeSells[index] < std::stoi(moneys[moneys.size() - 1])) {
                    activeSells[index] = std::stoi(moneys[moneys.size() - 1]);
                
                    //cancel the associated sell
                    int iter = associatedSells[index];
                    associatedSells[index] = stocks.size() - 1;
                    if (iter != -1) {
                        while (stocks[iter].size() != 0) {
                            stocks[iter].pop_back();
                        }
                        while (quantities[iter].size() != 0) {
                            quantities[iter].pop_back();
                        }
                        moneys[iter] = "0";
                    }

                }
                //std::cout<<activeBuys[index]<<" "<<moneys[moneys.size() - 1]<<" bruh\r\n";
                if (-activeBuys[index] == std::stoi(moneys[moneys.size() - 1])) {
                    std::cout<<"No Trade\r\n";
                    activeBuys[index] = -9999;
                
                    //cancel associated buy and this order!
                    while (stocks[stocks.size() - 1].size() != 0) {
                        stocks[stocks.size() -1].pop_back();
                    }
                    while (quantities[quantities.size() - 1].size() != 0) {
                        quantities[quantities.size() - 1].pop_back();
                    }
                    moneys[moneys.size() - 1] = "0";
                    int iter = associatedBuys[index];
                    associatedBuys[index] = -1;
                    if (iter != -1) {
                        while (stocks[iter].size() != 0) {
                            stocks[iter].pop_back();
                        }
                        while (quantities[iter].size() != 0) {
                            quantities[iter].pop_back();
                        }
                        moneys[iter] = "0";
                    }
                    continue;
                }
            }
            else {
                //cancellation for buy
                if (activeBuys[index] >= std::stoi(moneys[moneys.size() - 1])) {
                    std::cout<<"No Trade\r\n";
                    
                    //cancel this buy
                    while (stocks[stocks.size() - 1].size() != 0) {
                        stocks[stocks.size() - 1].pop_back();
                    }
                    while (quantities[quantities.size() - 1].size() != 0){
                        quantities[quantities.size() - 1].pop_back();
                    }
                    moneys[moneys.size() - 1] = "0";                    
                    
                    continue;
                }
                else  if (activeBuys[index] < std::stoi(moneys[moneys.size() - 1])) {
                    activeBuys[index] = std::stoi(moneys[moneys.size() - 1]);

                    //cancel associated buy
                    int iter = associatedBuys[index];
                    associatedBuys[index] = stocks.size() - 1;
                    if (iter != -1) {
                        while (stocks[iter].size() != 0) {
                            stocks[iter].pop_back();
                        }
                        while (quantities[iter].size() != 0) {
                            quantities[iter].pop_back();
                        }
                        moneys[iter] = "0";
                    }                
                }
                //std::cout<<activeSells[index]<<" bruh\r\n";
                if (-activeSells[index] == std::stoi(moneys[moneys.size() - 1])) {
                    std::cout<<"No Trade\r\n";
                    activeSells[index] = -9999;
                    
                    //cancel associated sell
                    while (stocks[stocks.size() - 1].size() != 0) {
                        stocks[stocks.size() -1].pop_back();
                    }
                    while (quantities[quantities.size() - 1].size() != 0) {
                        quantities[quantities.size() - 1].pop_back();
                    }
                    moneys[moneys.size() - 1] = "0";                    
                    int iter = associatedSells[index];
                    associatedSells[index] = -1;
                    if (iter != -1) {
                        while (stocks[iter].size() != 0) {
                            stocks[iter].pop_back();
                        }
                        while (quantities[iter].size() != 0) {
                            quantities[iter].pop_back();
                        }
                        moneys[iter] = "0";
                    }
                    continue;
                }                
            }
        }
        else {
            // if its unique, we need to push it into uniqueLinComb :)
            uniqueLinComb.push_back(linCombChecker);
            if (isSell) {
                activeSells.push_back(std::stoi(moneys[moneys.size() - 1]));
                associatedSells.push_back(stocks.size() - 1);//associatedSells has this one as index
                activeBuys.push_back(-9999);
                associatedBuys.push_back(-1); //no associatedBuy yet
            }
            else {
                activeBuys.push_back(std::stoi(moneys[moneys.size() - 1]));
                associatedBuys.push_back(stocks.size() - 1);
                activeSells.push_back(-99999);
                associatedSells.push_back(-1);
            }
        }

        std::vector<bool> gay;
    
        for (int i=0; i<= n; i++) {
            bool t = false;
            gay.push_back(t);
        }
        gay[n] = true; 

        std::vector<std::vector<bool>> multiArb;

        //isArbitrage(stocks,quantities,gay,0);
        //std::cout<<"Printing gay[i]\r\n";           
        if (isArbitrage(stocks,quantities,gay,0,multiArb)) {
            int sum = 0;
            int maxSum = 0;
            for (int i=0; i<multiArb.size(); i++) {
                sum = 0;
                for (int j=0; j<multiArb[i].size(); j++) {
                    if (multiArb[i][j]) {
                        sum += std::stoi(moneys[j]);
                    }
                }
                if (sum > maxSum) {
                    maxSum = sum;
                    gay = multiArb[i];
                }
            }

            //for (int i=0; i<gay.size(); i++) {
                //if (gay[i]) {
                //    sum += std::stoi(moneys[i]);
              //  }
            //}

            sum = maxSum;

            if (sum > 0) {
                for (int i=gay.size() - 1; i>=0; i--) {
                    if (gay[i]) {
                         // Print this shit and now take the orders off the market
                        std::vector<std::string> yeWalaLinComb = iterLinComb[i];
                        if (buyOrSell[i].compare(sell) == 0 || buyOrSell[i].compare(altSell) == 0) {
                            for (int j=0; j<stocks[i].size(); j++) {
                                std::cout<<stocks[i][j]<<" ";

                                int t = std::stoi(quantities[i][j]);
                                t = -t;
                                std::cout<<t<<" ";
                            }
                            int t = std::stoi(moneys[i]);
                            t = -t;
                            std::cout<<t<<" b#\r\n";
                        }
                        else {
                            for (int j=0; j<stocks[i].size(); j++) {
                                
                                std::cout<<stocks[i][j]<<" "<<quantities[i][j]<<" ";
                            }
                            std::cout<<moneys[i]<<" "<<"s#\r\n";
                        }

                        //reset activeBuy and activeSell
                        int index = -1;
                        for (int m=0; m<uniqueLinComb.size(); m++) {
                            if (newVecStringComp(uniqueLinComb[m],yeWalaLinComb)) {
                                index = m;
                                //std::cout<<"i is "<<i<<" and index being matches is "<<index<<"\r\n";
                            }
                        }

                        if (index != -1) {
                            if (buyOrSell[i].compare(sell) == 0 || buyOrSell[i].compare(altSell) == 0) {
                                activeSells[index] = -9999;
                                associatedSells[index] = -1;
                            }
                            else {
                                activeBuys[index] = -9999;
                                associatedBuys[index] = -1;
                            }
                        }

                        while (stocks[i].size() != 0) {
                            stocks[i].pop_back();
                        }
                        while (quantities[i].size() != 0){
                            quantities[i].pop_back();
                        }
                        moneys[i] = "0";
                    }
                }
                profit += sum;                  
            }
            else {
                std::cout<<"No Trade\r\n";
            }
        }
        else {
            std::cout<<"No Trade\r\n";
        }
    } 
    std::cout<<profit<<"\r\n";
    rcv.terminate();
}

bool isValid (std::vector<bool> bruh) {
    bool u = false;
    for (int i=0; i<bruh.size() - 1; i++) {
        if (bruh[i]) {
            u = true;
        }
    }
    return u;
}

void partThree(){
    bool foundDollar = false;
    std::vector<std::vector<std::string>> stocks;
    std::vector<std::vector<std::string>> quantities;
    std::vector<std::string> moneys;
    std::vector<std::string> buyOrSell;
    std::vector<std::string> orderQuantities;

    std::vector<std::vector<std::string>> uniqueLinComb;
    std::vector<std::vector<int>> activeBuys;
    std::vector<std::vector<int>> associatedBuys;
    std::vector<std::vector<int>> activeSells;
    std::vector<std::vector<int>> associatedSells;
    std::vector<std::vector<std::string>> iterLinComb;

    std::string sell = "s#";
    std::string buy = "b#";
    std::string altSell = "s#$";
    std::string altBuy = "b#$";
    std::vector<std::string> words;
    std::string bruh;   
    Receiver rcv; 
    while (!foundDollar) {
        std::string message = rcv.readIML();
        bruh.append(message);
        if (message.find('$') != std::string::npos) {
            foundDollar = true;
        }
    }
    for (int i=0; i<bruh.length(); i++) {
        if (bruh[i] == '\r' || bruh[i] == '\n') {
            bruh[i] = '*';
        }
    }        
    std::istringstream iss(bruh);
    std::string word;
    //std::vector<std::string> words;
    while (std::getline(iss,word,'*')) {
        words.push_back(word);
    }


    int profit = 0;
    for (int i=0; i<words.size(); i++) {
            //Lets tokenize ? 
        std::istringstream iss(words[i]);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        bool isSell = false;

        buyOrSell.push_back(tokens[tokens.size() - 1]);
        if (buyOrSell[buyOrSell.size() - 1].compare(sell) == 0 || buyOrSell[buyOrSell.size() - 1].compare(altSell) == 0) {
            isSell = true;
        }
        
        tokens.pop_back();
        orderQuantities.push_back(tokens[tokens.size() - 1]);
        tokens.pop_back();

        if (isSell) {
            int t = std::stoi(tokens[tokens.size() - 1]);
            t = -t;
            moneys.push_back(std::to_string(t));
        }
        else {
            moneys.push_back(tokens[tokens.size() - 1]);
        }
        tokens.pop_back();

        int n = tokens.size();
        //numStocks.push_back(n/2);

        std::vector<std::string> tmp;
        for (int i=0; i<n/2; i++) {
            std::string hi = "hi";
            tmp.push_back(hi);
        }

        std::vector<std::string> linCombChecker;
        stocks.push_back(tmp);
        quantities.push_back(tmp);
        n = stocks.size() - 1;
            
        for (int j=0; j<tokens.size(); j++) {
            linCombChecker.push_back(tokens[j]);
            if (j%2 == 0) {
                stocks[n][j/2] = tokens[j];
            }
            else {
                if (isSell) {
                    int t = std::stoi(tokens[j]);
                    t = -t;
                    quantities[n][j/2] = std::to_string(t);
                }
                else {
                    quantities[n][j/2] = tokens[j];
                }
            }
        }
        
        iterLinComb.push_back(linCombChecker);
        // cancellation time, only one thing here though :)
        
        int ind = -1;
        for (int j=0; j<uniqueLinComb.size(); j++) {
            if (newVecStringComp(linCombChecker,uniqueLinComb[j])) {
                ind = j;
            }
        }
        if (ind != -1) {
            bool isCurrCancelled = false;
            if (isSell) {
                //check across a buyVector for matchign stuff, if found delete both
                for (int j=0; j<activeBuys[ind].size(); j++) {
                    if (-activeBuys[ind][j] == std::stoi(moneys[moneys.size() - 1])) {
                        //remove this one and associated one
                        int iter = associatedBuys[ind][j];
                        //compare order quantities... remove lower one...shit.
                        int currStockQuant = std::stoi(orderQuantities[orderQuantities.size() - 1]);
                        int otherStockQuant = std::stoi(orderQuantities[iter]);

                        if (currStockQuant < otherStockQuant) {
                            //remove currStock
                            std::cout<<"No Trade\r\n";
                            while (stocks[stocks.size() - 1].size() != 0) {
                                stocks[stocks.size() -1].pop_back();
                            }
                            while (quantities[quantities.size() - 1].size() != 0) {
                                quantities[quantities.size() -  1].pop_back();
                            }
                            moneys[moneys.size() - 1] = "0";

                            //update otherStock quantity :(
                            orderQuantities[orderQuantities.size() - 1] = "1";
                            int t = otherStockQuant - currStockQuant;
                            orderQuantities[iter] = std::to_string(t);
                            isCurrCancelled = true;
                        }
                        else if (currStockQuant == otherStockQuant) {
                            //remove both
                            std::cout<<"No Trade\r\n";
                            while (stocks[stocks.size() - 1].size() != 0) {
                                stocks[stocks.size() -1].pop_back();
                            }
                            while (quantities[quantities.size() - 1].size() != 0) {
                                quantities[quantities.size() -  1].pop_back();
                            }
                            moneys[moneys.size() - 1] = "0";                        

                            orderQuantities[orderQuantities.size() - 1] = "1";
                            orderQuantities[iter] = "1";

                            while (stocks[iter].size() != 0) {
                                stocks[iter].pop_back();
                            }
                            while (quantities[iter].size() != 0) {
                                quantities[iter].pop_back();
                            }
                            moneys[iter] = "0";

                            activeBuys[ind][j] = -9999;
                            isCurrCancelled = true;                        
                        }
                        else {
                            //remove other stock
                            while (stocks[iter].size() != 0) {
                                stocks[iter].pop_back();
                            }
                            while (quantities[iter].size() != 0) {
                                quantities[iter].pop_back();
                            }
                            moneys[iter] = "0";

                            orderQuantities[iter] = "1";
                            int t = currStockQuant - otherStockQuant;
                            orderQuantities[orderQuantities.size() - 1] = std::to_string(t);
                            activeBuys[ind][j] = -9999;                             
                        }
                    }
                }
            }
            else {
                //check across a sellVector for matching stuff, if found delete both
                for (int j=0; j<activeSells[ind].size(); j++) {
                    if (-activeSells[ind][j] == std::stoi(moneys[moneys.size() - 1])) {
                        //remove this one and associated one
                        int iter = associatedSells[ind][j];
                        //compare order quantities... remove lower one...shit.
                        int currStockQuant = std::stoi(orderQuantities[orderQuantities.size() - 1]);
                        int otherStockQuant = std::stoi(orderQuantities[iter]);

                        if (currStockQuant < otherStockQuant) {
                            //remove currStock
                            std::cout<<"No Trade\r\n";
                            while (stocks[stocks.size() - 1].size() != 0) {
                                stocks[stocks.size() -1].pop_back();
                            }
                            while (quantities[quantities.size() - 1].size() != 0) {
                                quantities[quantities.size() -  1].pop_back();
                            }
                            moneys[moneys.size() - 1] = "0";

                            //update otherStock quantity :(
                            orderQuantities[orderQuantities.size() - 1] = "1";
                            int t = otherStockQuant - currStockQuant;
                            orderQuantities[iter] = std::to_string(t);
                            isCurrCancelled = true;
                        }
                        else if (currStockQuant == otherStockQuant) {
                            //remove both
                            std::cout<<"No Trade\r\n";
                            while (stocks[stocks.size() - 1].size() != 0) {
                                stocks[stocks.size() -1].pop_back();
                            }
                            while (quantities[quantities.size() - 1].size() != 0) {
                                quantities[quantities.size() -  1].pop_back();
                            }
                            moneys[moneys.size() - 1] = "0";                        

                            orderQuantities[orderQuantities.size() - 1] = "1";
                            orderQuantities[iter] = "1";

                            while (stocks[iter].size() != 0) {
                                stocks[iter].pop_back();
                            }
                            while (quantities[iter].size() != 0) {
                                quantities[iter].pop_back();
                            }
                            moneys[iter] = "0";

                            activeSells[ind][j] = -9999;
                            isCurrCancelled = true;                        
                        }
                        else {
                            //remove other stock
                            while (stocks[iter].size() != 0) {
                                stocks[iter].pop_back();
                            }
                            while (quantities[iter].size() != 0) {
                                quantities[iter].pop_back();
                            }
                            moneys[iter] = "0";

                            orderQuantities[iter] = "1";
                            int t = currStockQuant - otherStockQuant;
                            orderQuantities[orderQuantities.size() - 1] = std::to_string(t);
                            activeSells[ind][j] = -9999;                             
                        }
                    }
                }            
            }

            if (isCurrCancelled) {
                continue;
            }

            //its valid, lets put it in activeBuy/activeSell/associatedBuy/associatedSell
            if (isSell) {
                //put it in associatedSells and activeSells
                int t = std::stoi(moneys[moneys.size() - 1]);
                associatedSells[ind].push_back(stocks.size() - 1);
                activeSells[ind].push_back(t);
                iterLinComb.push_back(linCombChecker);
            }
            else {
                //put it in associatedBuys and activeBuys
                int t = std::stoi(moneys[moneys.size() - 1]);
                associatedBuys[ind].push_back(stocks.size() - 1);
                activeBuys[ind].push_back(t);
                iterLinComb.push_back(linCombChecker);
            }
        }
        else {
            uniqueLinComb.push_back(linCombChecker);
            std::vector<int> placeHolder;
            activeBuys.push_back(placeHolder);
            activeSells.push_back(placeHolder);
            associatedBuys.push_back(placeHolder);
            associatedSells.push_back(placeHolder);
            iterLinComb.push_back(linCombChecker);
            if (isSell) {
                //put ur shit in activeSells and associatedSells
                int t = std::stoi(moneys[moneys.size() - 1]);
                associatedSells[associatedSells.size() - 1].push_back(stocks.size() - 1);
                activeSells[activeSells.size() - 1].push_back(t);
            }
            else {
                int t = std::stoi(moneys[moneys.size() - 1]);
                associatedBuys[associatedBuys.size() - 1].push_back(stocks.size() - 1);
                activeBuys[activeBuys.size() - 1].push_back(t);
            }

        }
        // input done
        std::vector<int> current(orderQuantities.size());
        std::vector<std::vector<int>> varyingWeights;
        std::vector<int> arr;
        for (int i=0; i<orderQuantities.size(); i++) {
            int t = std::stoi(orderQuantities[i]);
            arr.push_back(t);
        }
        int zer = 0;
        std::vector<std::vector<bool>> whereArbitrage;
        
        printCombinations(arr,current,varyingWeights,0,arr.size());
        for (int i=0; i<varyingWeights.size(); i++) {

            std::vector<int> weights = varyingWeights[i];
            std::vector<bool> gay;
            whereArbitrage.push_back(gay);
            for (int i=0; i<quantities.size(); i++) {
                gay.push_back(false);
            }
            gay[quantities.size() - 1] = true;
            std::vector<std::vector<std::string>> storage = quantities; //at the end of this, restore.
            
            for (int j=0; j<varyingWeights[i].size(); j++) {
                vecMul(quantities[j],varyingWeights[i][j]);
            }
            std::vector<std::vector<bool>> multiArb;
            if (isArbitrage(stocks,quantities,gay,0,multiArb)) {
                //std::cout<<"Printing multiarb\r\n";
                /*
                for (int j=0; j<multiArb.size(); j++) {
                    for (int k=0; k<multiArb[j].size(); k++) {
                        if (multiArb[j][k]) {
                            std::cout<<"1 ";
                        }
                        else {
                            std::cout<<"0 ";
                        }
                    }
                    std::cout<<"\r\n";
                }
                */
                // set gay :)
                int sum = 0;
                int maxSum = 0;
                for (int j=0; j<multiArb.size(); j++) {
                    sum = 0;
                    int numOnes = 0;
                    for (int k=0; k<multiArb[j].size(); k++) {
                        if (multiArb[j][k]) {
                            numOnes++;
                        }
                    }
                    if (numOnes == 1) {
                        break;
                    }
                    for (int k=0; k<multiArb[j].size(); k++) {
                        
                        if (multiArb[j][k]) {
                            int money = std::stoi(moneys[k]);
                            int quant = varyingWeights[i][k];
                            sum += money*quant;
                        }
                    }
                    if (sum > maxSum) {
                        maxSum = sum;
                        gay = multiArb[j];
                    }
                }
                //
                whereArbitrage[whereArbitrage.size() - 1] = gay;
            }

            quantities = storage;
        }
        int maxSum = 0;
        int maxIndex = -1;
        for (int i=0; i<whereArbitrage.size(); i++) {
            if (whereArbitrage[i].size() != 0 && isValid(whereArbitrage[i])) {
                int sum = 0;
                for (int j=0; j<whereArbitrage[i].size(); j++) {
                    if (whereArbitrage[i][j]) {
                        int money = std::stoi(moneys[j]);
                        int quant = varyingWeights[i][j];
                        sum += money*quant;
                    }
                }
                if (maxSum < sum) {
                    maxSum = sum;
                    maxIndex = i;
                }
            }
        }
        if (maxIndex == -1) {
            std::cout<<"No Trade\r\n";
        }
        else {
            profit += maxSum;
            for (int j=whereArbitrage[maxIndex].size() - 1; j>=0; j--) {
                if (whereArbitrage[maxIndex][j]) {
                    bool isSell = false;
                    if (buyOrSell[j].compare(sell) == 0 || buyOrSell[j].compare(altSell) == 0) {
                        for (int k=0; k<stocks[j].size(); k++) {
                            std::cout<<stocks[j][k]<<" ";
                            int t = std::stoi(quantities[j][k]);
                            t = -t;
                            std::cout<<t<<" ";
                        }
                        int t = std::stoi(moneys[j]);
                        t = -t;
                        std::cout<<t<<" ";
                        std::cout<<varyingWeights[maxIndex][j]<<" ";
                        std::cout<<"b#\r\n";
                        isSell = true;
                    }
                    else {
                        for (int k=0; k<stocks[j].size(); k++) {
                            std::cout<<stocks[j][k]<<" "<<quantities[j][k]<<" ";
                        }
                        std::cout<<moneys[j]<<" "<<varyingWeights[maxIndex][j]<<" s#\r\n";
                    }
                    
                    if (varyingWeights[maxIndex][j] == std::stoi(orderQuantities[j])) {
                        //remove this stock entirely
                        while (!stocks[j].empty()) {
                            stocks[j].pop_back();
                        }
                        while (!quantities[j].empty()) {
                            quantities[j].pop_back();
                        }
                        moneys[j] = "0";

                        orderQuantities[j] = "1";
                        std::vector<std::string> find = iterLinComb[j];

                        int ind = -1;
                        for (int l=0; l<uniqueLinComb.size(); l++) {
                            if (newVecStringComp(find,uniqueLinComb[l])) {
                                ind = l;
                            }
                        }

                        if (ind != -1) {
                            // find corresponding order inside ind :(
                            
                            if (isSell) {
                                for (int m=0; m<associatedSells[ind].size(); m++) {
                                    if (associatedSells[ind][m] == j) {
                                        // this is tje one we cancel
                                        activeSells[ind][m] = -9999;
                                    }
                                }
                            }
                            else {
                                for (int m=0; m<associatedBuys[ind].size(); m++) {
                                    if (associatedBuys[ind][m] == j) {
                                        activeBuys[ind][m] = -9999;
                                    }
                                }
                            }
                            
                        }
                    }
                    else {
                        int t = std::stoi(orderQuantities[j]);
                        t = t - varyingWeights[maxIndex][j];
                        orderQuantities[j] = std::to_string(t);
                    }

                } 
            }
        }
    }
    std::cout<<profit;
}

void testing (){
    
    bool foundDollar = false;
    std::vector<std::string> words;   
    std::string bruh;
    Receiver rcv; 
    while (!foundDollar) {
        std::string message = rcv.readIML();
        bruh.append(message);
        if (message.find('$') != std::string::npos) {
            foundDollar = true;
        }
    }
    
        for (int i=0; i<bruh.length(); i++) {
            if (bruh[i] == '\r' || bruh[i] == '\n') {
                bruh[i] = '*';
            }
        }        
        std::istringstream iss(bruh);
        std::string word;
        //std::vector<std::string> words;
        while (std::getline(iss,word,'*')) {
            words.push_back(word);
        }
    



    for (int i=0; i<words.size(); i++) {
        std::cout<<words[i]<<"\r\n";
    }
    rcv.terminate();

}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr<<"bruh\n";
        return -1;
    }
    if (argv[1][0] == '1') {
        partOne();
        return 0;
    }
    else if (argv[1][0] == '2') {
        partTwo();
        return 0;
    }
    else if (argv[1][0] == '3') {
        partThree();
        return 0;
    }
    else if (argv[1][0] == '4') {
        testing();
        return 0;
    }
    else {
        std::cerr<<"bruh\r\n";
        return -1;
    } 
}
