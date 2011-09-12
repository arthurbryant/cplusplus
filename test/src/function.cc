#include "header.h"
#include <string>
#include <iostream>

string allToLower(string str)
{
    if(str == "")
    {
        return str;
    }
    int i = 0;
    char c;
    string lower = "";
    while(str[i])
    {
        c = str[i];
        lower += tolower(c);
        ++i;
    }
    return lower;
}

/*
int main()
{
	string str = "ABcdeF";

	string tmp = allToLower(str);
	cout << tmp << endl;

    return 0;
}
*/
