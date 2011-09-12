#include "header.h"
#include <string>
#include <iostream>
#include <cassert>

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

bool endWith(string source, string subString)
{
	if(source.length() < subString.length())
		return false;

	string tmp;
	tmp = source.substr(source.length() - subString.length());
	if(0 == tmp.compare(subString))
		return true;
	else
		return false;
}
void test_endWith()
{
	assert(endWith("abcd", "cd"));
	assert(!endWith("abcd12", "123"));
	assert(endWith("abcd12", "12"));

}
/*
int main()
{
	test_endWith();

    return 0;
}
*/
