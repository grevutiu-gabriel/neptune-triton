AC_INIT([neptune-triton], [1.1.0], [mark_r_eggleston@yahoo.co.uk])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AC_PROG_CXX
AC_LANG(C++)
AX_BOOST_BASE([1.40.0],,AC_MSG_ERROR([boost is required]))
AX_BOOST_THREAD
AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
#
# check for Qt ...
#
AT_WITH_QT([xml])
#if test "$have_qt" = no; then
#    AC_MSG_ERROR([Qt is required])
#fi

# check for version 4.6 and above ?

AC_MSG_CHECKING(for recent GMP)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include "gmp.h"
#if (__GNU_MP_VERSION*100+__GNU_MP_VERSION_MINOR*10+__GNU_MP_VERSION_PATCHLEVEL < 421)
# error "min GMP version is 4.2.1"
error
#endif
]])],[AC_MSG_RESULT(yes)],[
 AC_MSG_RESULT(no)
 AC_MSG_ERROR([GMP 4.2.1 min required])
])

AC_MSG_CHECKING(for recent MPFR)
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include "mpfr.h"
#if (MPFR_VERSION_MAJOR*100+MPFR_VERSION_MINOR*10+MPFR_VERSION_PATCHLEVEL < 300)
# error "min MPFR version is 3.0.0"
error
#endif
]])],[AC_MSG_RESULT(yes)],[
 AC_MSG_RESULT(no)
 AC_MSG_ERROR([MPFR 3.0.0 min required])
])

AC_SEARCH_LIBS(__gmpz_init, gmp, , AC_MSG_ERROR([gmp library not found]))
GMP_LIB="$ac_cv_search"
AC_SEARCH_LIBS(mpfr_init2, mpfr, , AC_MSG_ERROR([mpfr library not found]))
MPFR_LIB="$ac_cv_search"

AX_CXX_COMPILE_STDCXX_11
AX_X86_64_CHECK
case $ax_cv_gcc_environment in
"32-bit") CXXFLAGS="$CXXFLAGS -m32";;
"64-bit") CXXFLAGS="$CXXFLAGS -m64";;
esac
case $ax_cv_gcc_environment in
"32-bit") LDFLAGS="$LDFLAGS -m32";;
"64-bit") LDFLAGS="$LDFLAGS -m64";;
esac
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

