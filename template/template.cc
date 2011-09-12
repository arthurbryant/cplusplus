#include <iostream>

using namespace std;

template <typename T>

T abs(T& t)
{
	if(t < 0)
		return -t;
	else
		return t;
}

int main()
{
	int i = -1;
	float f = -21.1;
	double d = -12.032;
	
	i = abs(i);
	f = abs(f);
	d = abs(d);
	cout << i << "\t" << endl;
	cout << f << "\t" << endl;
	cout << d << endl;

	return 0;
}
