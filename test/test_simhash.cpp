#include <iostream>

#include "simhash.h"
#include "hash.h"

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

int TestIsNearDups()
{
    hash_t h1 = 0x0000000000000000;
    hash_t h2 = 0x0000000000000070;
    hash_t h3 = 0x0000000000000078;

    TEST_TRUE(Simhash::IsNearDups(h1, h2));
    TEST_TRUE(Simhash::IsNearDups(h2, h3));
    TEST_TRUE(!Simhash::IsNearDups(h1, h3));

    TEST_EQUAL(h1, h3);
    return 0;
}

int TestBuildFromHashFeature()
{
    vector<Simhash::HashFeatureType> features;
    features.push_back(Simhash::HashFeatureType(0x0000000000001100, 1.0));
    features.push_back(Simhash::HashFeatureType(0x0000000000001010, 2.1));
    features.push_back(Simhash::HashFeatureType(0x0000000000000001, 4.3));
    hash_t hash = Simhash::Build(features);
    TEST_EQUAL(hash, 0x0000000000000001);
    features.push_back(Simhash::HashFeatureType(0x0000000000001100, 2.3));
    hash = Simhash::Build(features);
    TEST_EQUAL(hash, 0x0000000000001000);
    return 0;
}

int TestBuildFromStringFeature()
{
    vector<Simhash::StringFeatureType> features;
    features.push_back(Simhash::StringFeatureType("abcde", 1.0));
    features.push_back(Simhash::StringFeatureType("fghij", 2.0));
    features.push_back(Simhash::StringFeatureType("klmno", 4.3));
    hash_t hash = Simhash::Build(features, JenkinsHash);
    TEST_EQUAL(hash, 0xB7BE6A85658DB55D);
    return 0;
}

int TestJenkinHash()
{
    TEST_EQUAL(JenkinsHash("ab"), 0x6B79A0F2FBB3A8DF);
    TEST_EQUAL(JenkinsHash("ac"), 0x00460C21632E499E);
    TEST_EQUAL(JenkinsHash("bb"), 0xFFF5901B5C901B03);
    return 0;
}

/*
int main()
{
//TestIsSimilary();
//TestBuildFromHashFeature();
//TestJenkinHash();
//TestBuildFromStringFeature();
cin.get();
return 0;
}
*/
