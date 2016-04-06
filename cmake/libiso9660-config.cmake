# - Try to find libiso9660
# Once done this will define
#  LIBISO9660_FOUND - System has libiso9660
#  LIBISO9660_INCLUDE_DIRS - The libiso9660 include directories
#  LIBISO9660_LIBRARIES - The libraries needed to use libiso9660
#  LIBISO9660_DEFINITIONS - Compiler switches required for using libiso9660

find_package(PkgConfig)
pkg_check_modules(PC_LIBISO9660 QUIET libiso9660)
set(LIBISO9660_DEFINITIONS ${PC_LIBISO9660_CFLAGS_OTHER})

find_path(LIBISO9660_INCLUDE_DIR iso9660.h
          HINTS ${PC_LIBISO9660_INCLUDEDIR} ${PC_LIBISO9660_INCLUDE_DIRS}
          PATH_SUFFIXES cdio )

find_library(LIBISO9660_LIBRARY NAMES iso9660 libiso9660
             HINTS ${PC_LIBISO9660_LIBDIR} ${PC_LIBISO9660_LIBRARY_DIRS} )

set(LIBISO9660_LIBRARIES ${LIBISO9660_LIBRARY} )
set(LIBISO9660_INCLUDE_DIRS ${LIBISO9660_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBISO9660_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libiso9660  DEFAULT_MSG
                                  LIBISO9660_LIBRARY LIBISO9660_INCLUDE_DIR)

mark_as_advanced(LIBISO9660_INCLUDE_DIR LIBISO9660_LIBRARY )
