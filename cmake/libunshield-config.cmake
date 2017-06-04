# - Try to find libunshield
# Once done this will define
#  LIBUNSHIELD_FOUND - System has libunshield
#  LIBUNSHIELD_INCLUDE_DIRS - The libunshield include directories
#  LIBUNSHIELD_LIBRARIES - The libraries needed to use libunshield
#  LIBUNSHIELD_DEFINITIONS - Compiler switches required for using libunshield

find_package(PkgConfig)
pkg_check_modules(PC_LIBUNSHIELD QUIET libunshield)
set(LIBUNSHIELD_DEFINITIONS ${PC_LIBUNSHIELD_CFLAGS_OTHER})

find_path(LIBUNSHIELD_INCLUDE_DIR libunshield.h
          HINTS ${PC_LIBUNSHIELD_INCLUDEDIR} ${PC_LIBUNSHIELD_INCLUDE_DIRS}
         )

find_library(LIBUNSHIELD_LIBRARY NAMES unshield libunshield
             HINTS ${PC_LIBUNSHIELD_LIBDIR} ${PC_LIBUNSHIELD_LIBRARY_DIRS} )

set(LIBUNSHIELD_LIBRARIES ${LIBUNSHIELD_LIBRARY} )
set(LIBUNSHIELD_INCLUDE_DIRS ${LIBUNSHIELD_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBUNSHIELD_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libunshield  DEFAULT_MSG
                                  LIBUNSHIELD_LIBRARY LIBUNSHIELD_INCLUDE_DIR)

mark_as_advanced(LIBUNSHIELD_INCLUDE_DIR LIBUNSHIELD_LIBRARY )
