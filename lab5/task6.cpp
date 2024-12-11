#include <iostream>

class Vector {
    double* _data;
    size_t _size;
    size_t _capacity;

public:

    class Iterator {
        double* ptr;
    public:
        explicit Iterator(double* ptr_) : ptr(ptr_) {}

        double operator*() {
            return *ptr;
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        Iterator& operator--() {
            --ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --ptr;
            return temp;
        }

        Iterator operator+(size_t offset) const {
            return Iterator(ptr + offset);
        }

        Iterator operator-(size_t offset) const {
            return Iterator(ptr - offset);
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
    };

    Iterator begin() {
        return Iterator(_data);
    }

    Iterator end() {
        return Iterator(_data + _size);
    }

    Vector() : _data(nullptr), _size(0), _capacity(0) {}

    explicit Vector(size_t cnt, double val = 0) : _size(cnt), _capacity(cnt) {
        _data = new double[_capacity];
        std::fill(_data, _data + _size, val);
    }

    template <typename Iterator>
    Vector(Iterator lt, Iterator rt) {
        _size = std::distance(lt, rt);
        _capacity = _size;
        _data = new double[_capacity];
        std::copy(lt, rt, _data);
    }

    Vector(std::initializer_list<double> vals) : Vector(vals.begin(), vals.end()) {}

    Vector(const Vector &other) : _size(other._size), _capacity(other._capacity) {
        _data = new double[_capacity];
        std::copy(other._data, other._data + other._size, _data);
    }

    ~Vector() {
        delete[] _data;
    }


    double& at(size_t ind) {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range.\n");
        }
        return _data[ind];
    }

    const double& at(size_t ind) const {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range.\n");
        }
        return _data[ind];
    }

    double& front() {
        return at(0);
    }

    const double& front() const {
        return at(0);
    }

    double& back() {
        return at(_size - 1);
    }

    const double& back() const {
        return at(_size - 1);
    }

    double* data() {
        return _data;
    }

    const double* data() const {
        return _data;
    }

    bool empty() const {
        return _size == 0;
    }

    size_t size() const {
        return _size;
    }

    void reserve(size_t num) {
        if (num > _capacity) {
            _capacity = num;
            double* new_data = new double[_capacity];
            std::copy(_data, _data + _size, new_data);
            delete[] _data;
            _data = new_data;
        }
    }

    size_t capacity() const {
        return _capacity;
    }

    void shrink_to_fit() {
        if (_capacity > _size) {
            _capacity = _size;
            double* new_data = new double[_capacity];
            std::copy(_data, _data + _size, new_data);
            delete[] _data;
            _data = new_data;
        }
    }

    void clear() {
        _size = 0;
    }

    void insert(size_t ind, double el) {
        if (ind > _size) {
            throw std::out_of_range("Index out of range.\n");
        }
        reserve((_capacity == 0) ? 1 : _size * 2);
        std::move_backward(_data + ind, _data + _size, _data + _size + 1);
        _data[ind] = el;
        ++_size;
    }

    void erase(size_t ind) {
        if (ind >= _size) {
            throw std::out_of_range("Index out of range.\n");
        }
        std::move(_data + ind + 1, _data + _size, _data + ind);
        --_size;
    }

    void push_back(double el) {
        reserve((_capacity == 0) ? 1 : _size * 2);
        _data[_size++] = el;
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty.\n");
        }
        --_size;
    }

    void resize(size_t new_size, double el = 0) {
        if (new_size > _size) {
            reserve(new_size);
            std::fill(_data + _size, _data + new_size, el);
        }
        _size = new_size;
    }

    std::partial_ordering operator<=>(const Vector& other) const {
        if (_size != other._size) {
            return _size <=> other._size;
        }
        if (_size == 0) {
            return std::strong_ordering::equal;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) {
                return _data[i] <=> other._data[i];
            }
        }
        return std::strong_ordering::equal;
    }

    bool operator==(const Vector& other) const {
        return (_size == other._size) && std::equal(_data, _data + _size, other._data);
    }

    Vector& operator= (const Vector& other) {
        if (*this == other) {
            return *this;
        }
        clear();
        reserve(other._capacity);
        _size = other._size;
        _capacity = other._capacity;
        std::copy(other._data, other._data + other._size, _data);
        return *this;
    }
};


int main() {
    try {
        Vector v = {1, 2, 3};

        std::cout << v.at(0) << std::endl;
        std::cout << v.at(1) << std::endl;
        std::cout << v.at(2) << std::endl;

        Vector b = v;

        v.clear();

        v.push_back(1);
        v.insert(1, 52);
        v.insert(0, 16);
        v.insert(2, 95);
        v.erase(1);
        v.insert(3, 89);
        v.pop_back();
        v.resize(5);
        v.resize(7, 12345);

        for (const auto& elem : v) {
            std::cout << elem << " ";
        }
        std::cout << "\n\n";
        for (const auto& elem : b) {
            std::cout << elem << " ";
        }
        std::cout << "\n\n";
        std::cout << (v == b) << '\n';
        std::cout << (v != b) << '\n';
        std::cout << (v < b) << '\n';
        std::cout << (v > b) << '\n';

        b = v;
        for (const auto& elem : b) {
            std::cout << elem << " ";
        }
        return 0;
    }

    catch (const std::out_of_range& e) {
        std::cout << "Vector: index out of range" << std::endl;
    }

}