#include "file.h"

#include <fstream>
#include <vector>

template <typename T>
class SequentialFilesReader {
 public:
  SequentialFilesReader(const std::vector<File>& files);
  bool read(T&);
  
 private:
  std::vector<File> files_;
  std::size_t current_file_index_;
  std::size_t current_file_element_index_;
  std::ifstream current_file_stream_;
};