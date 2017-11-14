#.rst:
# FindUCharDet
# --------------
#
# Find libuchardet library and headers
#
# The module defines the following variables:
#
# ::
#
#   LibUCharDet_FOUND        - true if libuchardet was found
#   LibUCharDet_INCLUDE_DIRS - include search path
#   LibUCharDet_LIBRARIES    - libraries to link

find_path(LibUCharDet_INCLUDE_DIR
    NAMES "uchardet.h"
    PATH_SUFFIXES "uchardet")

find_library(LibUCharDet_LIBRARY
    NAMES "uchardet" "libuchardet")

mark_as_advanced(LibUCharDet_INCLUDE_DIR LibUCharDet_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibUCharDet
                                  REQUIRED_VARS
                                      LibUCharDet_LIBRARY
                                      LibUCharDet_INCLUDE_DIR)
unset(LIBUCHARDET_FOUND)

if(LibUCharDet_FOUND)
    set(LibUCharDet_INCLUDE_DIRS ${LibUCharDet_INCLUDE_DIR})
    set(LibUCharDet_LIBRARIES    ${LibUCharDet_LIBRARY})
endif()
