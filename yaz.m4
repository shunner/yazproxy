## $Id: yaz.m4,v 1.2 2004-08-16 12:38:22 adam Exp $
## 
# Use this m4 function for autoconf if you use YAZ in your own
# configure script.

dnl ----- Setup Docbook documentation for YAZ
AC_DEFUN([YAZ_DOC],
[
AC_SUBST(DTD_DIR)	
AC_ARG_WITH(dtd, [  --with-dtd[=DIR]        use docbookx.dtd in DIR],
[
	if test -f "$withval/docbookx.dtd"; then
		DTD_DIR=$withva	l
	fi
],[
	AC_MSG_CHECKING(for docbookx.dtd)
	DTD_DIR=""
	for d in /usr/share/sgml/docbook/dtd/xml/4.1.2 \
		/usr/share/sgml/docbook/xml-dtd-4.1.2* \
		/usr/share/sgml/docbook/xml-dtd-4.1 \
		/usr/share/sgml/docbook/dtd/xml/4.0 \
		/usr/lib/sgml/dtd/docbook-xml 
	do
		if test -f $d/docbookx.dtd; then
			AC_MSG_RESULT($d)
			DTD_DIR=$d
			break
		fi
	done
	if test -z "$DTD_DIR"; then
		AC_MSG_RESULT(Not found)
	fi
])
AC_SUBST(DSSSL_DIR)
AC_ARG_WITH(dsssl,[  --with-dsssl[=DIR]      use Docbook DSSSL in DIR/{html,print}/docbook.dsl],
[
	if test -f "$withval/html/docbook.dsl"; then
		DSSSL_DIR=$withval
	fi
],[
	AC_MSG_CHECKING(for docbook.dsl)
	DSSSL_DIR=""
	for d in /usr/share/sgml/docbook/stylesheet/dsssl/modular \
		/usr/share/sgml/docbook/dsssl-stylesheets-1.* \
		/usr/lib/sgml/stylesheet/dsssl/docbook/nwalsh 
	do
		if test -f $d/html/docbook.dsl; then
			AC_MSG_RESULT($d)
			DSSSL_DIR=$d
			break
		fi
	done
	if test -z "$DSSSL_DIR"; then
		AC_MSG_RESULT(Not found)
	fi
])
AC_SUBST(XSL_DIR)
AC_ARG_WITH(xsl,[  --with-xsl[=DIR]        use Docbook XSL in DIR/{htmlhelp,xhtml}],
[
	if test -f "$withval/htmlhelp/htmlhelp.xsl"; then
		XSL_DIR=$withval
	fi
],[
	AC_MSG_CHECKING(for htmlhelp.xsl)
	for d in /usr/share/sgml/docbook/stylesheet/xsl/nwalsh \
		/usr/share/sgml/docbook/xsl-stylesheets-1.* 
	do
		if test -f $d/htmlhelp/htmlhelp.xsl; then
			AC_MSG_RESULT($d)
			XSL_DIR=$d
			break
		fi
	done
	if test -z "$XSL_DIR"; then
		AC_MSG_RESULT(Not found)
	fi
])
]) 

AC_DEFUN([YAZ_INIT],
[
	AC_SUBST(YAZLIB)
	AC_SUBST(YAZLALIB)
	AC_SUBST(YAZINC)
	AC_SUBST(YAZVERSION)
	yazconfig=NONE
	yazpath=NONE
	AC_ARG_WITH(yazconfig, [  --with-yazconfig=DIR    yaz-config in DIR (example /home/yaz-1.7)], [yazpath=$withval])
	if test "x$yazpath" != "xNONE"; then
		yazconfig=$yazpath/yaz-config
	else
		if test "x$srcdir" = "x"; then
			yazsrcdir=.
		else
			yazsrcdir=$srcdir
		fi
		for i in ${yazsrcdir}/../yaz* ${yazsrcdir}/../yaz ../yaz* ../yaz; do
			if test -d $i; then
				if test -r $i/yaz-config; then
					yazconfig=$i/yaz-config
				fi
			fi
		done
		if test "x$yazconfig" = "xNONE"; then
			AC_PATH_PROG(yazconfig, yaz-config, NONE)
		fi
	fi
	AC_MSG_CHECKING(for YAZ)
	if $yazconfig --version >/dev/null 2>&1; then
		YAZLIB=`$yazconfig --libs $1`
		# if this is empty, it's a simple version YAZ 1.6 script
		# so we have to source it instead...
		if test "X$YAZLIB" = "X"; then
			. $yazconfig
		else
			YAZLALIB=`$yazconfig --lalibs $1`
			YAZINC=`$yazconfig --cflags $1`
			YAZVERSION=`$yazconfig --version`
		fi
		AC_MSG_RESULT([$yazconfig])
	else
		AC_MSG_RESULT(Not found)
		YAZVERSION=NONE
	fi
	if test "X$YAZVERSION" != "XNONE"; then
		AC_MSG_CHECKING([for YAZ version])
		AC_MSG_RESULT([$YAZVERSION])
		if test "$2"; then
			have_yaz_version=`echo "$YAZVERSION" | awk 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 1000 + [$]2) * 1000 + [$]3;}'`
			req_yaz_version=`echo "$2" | awk 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 1000 + [$]2) * 1000 + [$]3;}'`
			if test "$have_yaz_version" -lt "$req_yaz_version"; then
				AC_MSG_ERROR([$YAZVERSION. Requires $2 or later])
			fi
		fi
	fi
]) 
