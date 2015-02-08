add_library(csv_import SHARED csv_import.cpp)

# TESTING

add_executable(CsvImportUT csv_import_ut.cpp)
target_link_libraries(CsvImportUT space csv_import gtest gtest_main)

add_test(CsvImportTest CsvImportUT)

