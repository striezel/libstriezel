cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for libstriezel::filesystem::file::createTemp().
add_subdirectory (createtemp)

# Recurse into subdirectory for libstriezel::filesystem::file::exists().
add_subdirectory (exists)

# Recurse into subdirectory for libstriezel::filesystem::file::getSize64().
add_subdirectory (getsize64)

# Recurse into subdirectory for
# libstriezel::filesystem::file::getSizeAndModificationTime().
add_subdirectory (getsizeandmtime)

# Recurse into subdirectory for libstriezel::filesystem::file::readIntoString().
add_subdirectory (readintostring)

# Recurse into subdirectory for libstriezel::filesystem::file::remove().
add_subdirectory (remove)

# Recurse into subdirectory for libstriezel::filesystem::file::rename().
add_subdirectory (rename)

# Recurse into subdirectory for
# libstriezel::filesystem::file::setSizeModificationTime().
add_subdirectory (setmtime)
