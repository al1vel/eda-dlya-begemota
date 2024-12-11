#include <iostream>
#include <cmath>

class ComplexNum {
    double real;
    double imag;
public:
    ComplexNum() : real(0), imag(0) {}

    ComplexNum(const double r, const double i) : real(0), imag(0) {
        real = r;
        imag = i;
    }

    ComplexNum(const ComplexNum &num) {
        real = num.real;
        imag = num.imag;
    }

    ~ComplexNum() = default;

    ComplexNum operator+ (ComplexNum& a) const {
        double r, i;
        r = real + a.real;
        i = imag + a.imag;
        return ComplexNum{r, i};
    }

    ComplexNum operator- (ComplexNum& a) const {
        double r, i;
        r = real - a.real;
        i = imag - a.imag;
        return ComplexNum{r, i};
    }

    ComplexNum operator* (ComplexNum& a) const {
        double r, i;
        r = real * a.real - imag * a.imag;
        i = imag * a.real + a.imag * real;
        return ComplexNum{r, i};
    }

    ComplexNum operator/ (ComplexNum& a) const {
        double r, i, divider;
        divider = a.real * a.real + a.imag * a.imag;
        r = (real * a.real + imag * a.imag) / divider;
        i = (imag * a.real - a.imag * real) / divider;
        return ComplexNum{r, i};
    }

    ComplexNum comp_sum(ComplexNum& a) {
        double r, i;
        r = real + a.real;
        i = imag + a.imag;
        return ComplexNum{r, i};
    }

    ComplexNum comp_dif (ComplexNum& a) const {
        double r, i;
        r = real - a.real;
        i = imag - a.imag;
        return ComplexNum{r, i};
    }

    ComplexNum comp_mult (ComplexNum& a) const {
        double r, i;
        r = real * a.real - imag * a.imag;
        i = imag * a.real + a.imag * real;
        return ComplexNum{r, i};
    }

    ComplexNum comp_div (ComplexNum& a) const {
        double r, i, divider;
        divider = a.real * a.real + a.imag * a.imag;
        r = (real * a.real + imag * a.imag) / divider;
        i = (imag * a.real - a.imag * real) / divider;
        return ComplexNum{r, i};
    }

    double module() const {
        double len = sqrt(real * real + imag * imag);
        return len;
    }

    double argument() const {
        if (real == 0) {
            if (imag == 0) {
                std::cout << "The argument for the null vector is not defined.\n";
                return -1;
            }
            return ((imag < 0) ? -1 : 1) * asin(1);
        }
        double arg = atan(imag / real);
        return arg;
    }

    friend std::ostream& operator << (std::ostream &out, const ComplexNum& a);
};

std::ostream& operator << (std::ostream &out, const ComplexNum& a) {
    out << a.real << " + " << a.imag << "i";
    return out;
}

int main() {
    ComplexNum a(1.2, 8.4);
    ComplexNum b(5.131, 123.5);

    std::cout << a.comp_div(b) << '\n';
    std::cout << a.comp_dif(b) << '\n';
    std::cout << a.comp_sum(b) << '\n';
    std::cout << a.comp_mult(b) << '\n';
}