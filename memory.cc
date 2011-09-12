#include <iostream>

using namespace std;

int main()
{
	char *p = "arthur";
	cout << p << endl;
	char array[] = "bryant";
	array[0] = 'A';
	cout << array << endl;
	char *p1 = "arthur";
	cout << &p << "\t" << &p1 << endl;

	return 0;
}
