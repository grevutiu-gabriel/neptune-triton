# AX_X86_64_CHECK - no parameters

AC_DEFUN([AX_X86_64_CHECK],
[AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_CANONICAL_HOST])

AC_MSG_CHECKING([for gcc environment])
AC_CACHE_VAL(ax_cv_gcc_environment,
[
ax_cv_gcc_environment="unknown"
if test "$GXX" = yes; then

if test "$cross_compiling" = no; then
case $host_cpu in
  i[[3456]]86*)
     ax_cv_gcc_environment="32-bit";;
  x86_64*)
     ax_cv_gcc_environment="64-bit";;
esac
fi # not cross-compiling
fi # $GXX=yes
])
AC_MSG_RESULT($ax_cv_gcc_environment)
])
