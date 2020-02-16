#include "external_merge_sort.h"
#include "file.h"
#include "sequential_files_reader.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

TEST(TestExternalSort, SortsCorrectly) {
  std::size_t N = 10000000;
  std::vector<long long> numbers(N);
  for (long long& number : numbers) number = rand() % 1000000;

  // ------- Normal sort ---------
  std::vector<long long> sorted_numbers = numbers;
  sort(sorted_numbers.begin(), sorted_numbers.end());

  // Write numbers to files.
  const std::size_t num_files = 10;
  std::vector<File> input_files(num_files);
  for (int i = 0; i < num_files; ++i) {
    input_files[i] = { "/tmp/input_" + std::to_string(i), N / num_files };
  }
  int index = 0;
  std::ofstream output_stream;
  for (File file : input_files) {
    output_stream.open(file.name);
    int start = index;
    for (int offset = 0; offset < file.length; ++offset) {
      output_stream << numbers[index + offset] << std::endl;
    }
    index += file.length;
    output_stream.close();
  }

  // ------- External sort -------
  std::size_t num_elements_in_RAM = 100000;
  ExternalMergeSort<long long> external_sort(num_elements_in_RAM);
  std::vector<File> externally_sorted_files = external_sort.sort(input_files);
  
  // Read externally sorted numbers.
  std::vector<long long> externally_sorted_numbers(N);
  SequentialFilesReader<long long> files_reader(externally_sorted_files);
  for (int i = 0; i < N; ++i) {
    ASSERT_TRUE(files_reader.read(externally_sorted_numbers[i]));
  }

  // Assert that both sorts produce same result.
  bool same_result = true;
  for (int i = 0; i < N; ++i) {
    same_result &= (externally_sorted_numbers[i] == sorted_numbers[i]);
    if (!same_result) break;
  }
  ASSERT_TRUE(same_result);
}