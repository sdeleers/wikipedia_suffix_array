#include <chrono>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

// Output:
// ps -o pid,min_flt,maj_flt 6721
//  PID  MINFL  MAJFL
//  6721   9909      0
// 
// 10^7 elements, 10^4 pagefaults -> 10^3 elements read at a time
// page size is 4 * 10^ 3 which makes sense since every int is 4 bytes

void generateTextFile() {
  std::ofstream os;
  os.open("data.txt");
  for (int i = 0; i < 1e7; ++i) {
    os << rand() << std::endl;
  }
  os.close();
}

void readFileLineByLine() {
  std::ifstream is;
  is.open("data.txt");
  int N = 1e7;
  std::vector<int> numbers(N);
  for (int i = 0; i < N; ++i) {
    is >> numbers[i];
  }
  is.close();
}

void readFileInChunks() {
  std::ifstream is;
  is.open("data.txt");
  int N = 1e7;
  std::vector<int> buffer(1000);
  is.read(buffer.data(), buffer.size());
  is.close();
}

int main() {
  // generateTextFile();
  std::cout << getpid() << std::endl;
  sleep(10);

  
  auto start = std::chrono::system_clock::now();
  auto end = std::chrono::system_clock::now();

  readFileLineByLine();

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;


  start = std::chrono::system_clock::now();
  end = std::chrono::system_clock::now();
  readFileInChunks();

  elapsed_seconds = end-start;
  end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;

  sleep(10);
}