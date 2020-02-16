#include <bits/stdc++.h>
using namespace std;

inline bool leq(int a1, int a2, int b1, int b2) // lexicographic order
{return(a1 < b1 || a1 == b1 && a2 <= b2); } // for pairs
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{return(a1 < b1 || a1 == b1 && leq(a2,a3, b2,b3)); } // and triples

// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
static void radixPass(const vector<int>& a, vector<int>& b, const vector<int>& r, int r_offset, int n, int K) {// count occurrences
  vector<int> c(K + 1); // counter array
  for (int i = 0; i <= K; i++) c[i] = 0; // reset counters
  for (int i = 0; i < n; i++) {
    if (a[i] + r_offset < 0) cout << "out of bounds1" << endl;
    if (a[i] + r_offset >= r.size()) cout << "out of bounds2" << endl;
    if (r[a[i] + r_offset] > K) cout << "out of bounds3" << endl;
    if (r[a[i] + r_offset] < 0) cout << "out of bounds4" << endl;
    c[r[a[i] + r_offset]]++; // count occurrences
  }
  for (int i = 0, sum = 0; i <= K; i++) // exclusive prefix sums
  {int t = c[i]; c[i] = sum; sum += t; }

  for (int i = 0; i < n; i++) b[c[r[a[i] + r_offset]]++] = a[i]; // sort
}
// find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(const vector<int>& s, vector<int>& SA, int n, int K) {
  int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
  vector<int> s12(n02 + 3);
  s12[n02] = s12[n02+1] = s12[n02+2] = 0;
  vector<int> SA12(n02 + 3);
  SA12[n02] = SA12[n02+1] = SA12[n02+2] = 0;
  vector<int> s0(n0);
  vector<int> SA0(n0);

  // generate positions of mod 1 and mod 2 suffixes
  // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
  for (int i=0, j=0; i < n+(n0-n1); i++) if (i%3 != 0) s12[j++] = i;
  // lsb radix sort the mod 1 and mod 2 triples
  radixPass(s12 , SA12, s, 2, n02, K);
  radixPass(SA12, s12 , s, 1, n02, K);
  radixPass(s12 , SA12, s , 0, n02, K);
  // find lexicographic names of triples
  int name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (int i = 0; i < n02; i++) {
    if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
      name++; c0 = s[SA12[i]]; c1 = s[SA12[i]+1]; c2 = s[SA12[i]+2];
    }
    if (SA12[i] % 3 == 1) { 
      s12[SA12[i]/3] = name;
    } // left half
    else {
      s12[SA12[i]/3 + n0] = name;
    } // right half
  }
  // recurse if names are not yet unique
  if (name < n02) {
    suffixArray(s12, SA12, n02, name);
    // store unique names in s12 using the suffix array
    for (int i = 0; i < n02; i++) s12[SA12[i]] = i + 1;
  } else // generate the suffix array of s12 directly
  for (int i = 0; i < n02; i++) SA12[s12[i] - 1] = i;
  // stably sort the mod 0 suffixes from SA12 by their first character
  for (int i=0, j=0; i < n02; i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
  radixPass(s0, SA0, s, 0, n0, K);
  // merge sorted SA0 suffixes and sorted SA12 suffixes
  for (int p=0, t=n0-n1, k=0; k < n; k++) {
    #define GetI() (SA12[t] < n0 ? SA12[t]*3+1: (SA12[t] - n0) * 3 + 2)
    int i = GetI(); // pos of current offset 12 suffix
    int j = SA0[p]; // pos of current offset 0 suffix
    if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
    leq(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) :
    leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0])) {// suffix from SA12 is smaller
      SA[k] = i; t++;
      if (t == n02) // done --- only SA0 suffixes left
      for (k++; p < n0; p++, k++) SA[k] = SA0[p];
    } else { // suffix from SA0 is smaller
      SA[k] = j; p++;
      if (p == n0) // done --- only SA12 suffixes left
      for (k++; t < n02; t++, k++) SA[k] = GetI();
    }
  }
}

int main() {
  // string s = "simon";
  // int N = s.size();

  // int s_ints[N + 3];
  // for (int i = 0; i < N; ++i) s_ints[i] = s[i];
  // // Add 3 zeros at the end.
  // for (int i = N; i < N + 3; ++i) s_ints[i] = 0;

  // int K = 1000; // Not sure what this means.
  // int suffix_array[N + 3];

  // suffixArray(s_ints, suffix_array, N, K);

  // for (int i = 0; i < N; ++i) {
  //   cout << suffix_array[i] << " ";
  // }
  // cout << endl;




  freopen("/home/simon/data/out/whole_string_medium_size.txt", "r", stdin);
  string s;
  getline(cin, s, '\0');
  // string s = "simon";



  int N = s.size();

  vector<int> s_ints(N + 3);
  int num_minus = 0;
  for (int i = 0; i < N; ++i) {
    s_ints[i] = s[i];
    if (s_ints[i] < 0) {
      s_ints[i] = 0;
      ++num_minus;
    }
  }
  cout << "num minus: " << num_minus << endl;
  // Add 3 zeros at the end.
  for (int i = N; i < N + 3; ++i) s_ints[i] = 0;

  int K = 50000000;
  vector<int> suffix_array(N + 3);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(); 


  suffixArray(s_ints, suffix_array, N, K);

  for (int i = 0; i < 5; ++i) {
    cout << suffix_array[i] << " ";
  }
  cout << endl;




  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now(); 
  std::chrono::duration<double> elapsed_seconds = end - start; 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end); 
  cout << "Elapsed time: " << elapsed_seconds.count() << endl; 
}