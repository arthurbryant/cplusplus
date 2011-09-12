#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template <typename T>
void foo(ostream& os, const T& t)
{
	if(os != NULL)
		os << t;
}

int main()
{
	foo(cout, "arthur");
	
	string filename = "txt";
	ofstream outfile(filename.c_str());
	foo(outfile, 12);

	stringstream ss(stringstream::in | stringstream::out);
	foo(ss, "good");
	string str;
	ss >> str;
	cout << str << endl;
	return 0;
}
