#ifndef HAVE_HEADER_H
#define HAVE_HEADER_H

#include <string>
#include <set>
#include <map>

using namespace std;

string allToLower(string);
bool endWith(string, string);

class DataSet
{
    private:
        string name;
        set<string> nameLookup;
    public:
        DataSet();
        DataSet(string name);
        bool lookup(string name);
        void loadFromFile(string fileName);
        void addItem(string name);
        string getName();
};

class KnowlegeBase
{
    private:
        map<string, DataSet> cached;
        map<string, string> dictFiles;
        //static KnowlegeBase *self;
    public:
        KnowlegeBase();
        DataSet getDictionary(string name);
        void put(string name, DataSet ds);
       
};

#endif
