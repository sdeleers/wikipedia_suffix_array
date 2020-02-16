#include <bits/stdc++.h>
using namespace std;

// Compiling:
//   g++ -std=c++11 -mcmodel=medium exec_time_sort.cpp

const int MAX_VALUE = 1000000000;

int values[1000000010];

int main() {

  for (long long N = 1000; N < 1000000000; N *= 1.5) {

    for (int i = 0; i < N; ++i) {
      values[i] = rand() % MAX_VALUE;
    }

    auto start = std::chrono::high_resolution_clock::now();

    sort(values, values + N);

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << N << ": " << elapsed.count() << endl;
  }
}