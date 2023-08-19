#ifndef INCLUDE__TEST__HPP
#define INCLUDE__TEST__HPP
#include <log/log.hpp>

class Test {
  public:
    void assert(bool is_true_ok) {
        if (!is_true)
            logf("有问题");
    }

  private:
};

#endif // !INCLUDE__TEST__HPP
