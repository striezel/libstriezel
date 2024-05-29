cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for 7-Zip archive tests.
add_subdirectory (7z)

# Recurse into subdirectory for Ar archive tests.
add_subdirectory (ar)

# Recurse into subdirectory for Cabinet archive tests.
add_subdirectory (cab)

# Recurse into subdirectory for filesystem function tests.
add_subdirectory (filesystem)

# Recurse into subdirectory for gzip decompression tests.
add_subdirectory (gzip)

# Recurse into subdirectory for hash tests (SHA160 - SHA512 tests).
add_subdirectory (hash)

# Recurse into subdirectory for ISO 9660 disk image tests.
add_subdirectory (iso9660)

# Recurse into subdirectory for os tests.
add_subdirectory (os)

# Recurse into subdirectory for procfs tests.
add_subdirectory (procfs)

# Recurse into subdirectory for rar archive tests.
add_subdirectory (rar)

# Recurse into subdirectory for tar archive tests.
add_subdirectory (tar)

# Recurse into subdirectory for xz archive tests.
add_subdirectory (xz)

# Recurse into subdirectory for ZIP tests.
add_subdirectory (zip)
