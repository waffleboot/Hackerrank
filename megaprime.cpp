
#include <iostream>
#include <cmath>

using namespace std;

bool is_prime(long n) {
  unsigned long q = sqrt(n);
  for (auto i = 3UL; i <= q; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
}

int digits_count(unsigned long n) {
  auto digitsCount = 0;
  for (; n > 0; n /= 10, digitsCount++);
  return digitsCount;
}

unsigned long d23(int digitsCount) {
  auto x = 0UL;
  for (auto i = 0; i < digitsCount-1; ++i) {
    x = x*10+2;
  }
  return x*10+3;
}

unsigned long d77(int digitsCount) {
  auto x = 0UL;
  for (auto i = 0; i < digitsCount; ++i) {
    x = x*10+7;
  }
  return x;
}

int main(){
  unsigned long first = 1;
  unsigned long last  = 100;

  auto digits_count_1 = digits_count(first);
  auto digits_count_2 = digits_count(last);
  
  auto d23_1 = d23(digits_count_1);
  auto d77_1 = d77(digits_count_1);
  auto d23_2 = d23(digits_count_2);
  auto d77_2 = d77(digits_count_2);
  
  if (d77_1 < first && last < d23_2) {
    cout << 0;
    return 0;
  }
  auto s = 0UL;
  if (first < d77_1) {
    s = d23_1;
  } else if (first == d77_1) {
    s = d77_1;
  } else {
    s = d23_2;
  }

  auto e = 0UL;
  if (last < d23_2) {
    if (digits_count_2 - digits_count_1 > 1) {
      e = d77(digits_count_2-1);
    } else {
      e = d77_1;
    }
  } else if (last == d23_2) {
    e = d23_2;
  } else if (last > d77_2) {
    e = d77_2;
  } else {
    e = last;
  }
  
  auto count = 0UL;
  if (first < 3)  count++;
  if (first < 5)  count++;
  if (first < 7)  count++;
  if (first < 11) count++;
  auto x = max(s,23UL);
  while (x <= e) {
    if (first <= x) {
      if (is_prime(x)) {
        count++;
      }
    }
    switch (x % 10) {
      case 7: {
        auto v = x;
        auto sevensCount = -1;
        for (; v % 10 == 7; v /= 10, sevensCount++);
        v = (v % 10 == 2) ? 0 : 1;
        for (auto i = 0; i < sevensCount; ++i) v = v*10+4;
        v = v*10+6;
        x += v;
        break;
      }
      case 3: {
        x += 4;
        break;
      }
    }
  }
  cout << count << '\n';
  return 0;
}
