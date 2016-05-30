AC_INIT([testPkg], 1.0)

## Use xapian-config-1.3 to find arguments for compiler and linker flags
##
## Check for non-standard programs: xapian-config-1.3
AC_PATH_PROG([XAPIAN_CONFIG], [xapian-config-1.3])
## If xapian-config-1.3 was found, let's use it
if test "${XAPIAN_CONFIG}" != ""; then
    # Use gsl-config for header and linker arguments
    XAPIAN_CXXFLAGS=`${XAPIAN_CONFIG} --cxxflags`
    XAPIAN_LIBS=`${XAPIAN_CONFIG} --libs`
else
    AC_MSG_ERROR([xapian-config-1.3 not found, is Xapian installed?])
fi

# Now substitute these variables in src/Makevars.in to create src/Makevars
AC_SUBST(XAPIAN_CXXFLAGS)
AC_SUBST(XAPIAN_LIBS)

AC_OUTPUT(src/Makevars)
