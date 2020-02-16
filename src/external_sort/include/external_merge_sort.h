#include "file.h"
#include <string>
#include <vector>

template <typename T>
class ExternalMergeSort {
 public:
  ExternalMergeSort(std::size_t num_elements_in_RAM);

  std::vector<File> sort(const std::vector<File>& files_in);

 private:
  std::size_t num_elements_in_RAM;
};