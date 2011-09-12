#include "header.h"
#include <map>
#include <iostream>

KnowlegeBase::KnowlegeBase()
{
    KnowlegeBase::dictFiles.insert(map<string, string>::value_type("domain-name", "domain.lst"));
    KnowlegeBase::dictFiles.insert(map<string, string>::value_type("freq-word-list", "freq.lst"));
}

DataSet KnowlegeBase::getDictionary(string name)
{
    //DataSet ds = (DataSet)self->cached[name];
    DataSet ds(name);

    ds.loadFromFile("test/" + this->dictFiles[name]);
    this->cached.insert(map<string, DataSet>::value_type(name, ds));

    return ds;
}

void KnowlegeBase::put(string name, DataSet ds)
{
    KnowlegeBase::cached.insert(map<string, DataSet>::value_type(name, ds));
}
/*
int main()
{
    KnowlegeBase kb;
    DataSet domain = kb.getDictionary("domain-name");
    cout << domain.getName() << endl;

    return 0;
}
*/
