#include "header.h"

#include <iostream>
#include <fstream>

DataSet::DataSet() {}

DataSet::DataSet(string name)
{
    DataSet::name = name;
}

bool DataSet::lookup(string name)
{
    return nameLookup.count(allToLower(name));
}

void DataSet::loadFromFile(string fileName)
{
    ifstream infile(fileName.c_str());
    if(!infile)
    {
        cerr << "Unable to open file: " << fileName << endl;
        exit(-1);
    }
    
    string strLine;
    while(getline(infile, strLine))
    {
        addItem(strLine);
    }

    infile.close();
}

void DataSet::addItem(string name)
{
    nameLookup.insert(allToLower(name));
}

string DataSet::getName()
{
    return DataSet::name;
}

/*
int main()
{
    DataSet ds("11");
    ds.loadFromFile("domain.lst");
    string input;
    while(1)
    {
        cin >> input;
        bool result = ds.lookup(input);
        cout << result << endl;
    }

    return 0;
}
*/
