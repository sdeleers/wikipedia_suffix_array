#include "priority_files_reader.h"

template <typename T>
PriorityFilesReader<T>::PriorityFilesReader(const std::vector<File>& files) :
    files_(files),
    file_streams_(files.size()),
    files_indices_(files.size(), 0) {
  // Open files and insert first element of every file into queue.
  for (int i = 0; i < files.size(); ++i) {
    file_streams_[i].open(files_[i].name);
    if (files_indices_[i] < files_[i].length) {
      ++files_indices_[i];
      T element;
      file_streams_[i] >> element;
      queue_.push({ element, i });
    }
  }
}

template <typename T>
bool PriorityFilesReader<T>::read(T& element) {
  if (!queue_.empty()) {
    std::pair<T, std::size_t> p = queue_.top();
    queue_.pop();
    element = p.first; // Returned element


    // Insert next element from that file, if there is one.
    std::size_t file_index = p.second;
    if (files_indices_[file_index] < files_[file_index].length) {
      ++files_indices_[file_index];
      T next_element;
      file_streams_[file_index] >> next_element;
      queue_.push({ next_element, file_index });
    }

    return true;
  } else {
    return false;
  }
}

template PriorityFilesReader<long long>::PriorityFilesReader(const std::vector<File>& files);
template bool PriorityFilesReader<long long>::read(long long& element);