# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_external_sort test_external_sort.cpp)
target_include_directories(test_external_sort PRIVATE include)
target_link_libraries(test_external_sort 
		                  gtest_main
		                  external_sort	
		                  )