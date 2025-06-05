cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for test of libstriezel::iso9660::archive::entries()
# and libstriezel::zip::archive::numEntries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::iso9660::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::iso9660::archive::isISO9660().
add_subdirectory (is-iso9660)
