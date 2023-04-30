#include "cache-int.hpp"
#include <iostream>
#include <deque>
// A very small amount of tests

#define MAKE_TEST(name, code) bool RUN_TEST_ ## name ## __ () { {code} return true; }
#define ASSERT(cond) if (!(cond)) { return false; }
#define RUN_TEST(name)\
    if (RUN_TEST_ ## name ## __ ()) {\
        printf("\033[;32mTest %s passed!\033[0m\n", #name);\
    } else {\
        printf("\033[;31mTest %s FAILED!\033[0m\n", #name);\
    }


MAKE_TEST(trivial, 
    ASSERT(1 + 1 == 2);
)


static std::deque<int> args_of_foo;

int foo(int i) {
    args_of_foo.push_back(i);
    return 0;
};

int foo2(int i, int j) {
    args_of_foo.push_back(i);
    return 0;
};

MAKE_TEST(one_arg,
    ReturnValueCache foo_cached(foo);
    foo_cached(5);
    ASSERT(args_of_foo.back() == 5);
    foo_cached(7);
    ASSERT(args_of_foo.back() == 7);
    foo_cached(5);
    ASSERT(args_of_foo.back() == 7);
    foo_cached.drop();
    foo_cached(7);
    ASSERT(args_of_foo.back() == 7);
    args_of_foo.clear();
)

MAKE_TEST(two_args,
    ReturnValueCache foo_cached2(foo2);
    ReturnValueCache foo_cached1 = foo_cached2;
    ReturnValueCache foo_cached = std::move(foo_cached1);
    foo_cached(5,1);
    ASSERT(args_of_foo.back() == 5);
    foo_cached(7,1);
    ASSERT(args_of_foo.back() == 7);
    foo_cached(5,1);
    ASSERT(args_of_foo.back() == 7);
    foo_cached.drop();
    foo_cached(7,1);
    ASSERT(args_of_foo.back() == 7);
    args_of_foo.clear();
)

int main() {
    RUN_TEST(one_arg);
    RUN_TEST(two_args);
}