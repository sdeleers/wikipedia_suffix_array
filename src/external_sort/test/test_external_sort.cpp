#include "external_merge_sort.h"
#include "file.h"
#include "sequential_files_reader.h"
#include "sequential_files_writer.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

bool sortsCorrectly(std::size_t num_elements, std::size_t num_files, std::size_t num_elements_in_RAM) {
  bool success = true;

  std::vector<long long> numbers(num_elements);
  for (long long& number : numbers) number = rand() % 1000000;

  // Write numbers to files
  std::vector<File> input_files(num_files);
  for (int i = 0; i < num_files; ++i) {
    input_files[i] = { "/tmp/input_" + std::to_string(i), num_elements / num_files };
  }
  SequentialFilesWriter<long long> input_files_writer(input_files);
  for (long long number : numbers) {
    input_files_writer.write(number);
  }

  // Normal sort
  std::vector<long long> sorted_numbers = numbers;
  sort(sorted_numbers.begin(), sorted_numbers.end());

  // External sort
  ExternalMergeSort<long long> external_sort(num_elements_in_RAM);
  std::vector<File> externally_sorted_files = external_sort.sort(input_files);
  
  // Read externally sorted numbers.
  std::vector<long long> externally_sorted_numbers(num_elements);
  SequentialFilesReader<long long> files_reader(externally_sorted_files);
  for (int i = 0; i < num_elements; ++i) {
    success &= files_reader.read(externally_sorted_numbers[i]);
  }

  // Assert that both sorts produce same result.
  for (int i = 0; i < num_elements; ++i) {
    success &= (externally_sorted_numbers[i] == sorted_numbers[i]);
    if (!success) break;
  }
  return success;
}

TEST(TestExternalSort, SortsCorrectly) {
  ASSERT_TRUE(sortsCorrectly(1000, 1, 1000));
  ASSERT_TRUE(sortsCorrectly(1000, 10, 1000));
  ASSERT_TRUE(sortsCorrectly(1000, 100, 1000));
  ASSERT_TRUE(sortsCorrectly(1000, 10, 10));
  ASSERT_TRUE(sortsCorrectly(1000, 10, 100));
  ASSERT_TRUE(sortsCorrectly(1000, 10, 10000));
  ASSERT_TRUE(sortsCorrectly(10000, 10, 1000));
  ASSERT_TRUE(sortsCorrectly(100000, 10, 1000));
}