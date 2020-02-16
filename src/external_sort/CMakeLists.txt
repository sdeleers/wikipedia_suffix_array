project(external_sort)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(external_sort
            src/sequential_files_reader.cpp
            src/priority_files_reader.cpp
            src/external_merge_sort.cpp
           )

add_subdirectory(test)
