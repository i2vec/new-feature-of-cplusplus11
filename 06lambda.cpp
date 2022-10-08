#include<vector>
#include<algorithm>
#include<iostream>

int main()
{
    std::vector<int> tmpv = {1, 3, 5, 2, 4, 6};
    std::sort(tmpv.begin(),tmpv.end(), [](int a, int b){return a > b;});
    for(auto& i : tmpv){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}