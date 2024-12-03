#include <iostream>

int increment(int num) {
    int mask = 1;
    while ((num & mask) != 0) {
        num = num ^ mask;
        mask = mask<<1;
    }
    num = num ^ mask;
    return num;
}

int add(int a, int b) {
    int zapomnil = 0, ans = 0;
    int len = sizeof(int) << 3;
    for (int i = 0; i < len; i = increment(i)) {
        int up = ((a & (1 << (i))) >> (i));
        int down = ((b & (1 << (i))) >> (i));

        int cur = (up ^ down ^ zapomnil);
        zapomnil = (zapomnil == 0) ? (up & down) : (up | down);
        ans = ans | (cur << (i));
    }
    return ans;
}

int minus(int a, int b) {
    return add(a, add(~b, 1));
}

int mult(int a, int b) {
    int ans = 0;
    if (a < b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    int i = 0;
    while (b != 0) {
        ans = (b & 1) ? add(ans, (a << (i))) : ans;
        b = b >> 1;
        i = increment(i);
    }
    return ans;
}

class binary_int {
    int num;
public:
    binary_int() : num(0) {}
    explicit binary_int(const int val) : num(val) {}
    binary_int(const binary_int& old) { num = old.num; }
    ~binary_int () = default;

    binary_int& operator- () {
        num = add(~num, 1);
        return *this;
    }
    binary_int& operator++ () {
        num = increment(num);
        return *this;
    }
    binary_int operator++ (int) {
        binary_int tmp = *this;
        ++*this;
        return tmp;
    }
    binary_int& operator-- () {
        num = minus(num, 1);
        return *this;
    }
    binary_int operator-- (int) {
        binary_int tmp = *this;
        num = minus(num, 1);
        return tmp;
    }
    binary_int& operator+= (const binary_int& other) {
        num = add(num, other.num);
        return *this;
    }
    binary_int operator+ (const binary_int& other) const {
        binary_int ans = binary_int{num};
        ans += other;
        return ans;
    }
    binary_int& operator-= (const binary_int& other) {
        num = minus(num, other.num);
        return *this;
    }
    binary_int operator- (const binary_int& other) const {
        binary_int ans = binary_int{num};
        ans -= other;
        return ans;
    }
    binary_int& operator*= (const binary_int& other) {
        num = mult(num, other.num);
        return *this;
    }
    binary_int operator* (const binary_int& other) const {
        binary_int ans = binary_int{num};
        ans *= other;
        return ans;
    }

    binary_int& operator<<= (const int a) {
        num <<= a;
        return *this;
    }
    binary_int& operator>>= (const int a) {
        num >>= a;
        return *this;
    }

    binary_int operator>> (const int a) const {
        binary_int ans = binary_int{num};
        ans >>= a;
        return ans;
    }
    binary_int operator<< (const int a) const {
        binary_int ans = binary_int{num};
        ans <<= a;
        return ans;
    }

    binary_int& operator= (const binary_int& other) {
        num = other.num;
        return *this;
    }
    friend std::ostream& operator << (std::ostream &out, const binary_int& a);
    friend std::pair<binary_int, binary_int> split(const binary_int& obj);
};


int PowOf2(int num) {
    int p = 0;
    if (num < 0) {
        return sizeof(int) << 3;
    }
    if (num == 0) {
        return 1;
    }

    while (num) {
        num >>= 1;
        p = increment(p);
    }
    return p;
}

std::ostream& operator << (std::ostream &out, const binary_int& a) {
    const int len = minus(PowOf2(a.num), 1);
    for (int i = len; i >= 0; i = minus(i, 1)) {
        out << ((a.num>>i) & 1);
    }
    return out;
}

std::pair<binary_int, binary_int> split(const binary_int& obj) {
    binary_int low(0);
    binary_int top(0);
    const int len = PowOf2(obj.num);
    const int halfLen = (len & 1) ? add(len>>1, 1): len>>1;

    for (int i = 0; i < halfLen; i = increment(i)) {
        low += binary_int{((obj.num>>i) & 1) << i};
    }
    for (int i = halfLen; i <= len; i = increment(i)) {
        top += binary_int{((obj.num>>i) & 1) << i};
    }
    return {top, low};
}

int main() {
    binary_int a(13);
    binary_int b(52);
    binary_int c;

    c = b - a;
    std::cout << b-- << " " << a << " = " << c << std::endl;
    std::cout << b;
}