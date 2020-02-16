#include "external_merge_sort.h"
#include "priority_files_reader.h"
#include "sequential_files_reader.h"
#include "sequential_files_writer.h"

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

  // Make buffers of size num_elements_in_RAM
  std::vector<File> files_sorted;
  files_sorted.reserve(total_file_length / num_elements_in_RAM);
  std::size_t length_to_assign = total_file_length;
  while (length_to_assign > 0) {
    const std::size_t file_length = std::min(num_elements_in_RAM, length_to_assign);
    if (file_length != 0) {
      files_sorted.push_back({ "/tmp/sorted_" + std::to_string(files_sorted.size()), file_length });
      length_to_assign -= file_length;
    }
  }
  SequentialFilesWriter<long long> sorted_files_writer(files_sorted);

  // Read input files, sort them and write to sorted files.
  for (File file_sorted : files_sorted) {
    std::size_t num_to_read = file_sorted.length;
    std::vector<T> elements;
    elements.reserve(num_to_read);

    T element;
    for (int i = 0; i < num_to_read; ++i) {
      files_in_reader.read(element);
      elements.push_back(element);
    }
    std::sort(elements.begin(), elements.end());

    // Write sorted elements to file.
    for (T element : elements) {
      sorted_files_writer.write(element);
    }
  }

  // Repeatedly read smallest element from sorted files and write to merged files.
  PriorityFilesReader<long long> files_sorted_reader(files_sorted);
  std::vector<File> files_merged;
  files_merged.reserve(files_sorted.size());
  for (int i = 0; i < files_sorted.size(); ++i) {
    files_merged.push_back({ "/tmp/merged_" + std::to_string(i), files_sorted[i].length });
  }
  SequentialFilesWriter<long long> files_merged_writer(files_merged);

  T element;
  while (files_sorted_reader.read(element)) {
    files_merged_writer.write(element);
  }

  return files_merged;
}

template ExternalMergeSort<long long>::ExternalMergeSort(std::size_t num_elements);
template std::vector<File> ExternalMergeSort<long long>::sort(const std::vector<File>& files_in);               
