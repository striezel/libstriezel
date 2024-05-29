cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for libstriezel::filesystem::directory class tests.
add_subdirectory (directory)

# Recurse into subdirectory for libstriezel::filesystem::file class tests.
add_subdirectory (file)

# Recurse into subdirectory for libstriezel::filesystem::getSizeString() tests.
add_subdirectory (getsizestring)

# Recurse into subdirectory for libstriezel::filesystem::splitPathFileExtension().
add_subdirectory (splitpathfileextension)
