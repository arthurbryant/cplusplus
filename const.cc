#include <iostream>
#include "../../dataStruct/link.c"

using namespace std;

void f(const int arr[])
{
    arr[0] = 1;
}
int main()
{
    int arr[] = {1, 2};
    f(arr);

    return 0;
}
