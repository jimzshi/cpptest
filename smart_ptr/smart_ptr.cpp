#include <memory>
#include <iostream>
#include <functional>

using namespace std;

struct Base : 
    enable_shared_from_this<Base> 
{
    Base() = default;
    virtual ~Base() = default;
    
    void foo() const { cout << "Base: foo()" << endl; }

};

struct Derived : 
    public Base
//  ,public enable_shared_from_this<Derived>    
// can't have this 'enable' class for both Base and Derived at the same time
{
    Derived() = default;
    virtual ~Derived() = default;

    void foo() const { cout << "Derived foo()" << endl; }
    void bar() const { cout << "Derived bar()" << endl; }
};

void call_foo(weak_ptr<Base> p) {
    if (auto pb = p.lock()) {
        cout << "call_foo: " << p.use_count() << endl;
        pb->foo();
    }
    else {
        cout << "call_foo: bad weak ptr" << endl;
    }
}

void call_bar(weak_ptr<Derived> p) {
    if (auto pb = p.lock()) {
        cout << "call_bar: " << p.use_count() << endl;
        pb->bar();
    }
    else {
        cout << "call_bar: bad weak ptr" << endl;
    }
}

int main() {
    auto spB = make_shared<Base>();
    auto spD = make_shared<Derived>();
    call_foo(spB->shared_from_this());
    call_foo(spB);
    call_foo(spD->shared_from_this());
    call_foo(spD);
    //call_bar(spB);
    call_bar(spD);
    // call_bar(spD->shared_from_this());  //return is Base type;
    weak_ptr<Derived> pD_empty;
    call_bar(pD_empty);

    function<void(void)> spB_foo = bind(&Base::foo, spB);
    spB_foo();
    function<void(void)> spD_B_foo = bind(&Base::foo, spD); //call base version, because it's not virtual;
    spD_B_foo();
    function<void(void)> spD_D_foo = bind(&Derived::foo, spD);
    spD_D_foo();
    //function<void(void)> spB_D_foo = bind(&Derived::foo, spB); //can't invoke
    //spB_D_foo();
    function<void(void)> spD_bar = bind(&Derived::bar, spD);
    spD_bar();

    return 0;
}