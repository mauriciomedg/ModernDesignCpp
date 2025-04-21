#pragma once

#include <iostream>
#include <memory>

struct Test {
    Test() { std::cout << "Construct\n"; }
    ~Test() { std::cout << "Destruct\n"; }

    Test(const Test&) { std::cout << "Copy Construct\n"; }
    Test(Test&&) noexcept { std::cout << "Move Construct\n"; }
};

void process(std::shared_ptr<Test> ptr) {
    std::cout << "In process, use_count = " << ptr.use_count() << "\n";
}

int runSmart() {
    std::shared_ptr<Test> a = std::make_shared<Test>();
    std::cout << "Before process, use_count = " << a.use_count() << "\n";
    process(std::move(a));
    std::cout << "After process, use_count = " << a.use_count() << "\n";
    return 0;
}