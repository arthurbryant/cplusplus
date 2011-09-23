#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream fin;
    fin.open("data");
    if(fin.fail())
        return -1;
    streambuf *buf;
    buf = cin.rdbuf();
    cin.rdbuf(fin.rdbuf());
    string c;
    while((cin >> c))
        cout << c;

    cin.rdbuf(buf);
    fin.close();

    return 0;
}
