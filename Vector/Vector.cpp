
#include <iostream>
#include <memory>


using namespace std;

template<typename T>
class Vector {
public:
    Vector(int size = 10) {
        _first = alloc.allocate(10);
        _last = _first;
        _end = _first + size;
    }
    ~Vector() {
        for (auto c = _first; c != _last; c++) {
            alloc.destroy(c);
        }
        alloc.deallocate(_first, _end - _first);
        _first = _last = _end = nullptr;  //指针置空
    }
    Vector(const Vector& other) {
        int len = other._end - other._first;
        int size = other._last - other._first;       
        _first = alloc.allocate(len);
        for (int i = 0; i < len; i++) {
            alloc.construct(_first[i], other._first[i]);
        }
        _first = other._first;
        _last = other._last;
        _end = other._end;
    }
    Vector operator = (const Vector& other) {
        if (this == &other) {
            return *this;
        }
        for (auto c = _first; c != _last; c++) {
            alloc.destroy(c);
        }
        int size = other._last - other._first;
        int len = other._end - other._first;
        alloc.deallocate(_first, _end - _first);
        _first = alloc.allocate(len);
        for (int i = 0; i < size; i++) {
            alloc.construct(_first[i], other._first[i]);
        }
        _first = other._first;
        _last = other._last;
        _end = other._end;
        return *this;
    }
    int size() const {
        return _last - _first;
    }
    bool isFull()const {
        return _last == _end;
    }
    bool isEmpty()const {
        return _first == _last;
    }
    void push_back(const T& val) {
        if (isFull()) expand();
        *_last = val;
        _last++;
    }
    void pop_back() {
        if (isEmpty()) return;
        alloc.destroy(_last);
        _last--;
    }
    T operator [](int i) {
        return _first[i];
    }
    T* begin() {
        return _first;
    }
    T* end() {
        return _last;
    }
    //以下迭代器内容
    class iterator {
    public:
        iterator(T* p = nullptr) : _p(p) {};
        bool operator != (T* other)const {
            return _p != other;
        }
        void operator ++() {
            _p++;
        }
        T operator *() {
            return *_p;
        }

    private:
        T* _p;
    };


private:
    T* _first;
    T* _last;
    T* _end;
    allocator<T> alloc;
    void expand() {
        int len = _end - _first;
        int size = _last - _first;
        T* temp = alloc.allocate(2 * len);
        for (int i = 0; i < size; i++) {
            alloc.construct(temp + i, _first[i]);
           //alloc.destroy(_first[i]);           
        }
        for (T* p = _first; p != _last; ++p)
        {
            alloc.destroy(p);
        }
        alloc.deallocate(_first, len);
        _first = _last = _end = nullptr;
        _first = temp;
        _end = _first + 2 * size;
        _last = _first + size;
    }
};
int main()
{
    Vector<int> v;
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.pop_back();
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it;
    }
}
