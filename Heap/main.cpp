#include <iostream>
#include "console.h"
#include "Heap.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

Vector<int> heapSort(Vector<int> vec)
{
    Heap<int> hp;
    for(int x : vec)
    {
        hp.insert(x);
    }

    Vector<int> result;
    while(!hp.isEmpty())
    {
        result.add(hp.extract());
    }
    return result;
}

int K_Max(int k, Vector<int> vec)
{
    assert(vec.size()>k);
    Heap<int> hp;
    for(int i = 0; i<vec.size();i++)
    {
        if(i < k)
        {
            hp.insert(vec[i]);
        }
        else
        {
            if(vec[i] > hp.peek())
            {
                hp.extract();
                hp.insert(vec[i]);
            }
        }
    }
    return hp.peek();
}

bool operator<(const Vector<int>& vec1, const Vector<int>& vec2)
{
    return vec1[0] < vec2[0];
}

Vector<int> merge(Vector<Vector<int>> vec)
{
    Heap<Vector<int>> hp;
    for(Vector<int> x : vec)
    {
        if(!x.isEmpty())
            hp.insert(x);
    }

    Vector<int> result;
    while(!hp.isEmpty())
    {
        Vector<int>& elem = hp.peek();
        result.add(elem.remove(0));
        if(elem.isEmpty())
            hp.extract();
        else
            hp.peekUpdate();
    }
    return result;
}

int main() {

    Heap<int> hp;
    hp.insert(9);
    hp.insert(8);
    hp.insert(1);
    hp.insert(5);
    if(hp.contains(5))
        cout<<"存在5"<<endl;
    else
        cout<<"不存在5"<<endl;
    cout<<"删除5后"<<endl;
    hp.remove(5);
    if(hp.contains(5))
        cout<<"存在5"<<endl;
    else
        cout<<"不存在5"<<endl;

    Vector<int> vec = {3,4,2,1,6,9,5};
    cout<<heapSort(vec)<<endl;
    cout<<K_Max(4,vec)<<endl;
    Vector<Vector<int>> Vec = {{1,2,3},{1,3,5},{2,4,6}};
    cout<<merge(Vec)<<endl;




    if (runSimpleTests(ALL_TESTS)) {
        return 0;
    }
    return 0;
}
