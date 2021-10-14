#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> buildSuffixArray(const string& s) {
  int N = s.size();
  int num_levels = log2(N) + 2; // Add some margin
  vector<vector<int>> suffix_array(num_levels, vector<int>(N));

  for (int level = 0; level < num_levels; ++level) {
    vector<pair<pair<int, int>, int>> elements(N, { { -1, -1 }, -1 });
    for (int i = 0; i < s.size(); ++i) {
      if (level == 0) {
        elements[i] = { { s[i], -1 }, i };
      } else {
        int step = level - 1;
        int second_element = (i + (1 << step)) < N ? suffix_array[level - 1][i + (1 << step)] : -1;
        elements[i] = { { suffix_array[level - 1][i],
                          second_element }, i };
      }
    }
    sort(elements.begin(), elements.end());  // Can be optimized using radix sort.
    int current = 0;
    for (int i = 0; i < N; ++i) {
      if (i > 0 && elements[i].first != elements[i - 1].first) ++current;
      suffix_array[level][elements[i].second] = current;
    }
  }
  return suffix_array;
}

vector<int> calculateLCP(const string& s,
                         const vector<pair<int, int>> order,
                         const vector<vector<int>>& suffix_array) {
  int N = s.size();
  int num_levels = suffix_array.size();
  vector<int> lcp(N - 1);

  for (int i = 0; i < N - 1; ++i) {
    // Compute LCP of final_order[i] and final_order[i + 1]
    lcp[i] = 0;
    int index1 = order[i].second;
    int index2 = order[i + 1].second;
    for (int level = num_levels - 1; level >= 0; --level) {
      // If we can make the jump, make it.
      if (index1 < N && index2 < N && suffix_array[level][index1] == suffix_array[level][index2]) {
        int increment = (1 << level);
        lcp[i] += increment;
        index1 += increment;
        index2 += increment;
      }
    }
  }
  return lcp;
}

// Longest valid prefix, i.e. containing 
// only characters a..z, A..Z, 0..9, -, ., !
vector<int> calculateLVP(const string& s) {
  // List of valid characters.
  vector<bool> valid(256, false);
  for (int i = 0; i < valid.size(); ++i) {
    char c = (char) i;
    if (isalnum(c)) valid[i] = true;
    if (c == '.') valid[i] = true;
    if (c == '!') valid[i] = true;
    if (c == ' ') valid[i] = true;
    if (c == '(') valid[i] = true;
    if (c == ')') valid[i] = true;
    if (c == '-') valid[i] = true;
    if (c == ',') valid[i] = true;
    if (c == ';') valid[i] = true;
    if (c == '\'') valid[i] = true;
  }

  // Calculate LVP (DP).
  int N = s.size();
  vector<int> lvp(N);
  for (int i = N - 1; i >= 0; --i) {
    int position = (int) s[i];
    if (position >= 0 && position < 256 && valid[position]) {
      lvp[i] = (i == N - 1) ? 1 : lvp[i + 1] + 1;
    } else {
      lvp[i] = 0;
    }
  }
  return lvp;
}

string findLongestRepeatingSubstring(const string& s,
                                     const vector<pair<int, int>>& order,
                                     const vector<int>& lcp,
                                     const vector<int>& lvp) {
  int max_index;
  int max_value = 0;
  for (int i = 0; i < lcp.size(); ++i) {
    int longest_valid = min(lvp[order[i].second], lvp[order[i + 1].second]);
    int longest_common = lcp[i];
    int length_i = min(longest_valid, longest_common);
    if (length_i > max_value) {
      max_value = length_i;
      max_index = i;
    }
  }
  return s.substr(order[max_index].second, max_value);
}

vector<pair<int, int>> getSuffixOrder(const vector<vector<int>>& suffix_array) {
  int N = suffix_array.back().size();
  vector<pair<int, int>> order(N);
  for (int i = 0; i < N; ++i) {
    order[i] = { suffix_array.back()[i], i };
  }
  sort(order.begin(), order.end());
  return order;
}

// Complexity: O(N log N)
int main() {
  freopen("/home/simon/data/out/whole_string.txt", "r", stdin);

  string s;
  getline(cin, s, '\0');

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(); 

  int N = s.size();
  vector<vector<int>> suffix_array = buildSuffixArray(s);
  vector<int> lvp = calculateLVP(s);

  // Calculate longest common prefixes of adjacent suffixes
  // in the sorted suffix array.
  vector<pair<int, int>> order = getSuffixOrder(suffix_array);
  vector<int> lcp = calculateLCP(s, order, suffix_array);

  // Find longest substring of s that appears at least twice.
  string longest_repeating_substring = findLongestRepeatingSubstring(s, order, lcp, lvp);
  cout << longest_repeating_substring.size() << endl;
  cout << longest_repeating_substring << endl;


  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now(); 

  std::chrono::duration<double> elapsed_seconds = end - start; 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end); 

  cout << "Elapsed time: " << elapsed_seconds.count() << endl; 
}
