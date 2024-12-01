#include <array>
#include <vector>
#include <iostream>

using namespace std;
class Cat
{

};
void foo(int num)
{
    vector<Cat> cats(num);

    // do some computation over cats
}
bool bar(int num)
{
    constexpr int maxCats = 1000;
    if(maxCats < num)
    {
        return false;
    }
    array<Cat, maxCats> cats;
    // do some computation over cats
}
int main(void)
{
    int count;
    cin >> count;
    // if we need performance critical cats -> alloc in STACK
    for(int i=0; i<10000; ++i)
    {
        //foo(count);   // heap (vector)
        bar(count);     // STACK (array)
    }
}