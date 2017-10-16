
#include <cstddef>
#include <memory>
#include <utility>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <string>

template <typename T> class slot_map;
template <typename T> std::ostream& operator<<(std::ostream&, const slot_map<T>&);

struct Foo {
    int value;
    Foo() = default;
    Foo(int value) : value(value) { }
    Foo(Foo&& rhs) : value(rhs.value) { rhs.value = 0; /*std::cout << "move ctor\n";*/ }
    Foo& operator=(Foo&& rhs) { value = rhs.value; rhs.value = 0; /*std::cout << "move " << value << '\n';*/ return *this; }
    ~Foo() { std::cout << "destroy " << value << '\n'; }
};

std::ostream& operator<<(std::ostream& os, const Foo& foo) {
    os << foo.value;
    return os;
}

template <typename T>
class slot_map {
public:
    typedef size_t size_type;
    struct key {
        size_type index;
        size_type generation;
    };
public:
    slot_map() : slot_map(16) { }
    slot_map(size_type capacity) {
        data.reserve(capacity);
        slots.reserve(capacity);
        for (size_type i = 0; i < capacity; ++i) {
            slots.push_back({i+1,0});
        }
    }
    key insert(T obj) {
        auto capacity = data.capacity();
        data.push_back(std::move(obj));
        if (capacity < data.capacity()) {
            for (size_type i = capacity; i < data.capacity(); ++i) {
                slots.push_back({i+1,0});
            }
        }
        auto slot = slots[free_list_head];
        std::swap(slot.index, free_list_head);
        slots[slot.index].index = data.size() - 1;
        return slot;
        
    }
    T* operator[](key key) {
        auto slot = slots[key.index];
        return slot.generation == key.generation ? data[slot.index] : nullptr;
    }
    void erase(key key) {
        auto& slot = slots[key.index];
        if (slot.generation == key.generation) {
            slot.generation++; // ABA
            auto to_remove = std::move(data[slot.index]);
            if (data.size() > 1) {
                data[slot.index] = std::move(data.back());
                slots[data.size()-1].index = slot.index;
                slot.index = free_list_head;
                free_list_head = key.index;
            }
            data.pop_back();
        }
    }
    friend std::ostream& operator<< <> (std::ostream&, const slot_map&);
    friend std::ostream& operator<<(std::ostream& os, const slot_map::key& key) {
        os << key.index << ':' << key.generation;
        return os;
    }
private:
    size_type free_list_head = 0;
    std::vector<key> slots;
    std::vector<T> data;
};

template <typename T> std::ostream& operator<<(std::ostream& os, const slot_map<T>& m) {
    os << "capacity=" << m.data.capacity() << ",size=" << m.data.size() << ",free=" << m.free_list_head;
    if (m.data.size() > 0) {
        os << " data=[" << m.data[0];
        if (m.data.size() > 1) {
            for (typename slot_map<T>::size_type i = 1; i < m.data.size(); ++i) {
                os << ',' << m.data[i];
            }
        }
        os << ']';
    }
    return os;
}



template <int size> struct matrix {
    typedef std::string type;
    std::string name;
    std::array<std::array<type,size>,size> data;
    matrix(std::string name) : name(std::move(name)) { }
    const type& get(int row, int col) const {
        return data[row-1][col-1];
    }
    type& get(int row, int col) {
        return data[row-1][col-1];
    }
};

template <int size> std::ostream& operator<<(std::ostream& os, const matrix<size>& m) {
    for (int row = 1; row <= size; ++row) {
        for (int col = 1; col < size; ++col) {
            std::cout << m.get(row,col) << ", ";
        }
        std::cout << m.get(row,size) << '\n';
    }
    return os;
}

template <int size> matrix<size> make(int ax, int ay, int bx, int by, std::string name = "") {
    matrix<size> m(std::move(name));
    for (int row = 1; row <= size; ++row) {
        for (int col = 1; col <= size; ++col) {
            std::ostringstream s;
            for (int k = 1; k <= size; ++k) {
                s << 'A' << (row+ay) << (k+ax) << '*' << 'B' << (k+by) << (col+bx);
                if (k < size) s << " + ";
            }
            m.get(row,col) = s.str();
        }
    }
    return m;
}

