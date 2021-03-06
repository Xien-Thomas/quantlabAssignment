#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
void updateMaxTimeGap(string, string, map<string, vector<int>> &m);
void updateTotalVolume(string, string, map<string, vector<int>> &m);
void updateMaxPrice(string, string, map<string, vector<int>> &m);
void updateWeightedAverage(string, map<string, vector<int>> &m);
void outputCSV(vector<string>, map<string, vector<int>> &m);

int main()
{

    //Map data structure
    //Each symbol has a: (Vector) which contains
    //          previoustime(0), currentGap(1), volumeSum(2), currentVolume(3), maxPrice(4), currentPrice(5),  rollingTrades(6), weightedAverage(7)
    map<string, vector<int>> m;
    //input file
    ifstream file("input.csv");

    string line;
    vector<string> symbolInput;
    int maxGap = 0;
    int totalVolume = 0, maxPrice = 0, wAvg = 0;

    while (getline(file, line))
    {
        vector<int> mapInput;

        int pos1 = line.find(',') + 1;
        int pos2 = line.find(',', pos1);

        string symbol = line.substr(pos1, pos2 - pos1);
        if (m.find(symbol) == m.end())
        {
            //initialize for every new symbol
            m[symbol] = mapInput;
            symbolInput.push_back(symbol);
        }
        //update
        updateMaxTimeGap(line, symbol, m);
        updateTotalVolume(line, symbol, m);
        updateMaxPrice(line, symbol, m);
        updateWeightedAverage(symbol, m);
    }

    //sort symbols
    sort(symbolInput.begin(), symbolInput.end());

    outputCSV(symbolInput, m);
    return 0;
}
void updateMaxTimeGap(string input, string symbolInput, map<string, vector<int>> &m)
{
    //Assuming the time is in a single day
    //Parse input for the timestamp
    int time = 0, diff = 0, pos = 0;
    pos = input.find(',');
    string inputTime = input.substr(2, pos - 1);
    time = stol(inputTime);

    if (m[symbolInput].size() == 0)
    {
        //empty log (init)
        m[symbolInput].push_back(time);
        m[symbolInput].push_back(0); //maxGap
    }
    else
    { //New time gap. If bigger than the last time gap then replace
        diff = time - m[symbolInput][0];

        if (m[symbolInput][1] < diff)
        {

            m[symbolInput][0] = time;
            m[symbolInput][1] = diff;
        }
    }
}
void updateTotalVolume(string input, string symbolInput, map<string, vector<int>> &m)
{
    //Parse input for the volume
    int sumVolume = 0, pos = 0, pos2 = 0, vol;
    pos = input.find(',') + 1;
    pos2 = input.find(',', pos) + 1;
    string inputVol = input.substr(pos2, input.find(',', pos2) - pos2);
    sumVolume = stol(inputVol);

    if (m[symbolInput].size() < 3)
    {
        //emptylog (init)
        m[symbolInput].push_back(sumVolume);
        m[symbolInput].push_back(sumVolume);
    }
    else
    {
        //Rolling Volume sum and currentVolume
        m[symbolInput][2] = m[symbolInput][2] + sumVolume;
        m[symbolInput][3] = sumVolume;
    }
}
void updateMaxPrice(string input, string symbolInput, map<string, vector<int>> &m)
{
    //Parse input for the price
    int price = 0, pos = 0, pos2 = 0, pos3 = 0;
    pos = input.find(',') + 1;
    pos2 = input.find(',', pos) + 1;
    pos3 = input.find(',', pos2) + 1;
    string inputPrice = input.substr(pos3, input.find(',', pos3) - pos3);
    price = stol(inputPrice);
    if (m[symbolInput].size() < 5)
    {
        //emptylog (init)
        m[symbolInput].push_back(price);
        m[symbolInput].push_back(price);
    }
    else
    {
        //get max price
        if (m[symbolInput][4] < price)
        {
            m[symbolInput][4] = price;
        }
        //get current price
        m[symbolInput][5] = price;
    }
}
void updateWeightedAverage(string symbolInput, map<string, vector<int>> &m)
{
    int price = 0, volume = 0, totalVolume = 0, trade = 0;
    price = m[symbolInput][5];
    volume = m[symbolInput][3];
    totalVolume = m[symbolInput][2];
    int pricePerVol = volume * price;
    //Roling trade
    if (m[symbolInput].size() < 7)
    {
        //emptylog (init)
        m[symbolInput].push_back(pricePerVol);
        m[symbolInput].push_back(totalVolume);
    }
    else
    {
        //numerator
        m[symbolInput][6] = m[symbolInput][6] + pricePerVol;
        //Weighted Average
        m[symbolInput][7] = m[symbolInput][6] / totalVolume;
    }
}
void outputCSV(vector<string> symbol, map<string, vector<int>> &m)
{
    //open file
    ofstream newFile;
    newFile.open("output.csv");

    //enter contents into csv file
    for (int i = 0; i < symbol.size(); i++)
    {
        newFile << symbol[i];
        newFile << ",";
        newFile << m[symbol[i]][1];
        newFile << ",";
        newFile << m[symbol[i]][2];
        newFile << ",";
        newFile << m[symbol[i]][7];
        newFile << ",";
        newFile << m[symbol[i]][4];
        newFile << endl;
    }
    newFile.close();
}