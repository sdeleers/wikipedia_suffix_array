import suffix_array
import time

def main():
  f = open("/home/simon/data/out/string_200_articles.txt", "r")
  text = f.read()

  # suff_array = suffix_array.buildSuffixArray(text)
  # lvp = suffix_array.calculateLVP(text)
  # order = suffix_array.getSuffixOrder(suff_array)
  # lcp = suffix_array.calculateLCP(text, order, suff_array)
  # longest_repeating_substring = suffix_array.findLongestRepeatingSubstring(text, order, lcp, lvp)
  # print(longest_repeating_substring)
  print(suffix_array.doEverything(text))


if __name__ == '__main__':
  start_time = time.time()
  main()
  print("%s seconds" % (time.time() - start_time))