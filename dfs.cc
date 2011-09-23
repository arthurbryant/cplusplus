#include <iostream>
#include <stack>
#include "../dataStruct/tree.h"

using namespace std;

void dfs(Tree t)
{
    stack<Tree> s;
    Tree tmp;
    if(NULL == t)
        return;
    s.push(t);
    while(!s.empty())
    {
        tmp = s.top();
        cout << tmp->data << endl;
        s.pop();
        if(NULL != tmp->right)
            s.push(tmp->right);
        if(NULL != tmp->left)
            s.push(tmp->left);
    }
}
int main()
{
    Tree t = NULL;
    t = createTree(t);
    //inOrder(t);
    dfs(t);
}

