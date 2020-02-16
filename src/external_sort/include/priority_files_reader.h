#include "file.h"

#include <fstream>
#include <queue>
#include <vector>


template <typename T>
class PriorityFilesReader {
 public:
  PriorityFilesReader(const std::vector<File>& files);
  bool read(T&);
  
 private:
  std::vector<File> files_;

  std::vector<std::ifstream> file_streams_;

  std::vector<std::size_t> files_indices_;

  // Pair consists of the element, and the file it's coming from.
  std::priority_queue<std::pair<T, std::size_t>,
                      std::vector<std::pair<T, std::size_t>>,
                      std::greater<std::pair<T, std::size_t>>> queue_;
};