// std
#include <cstdio>
#include <experimental/propagate_const>
#include <iostream>
#include <memory>
// myhead
#include "../myhead_file/myfmt.hpp"

using namespace my;

// 实现与定义分离
// 工厂模式

class PImplTest {
    struct Impl;
    std::experimental::propagate_const<std::unique_ptr<Impl>> p_impl;

  public:
    PImplTest() = default;
    PImplTest(int n) : p_impl{std::make_unique<Impl>(n)} {}
    PImplTest(PImplTest &&) = default;
    PImplTest(const PImplTest &) = default;
    PImplTest &operator=(PImplTest &&) = default;
    PImplTest &operator=(const PImplTest &) = default;
    ~PImplTest() = default;
    bool TODO() const { return true; }
    void DoSomething();

  private:
};

struct PImplTest::Impl {
    int x = 0;
    void DoSomething(const PImplTest &PImpl) {
        if (PImpl.TODO())
            "这是x:{}\n"_f(x);
    }
    Impl(int n) : x(n) {}
};

void PImplTest::DoSomething() {
    p_impl->DoSomething(*this);
    return;
}

int main(int argc, char *argv[]) {

    PImplTest ptest(2);

    ptest.DoSomething();

    return 0;
}
