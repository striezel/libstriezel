# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for test of libstriezel::zip::archive::entries()
# and libstriezel::zip::archive::numEntries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::zip::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::zip::archive::isZip().
add_subdirectory (is-zip)
