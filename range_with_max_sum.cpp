
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

pair<int,int> check(const vector<int>& v) {
    int maxSum = v[0];
    const size_t size = v.size();
    size_t leftPos = 0, rightPos = 0;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {

            int sum = 0;
            for (size_t k = i; k <= j; ++k) {
                sum += v[k];
            }
            if (sum > maxSum) {
                maxSum = sum;
                leftPos = i;
                rightPos = j;
            }

        }
    }
    return make_pair(leftPos, rightPos);
}

pair<int,int> find(const vector<int>& v) {
    const size_t size = v.size();
    vector<int> sum(size+1);
    vector<size_t> pos(size+1);
    sum[size] = 0;
    pos[size] = size;
    int totalRightSum = 0, minRightSum = 0; size_t rightPos = size;
    for (size_t i = size; i > 0; --i) {
        totalRightSum += v[i-1];
        if (totalRightSum < minRightSum) {
            minRightSum = totalRightSum;
            rightPos = i-1;
        }
        sum[i-1] = minRightSum;
        pos[i-1] = rightPos;
    }
    int totalLeftSum = 0, minSum = sum[1]; size_t leftPos = -1; rightPos = pos[1];
    for (size_t i = 0; i < size-1; ++i) {
        totalLeftSum += v[i];
        int s = totalLeftSum + sum[i+2];
        if (s < minSum) {
            minSum = s;
            leftPos = i;
            rightPos = pos[i+2];
        }
    }
    return make_pair(leftPos+1, rightPos-1);
}

void test(const vector<int>& x) {
    auto a = find(x);
    auto b = check(x);
    if (a != b) {
        int sum1 = 0, sum2 = 0;
        for (int i = a.first; i <= a.second; ++i) sum1 += x[i];
        for (int i = b.first; i <= b.second; ++i) sum2 += x[i];
        if (sum1 != sum2) {
            cout << "[" << x[0];
            for (int i = 1; i < x.size(); ++i) {
                cout << ' ' << x[i];
            }
            cout << "] ";
            cout << a.first << ',' << a.second << " ~ " << b.first << ',' << b.second << '\n';
        }
    }
}

int main() {
    if (true) {
        const int size = 7;
        const int limit = 6;
        vector<int> x(size);
        for (int i = 0; i < size; ++i) x[i] = -limit;
        while (x[0] <= limit) {
            test(x);
            ++x[x.size()-1];
            for (size_t i = size-1; i > 0; --i) {
                if (x[i] == limit) {
                    x[i] = -limit;
                    ++x[i-1];
                }
            }
        }
        cout << "done\n";
    } else {
        int n; cin >> n;
        for (;;) {
            vector<int> v(n);
            for (int i = 0; i < n; ++i) {
                cin >> v[i];
            }
            test(v);
        }
    }
}
