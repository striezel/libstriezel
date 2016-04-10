# - Try to find libarchive
# Once done this will define
#  LIBARCHIVE_FOUND - System has libarchive
#  LIBARCHIVE_INCLUDE_DIRS - The libarchive include directories
#  LIBARCHIVE_LIBRARIES - The libraries needed to use libarchive
#  LIBARCHIVE_DEFINITIONS - Compiler switches required for using libarchive

find_package(PkgConfig)
pkg_check_modules(PC_LIBARCHIVE QUIET libarchive)
set(LIBARCHIVE_DEFINITIONS ${PC_LIBARCHIVE_CFLAGS_OTHER})

find_path(LIBARCHIVE_INCLUDE_DIR archive.h
          HINTS ${PC_LIBARCHIVE_INCLUDEDIR} ${PC_LIBARCHIVE_INCLUDE_DIRS}
          PATH_SUFFIXES archive )

find_library(LIBARCHIVE_LIBRARY NAMES archive libarchive
             HINTS ${PC_LIBARCHIVE_LIBDIR} ${PC_LIBARCHIVE_LIBRARY_DIRS} )

set(LIBARCHIVE_LIBRARIES ${LIBARCHIVE_LIBRARY} )
set(LIBARCHIVE_INCLUDE_DIRS ${LIBARCHIVE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBARCHIVE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libarchive DEFAULT_MSG
                                  LIBARCHIVE_LIBRARY LIBARCHIVE_INCLUDE_DIR)

mark_as_advanced(LIBARCHIVE_INCLUDE_DIR LIBARCHIVE_LIBRARY )
