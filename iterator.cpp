#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main()
{
    vector<int> vecs = {1,2,3,4,5};

    for(auto iter = vecs.begin(); iter != vecs.end();)
    {
        if(*iter == 4)
        {
            iter = vecs.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    for(auto iter = vecs.begin(); iter != vecs.end();)
    {
        cout << *iter << endl;
        ++iter;
    }
    return 0;
}