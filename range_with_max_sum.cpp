
#include <iostream>
#include <utility>
using namespace std;
size_t size;
int data[200];
pair<size_t,size_t> check() {
    int maxSum = data[0];
    size_t leftPos = 0, rightPos = 0;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {

            int sum = 0;
            for (size_t k = i; k <= j; ++k) {
                sum += data[k];
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

int sum[200];
size_t pos[200];

pair<size_t,size_t> find() {
    sum[size] = 0;
    pos[size] = size;
    int totalRightSum = 0, minRightSum = 0; auto minRightPos = size;
    for (auto i = size-1; i > 0; --i) {
        totalRightSum += data[i];
        if (totalRightSum < minRightSum) {
            minRightSum = totalRightSum;
            minRightPos = i;
        }
        sum[i] = minRightSum;
        pos[i] = minRightPos;
    }
    int totalLeftSum = 0, minSum = sum[1]; size_t leftPos = 0;
    for (size_t i = 0; i < size-1; ++i) {
        totalLeftSum += data[i];
        int outerSum = totalLeftSum + sum[i+2];
        if (outerSum < minSum) {
            minSum  = outerSum;
            leftPos = i+1;
        }
    }
    return make_pair(leftPos, pos[leftPos+1]-1);
}

void test() {
    auto a = find();
//    auto b = check();
//    if (a != b) {
//        int sum1 = 0, sum2 = 0;
//        for (auto i = a.first; i <= a.second; ++i) sum1 += data[i];
//        for (auto i = b.first; i <= b.second; ++i) sum2 += data[i];
//        if (sum1 != sum2) {
//            cout << "[" << data[0];
//            for (size_t i = 1; i < size; ++i) {
//                cout << ' ' << data[i];
//            }
//            cout << "] ";
//            cout << a.first << ',' << a.second << " ~ " << b.first << ',' << b.second << '\n';
//        }
//    }
}

int main() {
    if (true) {
        size = 7;
        const int limit = 6;
        for (size_t i = 0; i < size; ++i) data[i] = -limit;
        while (data[0] <= limit) {
            test();
            ++data[size-1];
            for (size_t i = size-1; i > 0 && data[i] == limit; --i) {
                data[i] = -limit;
                ++data[i-1];
            }
        }
    } else {
        cin >> size;
        for (;;) {
            for (size_t i = 0; i < size; ++i) {
                cin >> data[i];
            }
            test();
        }
    }
}
