#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>


using namespace std;

#define MAX 10

void init(int arr[], int len)
{
    int i;
    srand(time(NULL));
    for(i = 0; i < len; ++i)
        arr[i] = rand() - 370558018;
}
void display(int arr[], int len)
{
    int i;
    for(i = 0; i < len; ++i)
        printf("%d\t", arr[i]);
    printf("\n");
}
int findMaxContinuousSequence1(int arr[], int len)
{
   int i, j;
   int sum = 0, tmp;
   for(i = 0; i < len; ++i)
   {
    tmp = arr[i];
    if(tmp > sum)
        sum = tmp;
    for(j = i+1; j < len; ++j)
    {
        tmp += arr[j];
        if(tmp > sum)
            sum = tmp;
    }
   }
   return sum;
}

int findMaxContinuousSequence2(int arr[], int start, int end)
{
    int maxLeft = 0, maxRight = 0;
    int mid = (start + end) / 2;
    int i, j;

    if(start > end)
        return 0;
    if(start == end)
        return max(0, arr[start]);
    int tmpLeft = 0;
    for(i = mid; i >= start; --i)
    {
        tmpLeft += arr[i];
        if(tmpLeft > maxLeft)
            maxLeft = tmpLeft;
    }
    int tmpRight = 0;
    for(j = mid+1; j <= end; ++j)
    {
        tmpRight += arr[j];
        if(tmpRight > maxRight)
            maxRight = tmpRight;
    }
    return max(maxLeft + maxRight, max(findMaxContinuousSequence2(arr, start, mid), findMaxContinuousSequence2(arr, mid+1, end)));
}

int findMaxContinuousSequence3(int arr[], int len)
{
    int i;
    int maxsofar = 0, maxendinghere = 0;

    for(i = 0; i < len; ++i)
    {
       maxendinghere = max(0, maxendinghere + arr[i]);
       maxsofar = max(maxsofar, maxendinghere);
    }
    return maxsofar;
}

int main()
{
    int result;
    int arr[MAX] = {31, -41, 59, 26, -53, 58, 97, -93, -23, 84};
    //init(arr, MAX);
    display(arr, MAX);
    result = findMaxContinuousSequence1(arr, MAX);
    printf("%d\n", result);
    result = findMaxContinuousSequence2(arr, 0, MAX-1);
    printf("%d\n", result);
    result = findMaxContinuousSequence3(arr, MAX);
    printf("%d\n", result);
    


    return 0;
}
