# C++ 11常用新特性demo
> 该仓库建立的目的是对C++11的新特性进行初步学习。后续如果有一些C++的应用项目会在此处更新链接。
<!--ts-->
* [C++ 11常用新特性demo](#c-11常用新特性demo)
   * [C++11 简介](#c11-简介)
   * [C++11 新特性](#c11-新特性)
      * [1.  类型推导](#1--类型推导)
         * [1.1 auto关键字自动类型推导](#11-auto关键字自动类型推导)
         * [1.2 decltype类型推导](#12-decltype类型推导)
         * [1.3 auto与decltype配合：拖尾返回类型（返回类型后置）](#13-auto与decltype配合拖尾返回类型返回类型后置)
      * [2. nullptr](#2-nullptr)
      * [3. 初始化列表](#3-初始化列表)
      * [4. 支持函数模版的默认模版参数](#4-支持函数模版的默认模版参数)
      * [5. 区间迭代](#5-区间迭代)
      * [6. Lambda表达式](#6-lambda表达式)
      * [7. 右值引用](#7-右值引用)
      * [8. tuple元组](#8-tuple元组)
      * [9. 智能指针](#9-智能指针)
   * [参考资料](#参考资料)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: runner, at: Sat Oct  8 13:55:14 UTC 2022 -->

<!--te-->
## C++11 简介
&emsp;&emsp;相比于C++98/03，C++11则带来了数量可观的变化，其中包含了约140个新特性，以及对C++03标准中约600个缺陷的修正，这使得C++11更像是从C++98/03中孕育出的一种新语言。相比较而言，C++11能更好地用于系统开发和库开发、语法更加泛华和简单化、更加稳定和安全，不仅功能更强大，而且能提升程序员的开发效率。
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
#### 1.3 auto与decltype配合：拖尾返回类型（返回类型后置）
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
但事实上这样的写法并不能通过编译。这是因为在编译器读到decltype(x+y) 时，x 和 y 尚未被定义。为了解决这个问题，C++11 还引入了一个叫做**拖尾返回类型（trailing return type)**，利用 auto 关键字将返回类型后置：
```cpp
template<typename T, typename U>
auto add(T x, U y) -> decltype(x+y) {
    return x+y;
}
```
从 C++14 开始是可以直接让普通函数具备返回值推导，因此下面的写法变得合法：
```cpp
template<typename T, typename U>
auto add(T x, U y) {
    return x+y;
}
```
### 2. nullptr
&emsp;&emsp;nullptr 出现的目的是为了替代 NULL。  
&emsp;&emsp;在某种意义上来说，传统 C++ 会把 NULL、0 视为同一种东西，这取决于编译器如何定义 NULL，有些编译器会将 NULL 定义为 ((void*)0)，有些则会直接将其定义为 0。  
&emsp;&emsp;C++ 不允许直接将 void * 隐式转换到其他类型，但如果 NULL 被定义为 ((void*)0)，那么当编译char *ch = NULL;时，NULL 只好被定义为 0。  
&emsp;&emsp;为了解决这个问题，C++11 引入了 nullptr 关键字，专门用来区分空指针、0。nullptr 的类型为 nullptr_t，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或者不等的比较。 
```cpp
//根据语言定义，在指针上下文中的常数0会在编译时转换为空指针。
//也就是说，在赋值比较的时候，如果一边是指针类型的值或者表达式，
//编译器可以确定另一边的常数0为空指针并生成正确的空指针指。
char* a = 0;
if(a == nullptr){
    std::cout << "hello nullptr" << std::endl;
}
return 0;
``` 

### 3. 初始化列表
&emsp;&emsp;C++11提供了统一的语法来初始化任意的对象，例如：
```cpp
struct A {
    int a;
    float b;
};
struct B {

    B(int _a, float _b): a(_a), b(_b) {}
private:
    int a;
    float b;
};

A a {1, 1.1};    // 统一的初始化语法
B b {2, 2.2};
```
C++11 还把初始化列表的概念绑定到了类型上，并将其称之为 std::initializer_list，允许构造函数或其他函数像参数一样使用初始化列表，这就为类对象的初始化与普通数组和 POD 的初始化方法提供了统一的桥梁，例如：
```cpp
#include <initializer_list>

class Magic {
public:
    Magic(std::initializer_list<int> list) {}
};

Magic magic = {1,2,3,4,5};
std::vector<int> v = {1, 2, 3, 4};
```
### 4. 支持函数模版的默认模版参数
&emsp;&emsp;在C++98标准中，类模版可以有默认的模版参数，如下：
```cpp
template <typename T, typename U = int, U N = 0>
struct Foo
{
    // ...
};
```
但是不支持函数的默认模版参数：
```cpp
template <typename T = int>  // error in C++98/03: default template arguments
void func()
{
    // ...
}
```
现在这一个限制在C++11中被解除了。上面的func函数在C++11中可以直接进行调用。 

### 5. 区间迭代
&emsp;&emsp;C++11引入了基于范围的迭代写法，我们可以写出像python一样简介的循环语句。
```cpp
// & 启用了引用
for(auto &i : arr) {    
    std::cout << i << std::endl;
}
```
### 6. Lambda表达式
* 基本语法
    ```cpp
    [caputure] (params) opt -> ret { body; };
    ```
    1. capture是捕获列表:lambda表达式的捕获列表精细控制了lambda表达式能够访问的外部变量，以及如何访问这些变量。
        * []不捕获任何变量。
        * [&]捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）。
        * [=]捕获外部作用域中所有变量，并作为副本在函数体中使用(按值捕获)。注意值捕获的前提是变量可以拷贝，且被捕获的变量在 lambda 表达式被创建时拷贝，而非调用时才拷贝。如果希望lambda表达式在调用时能即时访问外部变量，我们应当使用引用方式捕获。
    2. params:参数表；(选填)
    3. opt:函数选项；可以填mutable,exception,attribute（选填）
        * mutable说明lambda表达式体内的代码可以修改被捕获的变量，并且可以访问被捕获的对象的non-const方法。
        * exception说明lambda表达式是否抛出异常以及何种异常。
        * attribute用来声明属性。
    4. ret是返回值类型（拖尾返回类型）。(选填)
    5. body是函数体。
* 排序中使用lambda表达式
    ```cpp
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
    ```

### 7. 右值引用
* 什么是左右值
    * 左值：
        1. 左值可以取地址
        2. 左值可以修改
        3. 左值可以放在等号左右两边
    * 右值
        1. 右值不可以取地址
        2. 右值不可以修改
        3. 右值只能放在等号右边
        4. 右值往往是没有名称的
        * 右值也分为两种:
            1. 纯右值
                纯右值int a = 3;就是指等号右边的常数，上式中的3。
            2. 将亡值
                a. 函数的临时返回值int a = f(3); f(3)的返回值为右值，副本拷给a，然后消失。
                b. 表达式像(x+y)，其中(x+y)是右值。
    * 左右值使用原则
        1. 右值可以赋给左值，左值不可以赋给右值
            ```cpp
            int a = 3;  // a是左值，3是右值
            int d = a;  // d和a都是左值，左值可以赋给左值
            int &&d = a; × // 右值引用左值不行
            int &&d = 10;  // 右值引用右值可以
            int &&d = f(10);  // 右值引用右值可以
            ```
        2. 右值无法修改
        3. 编译器允许为左值建立引用，但不可以为右值建立引用
    这样存在一个问题：只有左值可以修改，那如果想要修改右值怎么办？因此有了**右值引用**。
* 右值引用语法： &&
* 使用右值引用注意三个问题
    1. 右值引用必须要进行初始化
    2. 不能使用左值为右值做初始化
    3. 右值引用可以对右值进行修改
* 右值引用的好处  
&emsp;&emsp;先想一下引用的目的，传递参数有两种方式：值传递和引用传递。两者相比引用传递的优势就是通过传递地址，来减少一次拷贝。在常规写程序的时候，使用的都是左值引用。左值引用有两个使用场景：函数传参、函数返回值。  
    1. 函数传参： int f(int& a);
    2. 函数返回值：int& f();

    以上两种情况使用的都是引用传递相比于值传递减少了拷贝次数。但是有一种情况会出问题：就是返回值是一个临时对象。如下

    ```cpp
    int& f(){
        int a;
        return a;
    }
    ```
    &emsp;&emsp;当返回对象a的地址的时候，a作为栈上的临时对象，作用域已经到了，将被析构。这样如果外界再对这个地址进行访问的时候，就会出现问题。这也是左值引用的一个弊端，而右值引用的出现就是为了解决这个问题。那右值引用如何解决返回的临时变量析构？当返回值为右值引用是，会把返回的临时变量中的内存据为己用，仍保持了有效性，也避免了拷贝。
* std::move  
&emsp;&emsp;std::move()可以将左值转换为右值。
* 右值引用的应用场景  
&emsp;&emsp;右值引用主要有两个应用场景：移动语义、完美转发
    1. 移动语义（就是将传进来的变量内存直接偷走）  
    &emsp;&emsp;如果我们将复制这中操作看作资源转移，那么传统的资源转让是通过拷贝进行实现的，需要两份空间。而移动语义是通过移动来实现资源转让，只需要一个空间。来看一下移动语义的实现原理:  
    &emsp;&emsp;考虑下面这种情况，我们有一个工厂，每次都会返回一个Test对象，但是由于Test()只在函数内起作用，因此返回后就会调用析构函数导致无法回传。

        ```cpp
        class Test {
            int * arr{nullptr};
        public:
            Test():arr(new int[5000]{1,2,3,4}) { 
            	cout << "default constructor" << endl;
            }
            Test(const Test & t) {
                cout << "copy constructor" << endl;
                if (arr == nullptr) arr = new int[5000];
                memcpy(arr, t.arr, 5000*sizeof(int));
            }
            ~Test(){
                cout << "destructor" << endl;
                delete [] arr;
            }
        }
        Test createTest() {
            return Test();
        }

        int main() {
            Test t(createTest());
        }
        ```
        这时候就用到了移动语义，修改其中构造函数。  

        ```cpp
        class Test {
            int * arr{nullptr};
        public:
            Test():arr(new int[5000]{1,2,3,4}) { 
            	cout << "default constructor" << endl;
            }

            Test(const Test & t) {
                cout << "copy constructor" << endl;
                if (arr == nullptr) arr = new int[5000];
                memcpy(arr, t.arr, 5000*sizeof(int));
            }
            // 构造函数修改
            Test(Test && t): arr(t.arr) {
                cout << "move constructor" << endl;
                t.arr = nullptr;
            }
            ~Test(){
                cout << "destructor" << endl;
                delete [] arr;
            }
        };
        ```
    2. 完美转发  
        我们考虑一个例子：

        ```cpp
        template <typename T>
        void func(T t) {
            cout << "in func" << endl;
        }

        template <typename T>
        void relay(T&& t) {
            cout << "in relay" << endl;
            func(t);
        }

        int main() {
            relay(Test());
        }
        ```

        &emsp;&emsp;在这个例子当中，我们的期待是，我们在main当中调用relay，Test的临时对象作为一个右值传入relay，在relay当中又被转发给了func，那这时候转发给func的参数t也应当是一个右值。也就是说，我们希望：当relay的参数是右值的时候，func的参数也是右值；当relay的参数是左值的时候，func的参数也是左值。
        运行结果：

        ```terminal
        default constructor
        in relay
        copy constructor
        in func
        destructor
        destructor
        ```
        &emsp;&emsp;我们看到，在relay当中转发的时候，调用了复制构造函数，也就是说编译器认为这个参数t并不是一个右值，而是左值。这个的原因已经在上一节将结果了，因为它有一个名字。那么如果我们想要实现我们所说的，如果传进来的参数是一个左值，则将它作为左值转发给下一个函数；如果它是右值，则将其作为右值转发给下一个函数，我们应该怎么做呢？  
        &emsp;&emsp;这时，我们需要std::forward<T>()。与std::move()相区别的是，move()会无条件的将一个参数转换成右值，而forward()则会保留参数的左右值类型。所以我们的代码应该是这样：  
        
        ```cpp
        template <typename T>
        void func(T t) {
            cout << "in func " << endl;
        }

        template <typename T>
        void relay(T&& t) {
            cout << "in relay " << endl;
            func(std::forward<T>(t));
        }
        ```
        

### 8. tuple元组
&emsp;&emsp;C++11标准新引入了一种类模版：tuple。特点为：实例化的对象可以存储任意数量、任意类型的数据。当需要存储多个不同类型的元素的时候，可以使用tuple；当函数需要返回多个数据的时候可以使用tuple存储多个返回值并进行返回。需要时网上自行查询。  
* 样例代码
    ```cpp
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
    ```  
### 9. 智能指针
* 简介  
&emsp;&emsp;在C++中没有垃圾回收机制，必须自己释放分配的内存，否则就会造成内存泄漏。解决这个问题最有效的方法是使用智能指针（smart pointer）。智能指针是存储指向动态分配（堆）对象指针的累，用于生存期的控制，能够确保在离开指针所在的作用域的时候，自动地销毁动态分配的对象，防止内存泄露。智能指针的核心实现技术是引用计数，每使用它一次，内部引用计数加1，，每析构一次内部的引用计数减1，减为0的时候，删除所指向的堆内存。  
* 共享指针分类  
    > C++11提供了三种智能指针，使用这些智能指针时需要引用头文件\<memory\> 
    
    * std::shared_ptr: 共享的智能指针
    * std::unique_str: 独占的智能指针
    * std::weak_ptr: 弱引用的智能指针，它不共享指针，不能操作资源，是用来监视shared_ptr的。  

* 分析  
    * shared_ptr
        1. 共享智能指针是指多个智能指针可以同时管理同一块有效的内存，共享智能指针shared_ptr 是一个模板类，如果要进行初始化有三种方式：通过构造函数、std::make_shared辅助函数以及reset方法。共享智能指针对象初始化完毕之后就指向了要管理的那块堆内存，如果想要查看当前有多少个智能指针同时管理着这块内存可以使用共享智能指针提供的一个成员函数use_count。  
        2. 对应基础数据类型来说，通过操作智能指针和操作智能指针管理的内存效果是一样的，可以直接完成数据的读写。但是如果共享智能指针管理的是一个对象，那么就需要取出原始内存的地址再操作，可以调用共享智能指针类提供的get()方法得到原始地址。  
        3. 当智能指针管理的内存对应的引用计数变为0的时候，这块内存就会被智能指针析构掉了。另外，我们在初始化智能指针的时候也可以自己指定删除动作，这个删除操作对应的函数被称之为删除器，这个删除器函数本质是一个回调函数，我们只需要进行实现，其调用是由智能指针完成的。  
        * 样例代码
            ```cpp
            #include <iostream>
            using namespace std;
            #include <string>
            #include <memory>

            class Test
            {
            public:
                Test() : m_num(0)
                {
                    cout << "construct Test..." << endl;
                }

                Test(int x) : m_num(0)
                {
                    cout << "construct Test, x = " << x << endl;
                }

                Test(string str) : m_num(0)
                {
                    cout << "construct Test, str = " << str << endl;
                }

                ~Test()
                {
                    cout << "destruct Test..." << endl;
                }

                void setValue(int v)
                {
                    this->m_num = v;
                }

                void print()
                {
                    cout << "m_num: " << this->m_num << endl;
                }

            private:
                int m_num;
            };

            int main()
            {
                /*--------------------------  一，初始化智能指针shared_ptr  ------------------------------*/
                //1.通过构造函数初始化
                shared_ptr<int> ptr1(new int(3));
                cout << "ptr1管理的内存引用计数: " << ptr1.use_count() << endl;

                //2.通过移动和拷贝构造函数初始化
                shared_ptr<int> ptr2 = move(ptr1);
                cout << "ptr1管理的内存引用计数: " << ptr1.use_count() << endl;
                cout << "ptr2管理的内存引用计数: " << ptr2.use_count() << endl;

                shared_ptr<int> ptr3 = ptr2;
                cout << "ptr2管理的内存引用计数: " << ptr2.use_count() << endl;
                cout << "ptr3管理的内存引用计数: " << ptr3.use_count() << endl;

                //3.通过 std::make_shared初始化
                shared_ptr<int> ptr4 = make_shared<int>(8);
                shared_ptr<Test> ptr5 = make_shared<Test>(7);
                shared_ptr<Test> ptr6 = make_shared<Test>("GOOD LUCKLY!");

                //4.通过reset初始化
                ptr6.reset(); //重置ptr6, ptr6的引用基数为0
                cout << "ptr6管理的内存引用计数: " << ptr6.use_count() << endl;

                ptr5.reset(new Test("hello"));
                cout << "ptr5管理的内存引用计数: " << ptr5.use_count() << endl;

                cout << endl;
                cout << endl;

                /*--------------------------  二，共享智能指针shared_ptr的使用  ------------------------------*/
                //1.方法一
                Test* t = ptr5.get();
                t->setValue(1000);
                t->print();

                //2.方法二
                ptr5->setValue(7777);
                ptr5->print();

                printf("\n\n");
                /*------------------------------------  三，指定删除器  -----------------------------------*/
                 //1.简单举例
                shared_ptr<Test> ppp(new Test(100), [](Test* t) {
                    //释放内存
                    cout << "Test对象的内存被释放了......." << endl;
                    delete t;
                    });
                printf("----------------------------------------------------------------------\n");

                2.如果是数组类型的地址，就需要自己写指定删除器，否则内存无法全部释放
                //shared_ptr<Test> p1(new Test[5], [](Test* t) {
                //    delete[]t;
                //    });

                //3.也可以使用c++给我们提供的 默认删除器函数（函数模板）
                shared_ptr<Test> p2(new Test[3], default_delete<Test[]>());

                //4.c++11以后可以这样写 也可以自动释放内存
                shared_ptr<Test[]> p3(new Test[3]);

                return 0;
            }
            ```
    * 独占的智能指针unique_ptr
        * 初始化  
        &emsp;&emsp;std::unique_ptr是一个独占型的智能指针，它不允许其他的智能指针共享其内部的指针，可以通过它的构造函数初始化一个独占智能指针对象，但是不允许通过赋值将一个unique_ptr赋值给另一个unique_ptr。  
        * 删除器  
        &emsp;&emsp; unique_ptr指定删除器和shared_ptr指定删除器是有区别的，unique_ptr指定删除器的时候需要确定删除器的类型，所以不能像shared_ptr那样直接指定删除器。  
        * 样例代码  
            ```cpp
            #include <iostream>
            using namespace std;
            #include <string>
            #include <memory>
            #include <functional>
            
            class Test
            {
            public:
                Test() : m_num(0)
                {
                    cout << "construct Test..." << endl;
                }
            
                Test(int x) : m_num(1)
                {
                    cout << "construct Test, x = " << x << endl;
                }
            
                Test(string str) : m_num(2)
                {
                    cout << "construct Test, str = " << str << endl;
                }
            
                ~Test()
                {
                    cout << "destruct Test..." << endl;
                }
            
                void setValue(int v)
                {
                    this->m_num = v;
                }
            
                void print()
                {
                    cout << "m_num: " << this->m_num << endl;
                }
            
            private:
                int m_num;
            };
            
            int main()
            {
                /*--------------------------  一，初始化智能指针unique_ptr  ------------------------------*/
                //1.通过构造函数初始化
                unique_ptr<int> ptr1(new int(3));
            
                //2.通过移动函数初始化
                unique_ptr<int> ptr2 = move(ptr1);
            
                //.通过reset初始化
                ptr2.reset(new int(7));
            
                /*--------------------------  二，unique_ptr的使用  ------------------------------*/
                //1.方法一
                unique_ptr<Test> ptr3(new Test(666));
                Test* pt = ptr3.get();
                pt->setValue(6);
                pt->print();
            
                //2.方法二
                ptr3->setValue(777);
                ptr3->print();
            
                /*------------------------------------  三，指定删除器  -----------------------------------*/
                //1.函数指针类型
                //using ptrFunc = void(*)(Test*);
                //unique_ptr<Test, ptrFunc> ptr4(new Test("hello"), [](Test* t) {
                //    cout << "-----------------------" << endl;
                //    delete t;
                //    });
            
                //2.仿函数类型（利用可调用对象包装器）
                unique_ptr<Test, function<void(Test*)>> ptr4(new Test("hello"), [](Test* t) {
                    cout << "-----------------------" << endl;
                    delete t;
                    });
            
                /*---------- 四，独占(共享)的智能指针可以管理数组类型的地址，能够自动释放 ---------*/
                unique_ptr<Test[]> ptr5(new Test[3]);
            
                //在c++11中shared_ptr不支持下面的写法，c++11以后才支持的
                shared_ptr<Test[]> ptr6(new Test[3]);
            
                return 0;
            }
            ```
    * 弱引用的智能指针weak_ptr  
        &emsp;&emsp;弱引用智能指针std::weak_ptr可以看做是shared_ptr的助手，它不管理shared_ptr内部的指针。std::weak_ptr没有重载操作符*和->，因为它不共享指针，不能操作资源，所以它的构造不会增加引用计数，析构也不会减少引用计数，它的主要作用就是作为一个旁观者监视shared_ptr中管理的资源是否存在。  
        * 初始化  
            ```cpp
            #include <iostream>
            #include <memory>
            using namespace std;

            int main() 
            {
                shared_ptr<int> sp(new int);

                weak_ptr<int> wp1;
                weak_ptr<int> wp2(wp1);
                weak_ptr<int> wp3(sp);
                weak_ptr<int> wp4;
                wp4 = sp;
                weak_ptr<int> wp5;
                wp5 = wp3;

                return 0;
            }
            ```
        * 常用函数  
            a. 通过调用expired()方式来判断观测的资源是否已经被释放  
            b. 通过调用lock()方法来获取管理所检测资源的shared_ptr对象  
            c. 通过调用reset()方法来清空对象，使其不监测任何资源
 


## 参考资料
[C语言中文网](http://c.biancheng.net/cplus/11/)  
[CSDN:C++11常用新特性快速一览](https://blog.csdn.net/jiange_zh/article/details/79356417)  
[C++11特性—右值引用](https://blog.csdn.net/gls_nuaa/article/details/126134537)  
[C++11智能指针](https://blog.csdn.net/qq_56673429/article/details/124837626)  
