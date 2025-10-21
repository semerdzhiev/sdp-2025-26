#include <iostream>
#include <string>

using std::cout;
using std::endl;

template <typename T>
T defaultFunc(T value) {
    return value;
}

template <typename T>
class Iterator {
public:
    Iterator(T* start) {
        _current = start;
    }

    Iterator<T>& operator++() {
        ++_current;
        return *this;
    }

    const T& operator*() const{
        return *_current;
    }

    bool operator!=(const Iterator& other) const {
        return _current != other._current;
    }

private:
    T* _current;
};

template <class T>
class Container;

template <typename T>
class MapIterator {
public:
    static MapIterator<T> mend(Container<T>* cont, T (*func)(const T)) {
        MapIterator<T> it(cont, func);
        it._current = cont->_data + cont->_size;
        return it;
    }

    MapIterator(Container<T>* cont, T (*func)(T)) {
        this->func = func;
        _current = cont->_data;
        _end = cont->_data + cont->_size;
    }

    MapIterator<T>& operator++() {
        ++_current;
        return *this;
    }

    T operator*() const{
        return func(*_current);
    }

    bool operator!=(const MapIterator& other) const {
        return _current != other._current;
    }

private:
    T (*func)(T);
    T* _current;
    T* _end;
};


template <typename T>
class ReverseIterator {
private:
    T* _current;
public:
    ReverseIterator(T* start) {
        _current = start;
    }

    ReverseIterator<T>& operator++() {
        --_current;
        return *this;
    }

    const T& operator*() const{
        return *_current;
    }

    bool operator!=(const ReverseIterator& other) const {
        return _current != other._current;
    }
};

template <class T>
class Container {
private:
    T* _data;
    int _size;

public:
    friend class MapIterator<T>;
    static const int DEFAULT_SIZE = 10;

    Container(int size=DEFAULT_SIZE) : _size(size) {
        _data = new T[_size];
    }

    ~Container() {
        delete[] _data;
    }

    T& operator[](int index) {
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    Iterator<T> begin() {
        return Iterator<T>(_data);
    }

    Iterator<T> end() {
        return Iterator<T>(_data + _size);
    }

    ReverseIterator<T> rbegin() {
        return ReverseIterator<T>(_data + _size - 1);
    }
    
    ReverseIterator<T> rend() {
        return ReverseIterator<T>(_data - 1);
    }
};

class Data {
public:
    Data(int a = 0) : m_data_(a) {}

    void set_data(int a) {
        m_data_ = a;
    }

    int data() {
        return m_data_;
    }

    const int data() const {
        return m_data_;
    }

private:
    int m_data_;
};

Data toSixthPower(const Data d) {
    int val = d.data();
    // Simple implementation for demonstration
    int sixth_power = val * val * val * val * val * val;

    return Data(sixth_power);
}

int main() {
    Container<Data> container(5);

    for (int i = 0; i < 5; ++i) {
        container[i].set_data(i * 10);
    }

    cout << "Range iterator:" << std::endl;

    // Range iterator, uses begin() and end()
    for (const Data& d : container) {
        std::cout << d.data() << " ";
    }

    cout << std::endl  << "Forward iteration:" << std::endl;
    
    for (Iterator<Data> it = container.begin(); it != container.end(); ++it) {
        std::cout << (*it).data() << " ";
    }

    cout << std::endl << "Reverse iterator:" << std::endl;
    
    for (ReverseIterator<Data> it = container.rbegin(); it != container.rend(); ++it) {
        std::cout << (*it).data() << " ";
    }

    cout << std::endl << "Map iterator:" << std::endl;

    MapIterator<Data> begin(&container, &toSixthPower);
    MapIterator<Data> end = MapIterator<Data>::mend(&container, &toSixthPower);

    for (MapIterator<Data> it = begin; it != end; ++it) {
        std::cout << (*it).data() << " ";
    }
}