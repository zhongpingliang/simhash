
#include <string>
#include <vector>
#include <iostream>

#include "string_handler.h"
#include "hash.h"
#include "simhash.h"
#include "simhash_table.h"

using namespace simhash;
using namespace std;

int main()
{
    string sentences[]  = {
        "I love china, I love Chengdu .",
        "Today is Sunday ."             ,
        "What are you talking about ?"  ,
        "Sounds good !"                 ,
        "Fight againt darkness ."       ,
        "Lock lock ."
    };
    SimhashTablePtr tablePtr = CreateSimhashTable(3U, 1U);
    vector<string> words;
    vector<pair<string, real_t> > features;
    hash_t simhashValue = 0U; // uint64_t
    for (uint_t i = 0; i < 6; ++i)
    {
        StringHandler::SplitString(sentences[i], words);
        features.clear();
        for (vector<string>::size_type j = 0; j < words.size(); ++j)
        {
            features.push_back(Simhash::StringFeatureType(words[j], 1.0));
        }
        hash_t simhashValue = Simhash::Build(features, JenkinsHash);
        tablePtr->Insert(simhashValue);
    }
    cout << "The size of table is " << tablePtr->GetSize() << endl;
    string query = "I love china, I love Chengdu . .";

    StringHandler::SplitString(query, words);
    features.clear();
    for (vector<string>::size_type j = 0; j < words.size(); ++j)
    {
        features.push_back(Simhash::StringFeatureType(words[j], 1.0));
    }
    simhashValue = Simhash::Build(features, JenkinsHash);
    bool s = tablePtr->Search(simhashValue);
    if (s)
    {
        cout << "No near duplicate in table." << endl;
    }
    else
    {
        cout << "Some near duplicate in table." << endl;
    }
    return 0;
}
