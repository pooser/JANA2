#
# Autoconf macro for cMsg3.0
# September 12, 2009  David Lawrence <davidl@jlab.org>
#
# Usage:
#
# Just include a single line with "CMSG" in your configure.ac file. This macro
# doesn't take any arguments. It will define HAVE_CMSG to be either "0" or
# "1" depending on whether cMsg is available (and requested). It will also
# define CMSG_CPPFLAGS, CMSG_LDFLAGS, and CMSG_LIBS.
#
#
AC_DEFUN([CMSG],
[
	HAVE_CMSG="0"

	# The behavior we want here is that if the user doesn't specify whether to
	# include cMsg or not, then we look to see if the CMSGROOT environment
	# variable is set. If it is, then we will automatically try to include it.
	# If it is not set, and the --with[out]-cmsg command line switch is not
	# specified, then we disable cMsg support.
	AC_ARG_WITH([cmsg],
		[AC_HELP_STRING([--with-cmsg@<:@=DIR@:>@],
		[include cMsg support (with cMsg install dir)])],
		[user_cmsg=$withval],
		[user_cmsg="notspecified"])

	if test "$user_cmsg" = "notspecified"; then
		if test ! x"$CMSGROOT" = x ; then
			user_cmsg="yes"
		else
			user_cmsg="no"
		fi
	fi
	
	if test ! "$user_cmsg" = "no"; then
		HAVE_CMSG="1"

		if test ! x"$user_cmsg" = xyes; then
			cmsgdir="$user_cmsg"
		elif test ! x"$CMSGROOT" = x ; then 
			cmsgdir="$CMSGROOT"
		else 
			cmsgdir="/usr/local"
		fi
		
		AC_MSG_CHECKING([Looking for cMsg in $cmsgdir])
		if test ! -d $cmsgdir; then
			AC_MSG_RESULT([no])
			HAVE_CMSG="0"
		else
			AC_MSG_RESULT([yes])
		
			CMSG_CPPFLAGS=-I$cmsgdir/include
			CMSG_LDFLAGS=-L$cmsgdir/lib
			CMSG_LIBS='-lcmsg -lcmsgxx -lcmsgRegex'
			
			save_CPPFLAGS=$CPPFLAGS
			save_LDFLAGS=$LDFLAGS
			save_LIBS=$LIBS
			
			CPPFLAGS=$CMSG_CPPFLAGS
			LDFLAGS=$CMSG_LDFLAGS
			LIBS=$CMSG_LIBS
			
			AC_LANG_PUSH(C++)
			AC_CHECK_HEADER(cMsg.hxx, [h=1], [h=0])
			
			AC_MSG_CHECKING([if cmsg needs librt to link])
			cmsg_link_ok=no
			AC_TRY_LINK([#include <cMsg.hxx>],
				[cmsg::cMsg cMsgSys("","","");],
				[cmsg_link_ok=yes])
			
			if test "$cmsg_link_ok" = "no"; then
				cmsg_link_ok=failed
				CMSG_LIBS+=' -ldl -lpthread -lrt'
				LIBS=$CMSG_LIBS
				AC_TRY_LINK([#include <cMsg.hxx>],
					[cmsg::cMsg cMsgSys("","","");],
					[cmsg_link_ok=yes])
			else
				# we get here if the first link attempt succeeded and cmsg_link_ok=yes
				# We set it to "no" to indicate -lrt was not needed (confusing isn't it?)
				cmsg_link_ok=no
			fi
			
			AC_MSG_RESULT($cmsg_link_ok);
			
			if test "$cmsg_link_ok" = "failed"; then
				AC_MSG_ERROR("Test link of cMsg failed (using path=$cmsgdir). Set your CMSGROOT environment variable or use the --with-cmsg=PATH_TO_CMSG argument when running configure")
			fi

			AC_LANG_POP
			
			CPPFLAGS="$save_CPPFLAGS $CMSG_CPPFLAGS"
			LDFLAGS="$save_LDFLAGS $CMSG_LDFLAGS"
			LIBS="$save_LIBS $CMSG_LIBS"
			CMSG_DIR=$cmsgdir
		fi
		
		if test "$HAVE_CMSG" = "0"; then
			AC_MSG_NOTICE("Can't find cMsg (using path=$cmsgdir). ")
			AC_MSG_NOTICE("Set your CMSGROOT environment variable or use the --with-cmsg=PATH_TO_CMSG")
			AC_MSG_NOTICE("argument when running configure. Otherwise disable cMsg support by")
			AC_MSG_NOTICE("re-running configure with --without-cmsg.")
		else
			CMSGROOT="$cmsgdir"
			AC_MSG_NOTICE([Configuring cMsg])
			AC_SUBST(CMSGROOT)
			AC_SUBST(CMSG_CPPFLAGS)
			AC_SUBST(CMSG_LDFLAGS)
			AC_SUBST(CMSG_LIBS)
		fi

	else
		AC_MSG_CHECKING([for cMsg])
		AC_MSG_RESULT([no])
	fi # test ! "$user_cmsg" = "no"

])
