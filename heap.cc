#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

#define N 5

using namespace std;

void print(vector<int> v)
{
		vector<int>::iterator iter = v.begin();
		while(iter != v.end())
			cout << *iter++ << " ";
		cout << endl;
}
int main()
{
		int arr[] = {2, 4, 1, 3, 5};
		vector<int> v(arr, arr+N);
		make_heap(v.begin(), v.end());
		cout << v.front() << endl;
		print(v);
		pop_heap(v.begin(), v.end());
		print(v);
		v.pop_back();
		print(v);
		cout << v.front() << endl;
                v.push_back(99);
		print(v);
                sort_heap(v.begin(), v.end());
		print(v);
                int i;
                for(i = 0; i < N; ++i)
                {
		        cout << v.front() << endl;
		        pop_heap(v.begin(), v.end());
		        v.pop_back();
                        sort_heap(v.begin(), v.end());
                    
                }

    return 0;
}
