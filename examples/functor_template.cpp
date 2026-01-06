#include <iostream>

struct Printer {
    template<typename T>
    void operator()(T v) const {
        std::cout << "Printer: " << v << '\n';
    }
};

template<typename F>
void do_prints(F f) {
    f(1);
    f(2.5);
    f("three");
}

int main() {
    Printer p;
    do_prints(p);
}

