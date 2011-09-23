#include <iostream>

using namespace std;


void testPointer(char *s)
{
    s = "abcd";
}
int main()
{
    char *p = "ab";
    cout << p << endl;
    testPointer(p);
    cout << p << endl;

    return 0;
}
