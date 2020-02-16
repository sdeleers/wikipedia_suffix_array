#include "external_merge_sort.h"
#include "priority_files_reader.h"
#include "sequential_files_reader.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

template <typename T>
ExternalMergeSort<T>::ExternalMergeSort(std::size_t num_elements) :
  num_elements_in_RAM(num_elements) {};

template <typename T>
std::vector<File> ExternalMergeSort<T>::sort(const std::vector<File>& files_in) {
  std::size_t total_file_length = 0;
  for (File file : files_in) total_file_length += file.length; 

  SequentialFilesReader<long long> files_in_reader(files_in);

  // Round up.
  const std::size_t num_buffers = std::ceil((double) total_file_length / num_elements_in_RAM);
  const std::size_t buffer_size = num_elements_in_RAM;

  // Make <num_buffers> buffers of size num_elements_in_RAM,
  // and sort each buffer separately
  std::vector<File> files_sorted;
  std::size_t total_num_read = 0;
  int output_file_index = 0;
  while (total_num_read < total_file_length) {
    // num_to_read will be the number of elements in the sorted output file.
    std::size_t num_to_read = std::min(buffer_size, total_file_length - total_num_read);
    std::vector<T> elements;
    elements.reserve(num_to_read);
    std::size_t num_read = 0;
    // Read <num_to_read> elements from input files (potentially multiple ones).
    T element;
    while (num_read < num_to_read && files_in_reader.read(element)) {
      elements.push_back(element);
      ++num_read;
    }
    std::sort(elements.begin(), elements.end());

    // Write sorted elements to file.
    File file_sorted = { "/tmp/sorted_" + std::to_string(output_file_index), elements.size() };
    files_sorted.push_back(file_sorted);
    std::ofstream file_sorted_stream;
    file_sorted_stream.open(file_sorted.name);
    for (T element : elements) {
      file_sorted_stream << element << std::endl;
    }
    file_sorted_stream.close();

    total_num_read += num_to_read;
    ++output_file_index;
  }

  // -------------- Merge --------------
  PriorityFilesReader<long long> files_sorted_reader(files_sorted);

  int file_out_index = 0;
  int file_out_element_index = 0;
  std::vector<File> files_out = { { "/tmp/out_sorted_0", files_sorted[0].length } };
  std::ofstream output_stream;
  output_stream.open(files_out.back().name);

  T element;
  while (files_sorted_reader.read(element)) {
    // Write element to output file
    if (file_out_element_index == files_out.back().length) {
      file_out_element_index = 0;
      ++file_out_index;
      files_out.push_back({ "/tmp/out_sorted_" + std::to_string(file_out_index),
                          files_sorted[file_out_index].length });
      output_stream.close();
      output_stream.open(files_out.back().name);
    }
    output_stream << element << std::endl;
    ++file_out_element_index;
  }

  return files_out;
}

template ExternalMergeSort<long long>::ExternalMergeSort(std::size_t num_elements);
template std::vector<File> ExternalMergeSort<long long>::sort(const std::vector<File>& files_in);               
