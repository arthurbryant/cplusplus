#include <iostream>
#include <string>

using namespace std;

void switchString(string s1, string s2)
{
    string tmp;
    if(s1.size() > s2.size())
    {
        tmp = s1;
        s1 = s2;
        s2 = tmp;
    }
    cout << s1 << endl;
}

int main()
{
    string s1 = "hello, arthur";
    string s2 = "excellent";

    switchString(s1, s2);

    return 0;
}
