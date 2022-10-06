#include<iostream>
#include<string>


int main()
{
    auto var_int = 1;
    auto var_float = 1.5;
    auto var_string = "hello world";
    std::cout << "var_int: " << typeid(var_int).name() << std::endl;
    std::cout << "var_float: " <<  typeid(var_float).name() << std::endl;
    std::cout << "var_string: " << typeid(var_string).name() << std::endl;
    return 0;
}