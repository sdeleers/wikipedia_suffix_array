import math
import time

def buildSuffixArray(text):
  N = len(text)
  num_levels = int(math.log2(N)) + 2 # Add some margin.
  suffix_array = []
  for level in range(num_levels):
    elements = []
    for i in range(N):
      if level == 0:
        element = ((text[i], -1), i)
        elements.append(element)
      else:
        step = level - 1
        second_element = suffix_array[level - 1][i + (1 << step)] if ((i + (1 << step)) < N) else -1
        element = ((suffix_array[level - 1][i], second_element), i)
        elements.append(element)
    elements.sort()
    current = 0
    suffix_array_level = N * [None]
    for i in range(N):
      if i > 0 and elements[i][0] != elements[i - 1][0]:
        current += 1
      suffix_array_level[elements[i][1]] = current
    suffix_array.append(suffix_array_level)
  return suffix_array

def calculateLCP(text, order, suffix_array):
  N = len(text)
  num_levels = len(suffix_array)
  lcp = []
  for i in range(N - 1):
    value = 0
    index1 = order[i][1]
    index2 = order[i + 1][1]
    for level in range(num_levels - 1, -1, -1):
      # If we can make the jump, make it.
      if index1 < N and index2 < N and suffix_array[level][index1] == suffix_array[level][index2]:
        increment = 1 << level
        value += increment
        index1 += increment
        index2 += increment
    lcp.append(value)
  return lcp

def calculateLVP(text):
  valid = 256 * [False]
  for i in range(len(valid)):
    c = chr(i)
    if (c.isalnum()): valid[i] = True
    if (c == '.'): valid[i] = True
    if (c == '!'): valid[i] = True
    if (c == ' '): valid[i] = True
    if (c == '('): valid[i] = True
    if (c == ')'): valid[i] = True
    if (c == '-'): valid[i] = True
    if (c == ','): valid[i] = True
    if (c == ';'): valid[i] = True
    if (c == '\''): valid[i] = True
  
  # Calculate LVP (DP).
  N = len(text)
  lvp = N * [0]
  for i in range(N - 1, -1, -1):
    char_ord = ord(text[i])
    if char_ord >= 0 and char_ord < 256 and valid[char_ord]:
      if i == N - 1:
        lvp[i] = 1
      else:
        lvp[i] = lvp[i + 1] + 1
  return lvp

def findLongestRepeatingSubstring(text, order, lcp, lvp):
  max_value = 0
  for i in range(len(lcp)):
    longest_valid = min(lvp[order[i][1]], lvp[order[i + 1][1]])
    longest_common = lcp[i]
    length_i = min(longest_valid, longest_common)
    if length_i > max_value:
      max_value = length_i
      max_index = i
  start = order[max_index][1]
  stop = order[max_index][1] + max_value 
  return text[start:stop]

def main():
  f = open("/home/simon/data/out/string_200_articles.txt", "r")
  text = f.read()
  N = len(text)

  suffix_array = buildSuffixArray(text)
  order = []
  for i in range(N):
    order.append((suffix_array[-1][i], i))
  order.sort()

  lvp = calculateLVP(text)
  lcp = calculateLCP(text, order, suffix_array)

  longest_repeating_substring = findLongestRepeatingSubstring(text, order, lcp, lvp)
  print(longest_repeating_substring)

if __name__ == '__main__':
  start_time = time.time()
  main()
  print("%s seconds" % (time.time() - start_time))
