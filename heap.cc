#include <iostream>
#include <cstdlib>

#define minData -1;

using namespace std;

typedef int ElementType;
typedef struct HeapStruct* PQ;

struct HeapStruct
{
    int size;
    int capacity;
    ElementType *Elements;
};

PQ init(int max)
{
    PQ H;
    H = new HeapStruct;
    if(NULL == H)
    {
        cerr << "heap overflow" << endl;
        exit(-1);
    }
    H->Elements = new ElementType[max + 1];
    H->size = 0;
    H->capacity = max;
    H->Elements[0] = minData;
    
    return H;
}
bool isFull(PQ H)
{
    return H->size >= H->capacity ? true : false;
}
void insert(ElementType x, PQ H)
{
    if(isFull(H))
    {
        cerr << "heap is full" << endl;
        exit(-1);
    }
    int i;
    for(i = ++H->size; x < H->Elements[i/2]; i/=2)
        H->Elements[i] = H->Elements[i/2];
    H->Elements[i] = x;
}
bool isEmpty(PQ H)
{
    return H->size <= 0 ? true : false;
}
void deleteMin(PQ H)
{
    if(isEmpty(H))
    {
        cerr << "heap is empty" << endl;
        exit(-1);
    }
    ElementType last = H->Elements[H->size--];
    cout << "last = " << last << endl;
    int i, child;
    for(i = 1; i*2 <= H->size; i = child)
    {
        child = i*2;
        if(child != H->size && H->Elements[child+1] < H->Elements[child])
            ++child;
        if(last > H->Elements[child])
            H->Elements[i] = H->Elements[child];
        else
            break;
    }
    H->Elements[i] = last;
}
void printHeap(PQ H)
{
    for(int i = 1; i <= H->size; ++i)
        cout << H->Elements[i] << "\t";
    cout << endl;
}
int main()
{
    int max = 10;
    int arr[] = {6, 2, 5, 8, 1, 9, 3};
    int len = sizeof(arr) / sizeof(int);
    PQ H;
    H = init(max);
    for(int i = 0; i < len; ++i)
        insert(arr[i], H);
    printHeap(H);
    deleteMin(H);
    printHeap(H);
    deleteMin(H);
    printHeap(H);

    delete[] H->Elements;
    delete H;
    
    return 0;
}
