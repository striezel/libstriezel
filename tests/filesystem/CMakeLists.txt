cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for libstriezel::filesystem::file class tests.
add_subdirectory (file)

# Recurse into subdirectory for general libstriezel::filesystem tests.
add_subdirectory (unified)
