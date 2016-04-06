# - Try to find libcdio
# Once done this will define
#  LIBCDIO_FOUND - System has libcdio
#  LIBCDIO_INCLUDE_DIRS - The libcdio include directories
#  LIBCDIO_LIBRARIES - The libraries needed to use libcdio
#  LIBCDIO_DEFINITIONS - Compiler switches required for using libcdio

find_package(PkgConfig)
pkg_check_modules(PC_LIBCDIO QUIET libcdio)
set(LIBCDIO_DEFINITIONS ${PC_LIBCDIO_CFLAGS_OTHER})

find_path(LIBCDIO_INCLUDE_DIR cdio.h
          HINTS ${PC_LIBCDIO_INCLUDEDIR} ${PC_LIBCDIO_INCLUDE_DIRS}
          PATH_SUFFIXES cdio )

find_library(LIBCDIO_LIBRARY NAMES cdio libcdio
             HINTS ${PC_LIBCDIO_LIBDIR} ${PC_LIBCDIO_LIBRARY_DIRS} )

set(LIBCDIO_LIBRARIES ${LIBCDIO_LIBRARY} )
set(LIBCDIO_INCLUDE_DIRS ${LIBCDIO_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBCDIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libcdio  DEFAULT_MSG
                                  LIBCDIO_LIBRARY LIBCDIO_INCLUDE_DIR)

mark_as_advanced(LIBCDIO_INCLUDE_DIR LIBCDIO_LIBRARY )
