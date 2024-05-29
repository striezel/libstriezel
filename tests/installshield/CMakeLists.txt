cmake_minimum_required (VERSION 3.8)

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
