#include <iostream>

template<typename F>
void use_for_int_and_double(F f) {
    // Let the compiler instantiate operator() for each call
    f(10);      // instantiates operator()<int>
    f(3.14);    // instantiates operator()<double>
}

int main() {
    auto g = [](auto x) {
        std::cout << "called with: " << x << "\n";
    };

    use_for_int_and_double(g);
    return 0;
}

