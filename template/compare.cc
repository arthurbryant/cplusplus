#include <iostream>

using namespace std;

template <typename T>
int compare(const T& t1, const T& t2)
{
	if(t1 < t2)	return -1;
	if(t1 > t2)	return 1;

	return 0;
}

int main()
{
	string str1 = "hi";
	string str2 = "world";
	int i = compare(str1, str2);
	cout << i << endl;

	return 0;
}
