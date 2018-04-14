
#include <vector>
#include <iostream>

using namespace std;

template <typename T> T adj(T v) { return v - 1; }

struct Distance {
    typedef short value_type;
    static const value_type inf;
    static bool is_inf(value_type v) { return v == inf; }
    static value_type sum(value_type a, value_type b) {
        return (is_inf(a) || is_inf(b)) ? inf : a + b;
    }
    static ostream& print(ostream& os, value_type v) {
        if (Distance::is_inf(v) || v == 0) {
            os << ".";
        } else {
            os << v;
        }
        return os;
    }
};

struct Matrix {
    typedef short size_type;
    typedef Distance::value_type value_type;
    typedef vector<value_type> matrix_type;
    matrix_type matrix1, matrix2;
    const size_type n;
    Matrix(size_type n) : n(n), matrix1(n*n),matrix2(n*n) {
        for (size_type i = 1; i <= n; ++i) {
            auto b = begin(matrix(0)) + adj(i)*n;
            fill(b,b+n,Distance::inf);
        }
    }
    matrix_type& matrix(size_type k) { return (k % 2) ? matrix1 : matrix2; }
    const matrix_type& matrix(size_type k) const { return (k % 2) ? matrix1 : matrix2; }
    value_type operator()(size_type i, size_type j, size_type k) const {
        return matrix(k)[adj(i)*n+adj(j)];
    }
    value_type& operator()(size_type i, size_type j, size_type k) {
        return matrix(k)[adj(i)*n+adj(j)];
    }
    void dump() {
        cout << '\t';
        for (size_type i = 1; i <= n; ++i) cout << i << '\t';
        cout << "\n\t";
        for (size_type i = 1; i <= n; ++i) cout << "_\t";
        cout << '\n';
        for (size_type i = 1; i <= n; ++i) {
            cout << i << ")\t";
            for (size_type j = 1; j <= n; ++j) {
                Distance::print(cout,(*this)(i,j,n)); cout << '\t';
            }
            cout << '\n';
        }
    }
};

const Distance::value_type Distance::inf = numeric_limits<Distance::value_type>::max();

struct Graph {
    typedef size_t size_type;
    vector<vector<size_type>> adjacency_list;
    Graph(size_type n) : adjacency_list(n,vector<size_type>()) { }
    Graph& add(size_type v, size_type u) {
        auto lo = (v < u) ? v : u;
        auto hi = (v < u) ? u : v;
        adjacency_list[lo-1].push_back(hi);
        return *this;
    }
    Matrix distances() const {
        const auto n = adjacency_list.size();
        Matrix m(n);
        for (size_type v = 1; v <= n; ++v) {
            for (size_type u : adjacency_list[v-1]) {
                m(v,u,0) = 1;
                m(u,v,0) = 1;
            }
        }
        for (size_type k = 1; k <= n; ++k) {
            for (size_type i = 1; i <= n; ++i) {
                for (size_type j = 1; j <= n; ++j) {
                    if (i == j) continue;
                    auto a = m(i,k,k-1);
                    auto b = m(k,j,k-1);
                    auto sum = Distance::sum(a,b);
                    m(i,j,k) = min(sum,m(i,j,k-1));
                }
            }
        }
        return m;
    }
};

int main() {
//    Matrix3D(3).distances().dump();
    Graph g(8);
    g.add(1,3).add(3,4).add(4,2).add(3,5).add(4,6).add(5,6).add(5,7).add(6,8);
//    g.add(1,2).add(2,3);
    g.distances().dump();
    return 0;
}
