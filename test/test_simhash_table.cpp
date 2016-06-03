#include <iostream>
#include <cstdlib>

#include "simhash_table.h"

#include "simhash.h"
#include "hash.h"

#include <cstdlib>
#include <ctime>

using namespace std;
using namespace simhash;

#define TEST_EQUAL(x,y) \
if(x != y) \
{\
    cout << "Expression:\"" << #x << "==" << #y \
    << "\". Not equal, where x = " << x <<", and y = " << y << endl;\
}

#define TEST_TRUE(x) \
if(!x) \
{\
    cout << "Expression:\"" << #x << "\". Not True"<< endl; \
}

#define OUT_HEX(x) \
cout << hex << x << endl;


int TestSimhashTable()
{
    hash_t h1 = 0x0000000000000000;
    hash_t h2 = 0x0000000000000070;
    hash_t h3 = 0x0000000000000078;

    SimhashTablePtr tablePtr = CreateSimhashTable();
    tablePtr->Insert(h1);
    tablePtr->Insert(h3);

    cout << boolalpha << tablePtr->Search(h1) << endl;

    FindAnswerType ans;
    tablePtr->FindNearDups(h2, ans);

    cout << "ans.size() = " << ans.size() << ". They are :" << endl;
    for (FindAnswerType::iterator iter = ans.begin(); ans.end() != iter; ++iter)
    {
        cout << hex << *iter << endl;
    }
    cout << endl;


    tablePtr->FindNearDups(h1, ans);

    cout << "ans.size() = " << ans.size() << ". They are :" << endl;
    for (FindAnswerType::iterator iter = ans.begin(); ans.end() != iter; ++iter)
    {
        cout << hex << *iter << endl;
    }
    cout << endl;

    //TEST_EQUAL(h1, h3);
    return 0;
}

inline hash_t get_rand(hash_t seed)
{
    return seed * (hash_t)25214903917 + (hash_t)11;
}

int TestSimhashTableSearch()
{
    SimhashTablePtr tablePtr = CreateSimhashTable(3, 1);
    int repet = 10000000;
    clock_t start = clock();
    hash_t seed = (hash_t)rand();

    for (int i = 1; i <= repet; ++i)
    {
        if (i % 100000 == 0)
        {
            cout << "Current " << i << " repets, time" << clock() - start << " ms." << endl;
        }
        seed = get_rand(seed);
        tablePtr->Insert(seed);
    }
    clock_t end = clock();

    cout << "Total " << repet << " repets, time" << end - start << " ms." << endl;

    start = clock();
    uint_t count = 0;
    for (int i = 1; i <= repet; ++i)
    {
        if (i % 100000 == 0)
        {
            cout << "Current " << i << " repets, time" << clock() - start << " ms." << endl;
        }
        seed = get_rand(seed);
        count += tablePtr->HasNearDups(seed) ? 1 : 0;
    }
    end = clock();
    cout << "Total " << repet << " repets, time" << end - start << " ms." << endl;
    cout << "Search result: " << count << " hits." << endl;
    //TEST_EQUAL(h1, h3);
    return 0;
}

int TestSimhashTableSave()
{
    string binFile = "tmp.bin";
    string strFile = "tmp.str";
    SimhashTablePtr tablePtr = CreateSimhashTable(3, 1);
    int repet = 1000000;
    clock_t start = clock();
    hash_t seed = (hash_t)rand();

    for (int i = 1; i <= repet; ++i)
    {
        if (i % 100000 == 0)
        {
            cout << "Current " << i << " repets, time " << (clock() - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;
        }
        seed = get_rand(seed);
        tablePtr->Insert(seed);
    }
    clock_t end = clock();

    cout << "Total " << repet << " repets, time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;

    start = clock();
    tablePtr->SaveToFile(binFile, true);
    end = clock();
    cout << "Save to bin" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;

    start = clock();
    tablePtr->SaveToFile(strFile, false);
    end = clock();

    cout << "Save to str" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;
    //TEST_EQUAL(h1, h3);
    return 0;
}

int TestSimhashTableLoad()
{
    string binFile = "tmp.bin";
    string strFile = "tmp1.str";
    SimhashTablePtr tablePtr = CreateSimhashTable(3, 1);
    clock_t start = clock();
    tablePtr->LoadFromFile(binFile, true);
    clock_t end = clock();
    cout << "Load From bin File" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;

    start = clock();
    tablePtr->SaveToFile(strFile, false);
    end = clock();
    cout << "Save To str File" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;
    return 0;
}
int TestSimhashTableLoad1()
{
    string strFile1 = "tmp1.str";
    string strFile2 = "tmp2.str";
    SimhashTablePtr tablePtr = CreateSimhashTable(3, 1);
    clock_t start = clock();
    tablePtr->LoadFromFile(strFile1, false);
    clock_t end = clock();
    cout << "Load From str1 File" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;

    start = clock();
    tablePtr->SaveToFile(strFile2, false);
    end = clock();
    cout << "Save To str2 File" << endl;
    cout << "Total time " << (end - start) * 1000 / CLOCKS_PER_SEC << " ms." << endl;
    return 0;
}
int main()
{
    //TestIsSimilary();
    //TestBuildFromHashFeature();
    //TestJenkinHash();
    //TestBuildFromStringFeature();
    //TestSimhashTable();
    //TestSimhashTableInsert();
    //TestSimhashTableSearch();
    TestSimhashTableSave();
    TestSimhashTableLoad();
    TestSimhashTableLoad1();
    cin.get();
    return 0;
}

