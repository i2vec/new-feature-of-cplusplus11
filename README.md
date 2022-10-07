# C++ 11常用新特性demo
> 该仓库建立的目的是对C++11的新特性进行初步学习。后续如果有一些C++的应用项目会在此处更新链接。
<!--ts-->
* [C++ 11常用新特性demo](#c-11常用新特性demo)
   * [C++11 简介](#c11-简介)
   * [C++11 新特性](#c11-新特性)
      * [1.  类型推导](#1--类型推导)
         * [1.1 auto关键字自动类型推导](#11-auto关键字自动类型推导)
         * [1.2 decltype类型推导](#12-decltype类型推导)
         * [1.3 auto与decltype配合：拖尾返回类型](#13-auto与decltype配合拖尾返回类型)
      * [2. nullptr](#2-nullptr)
      * [3.](#3)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: runner, at: Fri Oct  7 03:50:23 UTC 2022 -->

<!--te-->
## C++11 简介
相比于C++98/03，C++11则带来了数量可观的变化，其中包含了约140个新特性，以及对C++03标准中约600个缺陷的修正，这使得C++11更像是从C++98/03中孕育出的一种新语言。相比较而言，C++11能更好地用于系统开发和库开发、语法更加泛华和简单化、更加稳定和安全，不仅功能更强大，而且能提升程序员的开发效率。
## C++11 新特性
### 1.  类型推导
#### 1.1 auto关键字自动类型推导
&emsp;&emsp;C++11 赋予 auto 关键字新的含义，使用它来做自动类型推导。也就是说，使用了 auto 关键字以后，编译器会在编译期间自动推导出变量的类型，这样我们就不用手动指明变量的数据类型了。
* 基本语法
```cpp
auto name = value;
```
* 注意事项
    * 推导的时候不能有二义性。
        例如：如果变量已经被推导为int类型，后面就不能被赋值为1.25。
    * 使用 auto 类型推导的变量必须马上初始化。
    * auto 不能在函数的参数中使用。
        这个很好理解，因为auto在定义后必须马上进行初始化，而函数的参数则是再使用时才会进行初始化，与auto的使用刚好是矛盾的。
    * auto 不能作用于类的非静态成员变量（也就是没有 static 关键字修饰的成员变量）中。
    * auto 关键字不能定义数组。
    * auto 不能作用于模板参数。

* auto的一些常见用法
    * auto作为迭代器
    ```cpp
    #include <vector>
    using namespace std;

    int main(){
        vector< vector<int> > v;
        //vector< vector<int> >::iterator i = v.begin();
        auto i = v.begin();  //使用 auto 代替具体的类型
        return 0;
    }
    ```
    * auto用于泛型编程
    当我们不知道变量是什么类型，或者不希望指明具体类型的时候，比如泛型编程中。

#### 1.2 decltype类型推导
&emsp;&emsp;decltype全拼为declare type，声明类型。
* 基本语法
```cpp
// 其中exp为一个普通的表达式，可以是任何复杂的类型。
// 但是必须保证exp的结果有类型。
// 可以不初始化
decltype(exp) varname = value;
```
&emsp;&emsp;decltype 能够根据变量、字面量、带有运算符的表达式推导出变量的类型。注意其中的y没有被初始化。
```cpp
int a = 0;
decltype(a) b = 1;  //b 被推导成了 int
decltype(10.8) x = 5.5;  //x 被推导成了 double
decltype(x + 100) y;  //y 被推导成了 double
```
* decltype推导规则
    * 如果 exp 是一个不被括号( )包围的表达式，或者是一个类成员访问表达式，或者是一个单独的变量，那么 decltype(exp) 的类型就和 exp 一致，这是最普遍最常见的情况。
    * 如果 exp 是函数调用，那么 decltype(exp) 的类型就和函数返回值的类型一致。
    * 如果 exp 是一个左值，或者被括号( )包围，那么 decltype(exp) 的类型就是 exp 的引用；假设 exp 的类型为 T，那么 decltype(exp) 的类型就是 T&。

* decltype的使用
    * auto 只能用于类的静态成员，不能用于类的非静态成员（普通成员），如果我们想推导非静态成员的类型，这个时候就必须使用 decltype了。
        * 原版代码   
        ```cpp
        #include <vector>
        using namespace std;

        template <typename T>
        class Base {
        public:
            void func(T& container) {
                m_it = container.begin();
            }

        private:
            typename T::iterator m_it;  //注意这里
        };

        int main()
        {
            const vector<int> v;
            Base<const vector<int>> obj;
            obj.func(v);

            return 0;
        }
        ```
        * decltype版
        ```cpp
        template <typename T>
        class Base {
        public:
            void func(T& container) {
                m_it = container.begin();
            }
        private:
            decltype(T().begin()) m_it;  //注意这里
        };
        ```
#### 1.3 auto与decltype配合：拖尾返回类型
&emsp;&emsp;考虑这样一个例子加法函数的例子，在传统 C++ 中我们必须这么写：
```cpp
template<typename R, typename T, typename U>
R add(T x, U y) {
    return x+y
}
```
&emsp;&emsp;这样的代码其实变得很丑陋，因为程序员在使用这个模板函数的时候，必须明确指出返回类型。但事实上我们并不知道 add() 这个函数会做什么样的操作，获得一个什么样的返回类型。
&emsp;&emsp;在 C++11 中这个问题得到解决。虽然你可能马上回反应出来使用 decltype 推导 x+y 的类型，写出这样的代码：
```cpp
decltype(x+y) add(T x, U y);
```
但事实上这样的写法并不能通过编译。这是因为在编译器读到decltype(x+y) 时，x 和 y 尚未被定义。为了解决这个问题，C++11 还引入了一个叫做**拖尾返回类型（trailing return type**，利用 auto 关键字将返回类型后置：
```cpp
template<typename T, typename U>
auto add(T x, U y) -> decltype(x+y) {
    return x+y;
}
```
### 2. nullptr
&emsp;&emsp;nullptr 出现的目的是为了替代 NULL。
&emsp;&emsp;在某种意义上来说，传统 C++ 会把 NULL、0 视为同一种东西，这取决于编译器如何定义 NULL，有些编译器会将 NULL 定义为 ((void*)0)，有些则会直接将其定义为 0。
&emsp;&emsp;C++ 不允许直接将 void * 隐式转换到其他类型，但如果 NULL 被定义为 ((void*)0)，那么当编译char *ch = NULL;时，NULL 只好被定义为 0。
&emsp;&emsp;为了解决这个问题，C++11 引入了 nullptr 关键字，专门用来区分空指针、0。nullptr 的类型为 nullptr_t，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或者不等的比较。

### 3. 