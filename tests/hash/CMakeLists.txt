# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# option for long-running hash tests
option(LONG_HASH_TESTS "whether to run long-running hash tests" ON)

# Recurse into subdirectory for SHA-1 test (160 bit digest).
add_subdirectory (sha160)

# Recurse into subdirectory for SHA-224 tests (224 bit digest).
add_subdirectory (sha224)

# Recurse into subdirectory for SHA-256 tests (256 bit digest).
add_subdirectory (sha256)

# Recurse into subdirectory for SHA-384 tests (384 bit digest).
add_subdirectory (sha384)

# Recurse into subdirectory for SHA-512 tests (512 bit digest).
add_subdirectory (sha512)
