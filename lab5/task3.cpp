#include <iostream>

int PowOf2(unsigned int base) {
    int p = 0;
    if (base == 0) return 1;
    while (base) {
        base >>= 1;

        p++;
    }
    return p;
}

class logical_values_array {
    unsigned int value;
public:
    logical_values_array() : value(0) {}
    explicit logical_values_array(const unsigned int val = 0) : value(val) {}

    logical_values_array inversion() const{
        return logical_values_array(~value);
    }

    logical_values_array conjunction(const logical_values_array& a) const{
        return logical_values_array{value & a.value};
    }

    logical_values_array disjunction(logical_values_array& a) const{
        return logical_values_array{value | a.value};
    }

    logical_values_array implication(logical_values_array& a) const{
        return logical_values_array{(~value) | a.value};
    }

    logical_values_array coimplication(logical_values_array& a) const{
        return logical_values_array{value | (~a.value)};
    }

    logical_values_array XOR(logical_values_array& a) const{
        return logical_values_array{value ^ a.value};
    }

    logical_values_array equivalence(logical_values_array& a) const{
        return logical_values_array{~(value ^ a.value)};
    }

    logical_values_array Piers(logical_values_array& a) const{
        return logical_values_array{~(value | a.value)};
    }

    logical_values_array Sheffer(logical_values_array& a) const{
        return logical_values_array{~(value & a.value)};
    }

    static logical_values_array equals(logical_values_array& a, logical_values_array& b) {
        return a.equivalence(b);
    }

    logical_values_array getBit(unsigned int ind) const{
        return logical_values_array{(value >> ind) & 1};
    }

    unsigned int get_val() const{
        return value;
    }

    friend std::ostream& operator << (std::ostream &out, const logical_values_array& a);
};

std::ostream& operator << (std::ostream &out, const logical_values_array& a) {
    int len = PowOf2(a.value) - 1;
    for (int i = len; i >= 0; i--) {
        out << ((a.value>>i) & 1);
    }
    return out;
}

int main() {
    logical_values_array a(12);
    logical_values_array b(10);

    std::cout << a.inversion() << '\n';
    std::cout << a.conjunction(logical_values_array(6)) << '\n';
    std::cout << a.disjunction(b) << '\n';
    std::cout << a.implication(b) << '\n';
    std::cout << a.coimplication(b) << '\n';
    std::cout << a.XOR(b) << '\n';
    std::cout << a.equivalence(b) << '\n';
    std::cout << a.Piers(b) << '\n';
    std::cout << a.Sheffer(b) << '\n';
    std::cout << logical_values_array::equals(a, b) << '\n';
    std::cout << a.get_val() << '\n';
    std::cout << a.getBit(3) << '\n';
}