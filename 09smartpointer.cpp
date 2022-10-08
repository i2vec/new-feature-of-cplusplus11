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

    //2.如果是数组类型的地址，就需要自己写指定删除器，否则内存无法全部释放
    shared_ptr<Test> p1(new Test[5], [](Test* t) {
       delete[]t;
       });

    //3.也可以使用c++给我们提供的 默认删除器函数（函数模板）
    shared_ptr<Test> p2(new Test[3], default_delete<Test[]>());

    //4.c++11以后可以这样写 也可以自动释放内存
    shared_ptr<Test[]> p3(new Test[3]);

    return 0;
}