#! /bin/sh
#
# $Id$
#
# A script for running the POCO testsuites.
#
# usage: runtests
#
# If the environment variable EXCLUDE_TESTS is set, containing
# a space-separated list of project names (as found in the
# components file), these tests will be skipped.
#

TESTRUNNER=./testrunner
TESTRUNNERARGS=-all

components=`cat $POCO_BASE/components`

if [ "$OSNAME" = "" ] ; then
	OSNAME=`uname`
fi
if [ "$OSARCH" = "" ] ; then
	OSARCH=`uname -m | tr ' /' _-`
fi
BINDIR="bin/$OSNAME/$OSARCH/"

runs=0
failures=0
failedTests=""
status=0

for comp in $components ;
do
	excluded=0
	for excl in $EXCLUDE_TESTS ;
	do
		if [ "$excl" = "$comp" ] ; then
			excluded=1
		fi
	done
	if [ $excluded -eq 0 ] ; then
		if [ -d "$POCO_BASE/$comp/testsuite/$BINDIR" ] ; then
			if [ -x "$POCO_BASE/$comp/testsuite/$BINDIR/$TESTRUNNER" ] ; then
				echo ""
				echo ""
				echo "****************************************" 
				echo "*** $comp"                                
				echo "****************************************" 
				echo ""

				runs=`expr $runs + 1`
				sh -c "cd $POCO_BASE/$comp/testsuite/$BINDIR && $TESTRUNNER $TESTRUNNERARGS"
				if [ $? -ne 0 ] ; then
					failues=`expr $failures + 1`
					failedTests="$failedTests $comp"
					status=1
				fi
			fi
		fi
	fi
done

echo ""
echo ""
echo "$runs runs, $failures failed."
echo ""
for test in $failedTests ;
do
	echo "Failed: $test"
done
echo ""

exit $status
