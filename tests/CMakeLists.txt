# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for filesystem function tests.
add_subdirectory (filesystem)

# Recurse into subdirectory for hash tests (SHA160 - SHA512 tests).
add_subdirectory (hash)

# Recurse into subdirectory for ISO 9660 disk image tests.
add_subdirectory (iso9660)

# Recurse into subdirectory for os tests.
add_subdirectory (os)

# Recurse into subdirectory for procfs tests.
add_subdirectory (procfs)

# Recurse into subdirectory for ZIP tests.
add_subdirectory (zip)
