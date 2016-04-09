# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for test of libthoro::iso9660::archive::entries()
# and libthoro::zip::archive::numEntries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libthoro::iso9660::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libthoro::iso9660::archive::isISO9660().
add_subdirectory (is-iso9660)
