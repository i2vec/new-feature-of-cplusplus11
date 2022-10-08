#include<tuple>
#include<string>
#include<iostream>

auto get_params(){
    std::tuple<int, char, std::string> res{1, 'a', "helloworld"};
    return res;
}

int main()
{
    int in;
    char ch;
    std::string str;
    std::tie(in, ch, str) = get_params();
    std::cout << "in:" << in << std::endl;
    std::cout << "ch:" << ch << std::endl;
    std::cout << "str:" << str << std::endl;
}