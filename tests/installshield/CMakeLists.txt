# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for test of
# libstriezel::installshield::archive::entries() and
# libstriezel::installshield::archive::numEntries().
add_subdirectory (entries)

# Recurse into subdirectory for test of
# libstriezel::installshield::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of
# libstriezel::installshield::archive::isInstallShield().
add_subdirectory (is-installshield)
