dnl Given a LIBS string value in $1 coming from command-line arguments or
dnl pkg-config, prepare a similar string named in $2 with parameters for
dnl the linker -- by picking the "-R/some/path" values, and optionally
dnl converting the "-L/some/path" into "-R" too.
dnl Activity is controlled by $3:
dnl   no      Do nothing, return empty $2
dnl   only-R  Only select -R hits (if any are present)
dnl   yes     Convert all -R and -L hits
dnl   /some:/paths     Convert all -R and -L hits except "/some" or "/paths"
dnl Copyright (C) 2024-2025 by Jim Klimov

AC_DEFUN([AX_EXTRACT_RPATH],
[
])
