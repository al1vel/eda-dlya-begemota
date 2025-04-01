#include <exception>

class Summator {
public:
    int sum(int a, int b) {
        return a + b;
    }

    int div(int a, int b) {
        if (b == 0) {
            throw std::exception();
        }
        return a / b;
    }
};
