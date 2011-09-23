#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    streambuf *buf;
    ofstream fout;
    fout.open("output");
    if(fout.fail())
        cerr << "not exist";
    buf = cout.rdbuf();
    cout.rdbuf(fout.rdbuf());

    string s = "good work";
    cout << s << endl;
    cout.rdbuf(buf);

    cout << "good" << endl;
    fout.close();

    return 0;
}
