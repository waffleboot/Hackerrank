#include <bits/stdc++.h>

// https://www.hackerrank.com/contests/w35/challenges/matrix-land/problem

using namespace std;

vector<int> t;
vector<int> a1;
vector<int> b1;
vector<int> a2;
vector<int> b2;
void tt(const vector<int>& v) {
    auto width = v.size();
    a1[0] = v[0];
    b1[0] = v[0] + t[0];
    for (size_t i = 1; i < width; ++i) {
        a1[i] = max(v[i],a1[i-1]+v[i]);
        b1[i] = max(a1[i]+t[i],b1[i-1]+v[i]);
    }
    a2[width-1] = v[width-1];
    b2[width-1] = v[width-1] + t[width-1];
    for (size_t i = 1; i < width; ++i) {
        auto j = width-i-1;
        a2[j] = max(v[j],a2[j+1]+v[j]);
        b2[j] = max(a2[j]+t[j],v[j]+b2[j+1]);
    }
    t[0] = b2[0];
    for (int i = 1; i < width-1; ++i) {
        t[i] = max(b1[i]+a2[i],a1[i]+b2[i])-v[i];
    }
    t[width-1] = b1[width-1];
}

int matrixLand(const vector<vector<int>>& A) {
    auto width = A[0].size();
    t.resize(width);
    a1.reserve(width);
    b1.reserve(width);
    a2.reserve(width);
    b2.reserve(width);
    for (size_t i = 0; i < A.size(); ++i) {
        tt(A[i]);
    }
    return *max_element(t.cbegin(), t.cend());
}

int main() {
    int n;
    int m;
    cin >> n >> m;
    vector< vector<int> > A(n,vector<int>(m));
    for(int A_i = 0;A_i < n;A_i++){
       for(int A_j = 0;A_j < m;A_j++){
          cin >> A[A_i][A_j];
       }
    }
    int result = matrixLand(A);
    cout << result << endl;
    return 0;
}

