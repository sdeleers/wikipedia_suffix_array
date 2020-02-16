#include "sequential_files_reader.h"

template <typename T>
SequentialFilesReader<T>::SequentialFilesReader(const std::vector<File>& files) :
    files_(files),
    current_file_index_(0),
    current_file_element_index_(0),
    current_file_stream_() {
  if (!files_.empty()) {
    current_file_stream_.open(files[0].name);
  }
}

template <typename T>
bool SequentialFilesReader<T>::read(T& element) {
  if (current_file_element_index_ == files_[current_file_index_].length) {
    ++current_file_index_;
    current_file_element_index_ = 0;
    if (current_file_index_ < files_.size()) {
      current_file_stream_.close();
      current_file_stream_.open(files_[current_file_index_].name);
    }
  }
  if (current_file_index_ < files_.size()) {
    current_file_stream_ >> element;
    ++current_file_element_index_;
    return true;
  } else {
    return false;
  }
}

template SequentialFilesReader<long long>::SequentialFilesReader(const std::vector<File>& files);
template bool SequentialFilesReader<long long>::read(long long& element);