template <int size> void replace(matrix<size>& m, const std::string& from, const std::string& name) {
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            auto& str = m.get(i,j);
            size_t pos = str.find(from);
            if (pos != std::string::npos) {
                str.replace(pos, from.length(), name);
            }
        }
    }
}

template <int size1, int size2> void replace(matrix<size1>& m1, const matrix<size2>& m2) {
    for (int i = 1; i <= size2; ++i) {
        for (int j = 1; j <= size2; ++j) {
            std::ostringstream s;
            s << m2.name;
            s << '[' << i << ',' << j << ']';
            replace(m1, m2.get(i, j), s.str());
        }
    }
}

constexpr int size1 = 8;
constexpr int size2 = size1 / 2;

char nm(int x, int y) {
    switch (10*x+y) {
        case 0:  return '1';
        case 4:  return '2';
        case 40: return '3';
        case 44: return '4';
        default: return '?';
    }
}

int main() {
    auto m = make<size1>(0,0,0,0);
    for (int ax = 0; ax < size1; ax += size2) {
        for (int ay = 0; ay < size1; ay += size2) {
            for (int bx = 0; bx < size1; bx += size2) {
                for (int by = 0; by < size1; by += size2) {
                    std::ostringstream s;
                    s << 'C' << nm(ax,ay) << nm(bx,by);
                    replace(m, make<size2>(ax,ay,bx,by,s.str()));
                }
            }
        }
    }
    std::cout << m << '\n';
    
    
//    print<2>(0, 0, 0, 0);
//    print<2>(0, 0, 0, 2);
//    print<2>(0, 0, 2, 0);
//    print<2>(0, 0, 2, 2);
//
//    print<2>(0, 2, 0, 0);
//    print<2>(0, 2, 0, 2);
//    print<2>(0, 2, 2, 0);
//    print<2>(0, 2, 2, 2);
//
//    print<2>(2, 0, 0, 0);
//    print<2>(2, 0, 0, 2);
//    print<2>(2, 0, 2, 0);
//    print<2>(2, 0, 2, 2);
//
//    print<2>(2, 2, 0, 0);
//    print<2>(2, 2, 0, 2);
//    print<2>(2, 2, 2, 0);
//    print<2>(2, 2, 2, 2);



    
//    slot_map<Foo> sm(2);
//    auto k1 = sm.insert(1); std::cout << k1 << '\n';
//    auto k2 = sm.insert(2); std::cout << k2 << '\n';
//    auto k3 = sm.insert(3); std::cout << k3 << '\n';
//    auto k4 = sm.insert(4); std::cout << k4 << '\n';
//    std::cout << sm << '\n';
//    sm.erase(k2);
//    std::cout << sm << '\n';
//    k2 = sm.insert(2); std::cout << sm << ' ' << k2 << '\n';
//    sm.erase(k1);
//    sm.erase(k2);
//    sm.erase(k3);
//    std::cout << sm << '\n';

    
    
//    auto k1 = sm.insert(123);
//    auto k2 = sm.insert(1233);
//    std::cout << sm << '\n';
//    sm.erase(k1);
//    std::cout << sm << '\n';
//    auto k1 = sm.insert(1); std::cout << k1 << '\n';
//    auto k2 = sm.insert(2); std::cout << k2 << '\n';
//    auto k3 = sm.insert(3); std::cout << k3 << '\n';
//    {
//        Foo f4 = 4;
//        auto k4 = sm.insert(std::move(f4)); std::cout << k3 << '\n';
//    }
//    std::cout << sm << '\n';
//    sm.erase(k2);
//    sm.erase(k2);
//    std::cout << sm << '\n';
//    k2 = sm.insert(2); std::cout << sm << ' ' << k2 << '\n';
//    sm.erase(k1);
//    sm.erase(k2);
//    sm.erase(k3);
//    std::cout << sm << '\n';
//    k2 = sm.insert(2); std::cout << sm << ' ' << k2 << '\n';
//    std::cout << "--------\n";
}
