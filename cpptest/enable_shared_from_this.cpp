#include <memory>
#include <iostream>

using namespace std;

struct A : std::enable_shared_from_this<A> {
    int a = 10;
};

struct B : std::enable_shared_from_this<A> {
    int b = 200;
};

int main() {
    auto a = std::make_shared<A>();
    auto ap = a->shared_from_this();
    cout << "ap count: " << ap.use_count() << endl;
    
    B* b = new B();
    auto bp = b->shared_from_this();
    cout << "bp count: " << bp.use_count() << endl;
    delete b;
    cout << "bp count: " << bp.use_count() << endl;
    return 0;
}