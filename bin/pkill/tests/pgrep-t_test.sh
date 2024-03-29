#!/bin/sh
# $FreeBSD: release/10.1.0/bin/pkill/tests/pgrep-t_test.sh 263351 2014-03-19 12:46:04Z jmmv $

base=`basename $0`

echo "1..2"

name="pgrep -t <tty>"
tty=`ps -x -o tty -p $$ | tail -1`
if [ "$tty" = "??" -o "$tty" = "-" ]; then
	tty="-"
	ttyshort="-"
else
	case $tty in
	pts/*)	ttyshort=`echo $tty | cut -c 5-` ;;
	*)	ttyshort=`echo $tty | cut -c 4-` ;;
	esac
fi
sleep=$(pwd)/sleep.txt
ln -sf /bin/sleep $sleep
$sleep 5 &
sleep 0.3
chpid=$!
pid=`pgrep -f -t $tty $sleep`
if [ "$pid" = "$chpid" ]; then
	echo "ok 1 - $name"
else
	echo "not ok 1 - $name"
fi
pid=`pgrep -f -t $ttyshort $sleep`
if [ "$pid" = "$chpid" ]; then
	echo "ok 2 - $name"
else
	echo "not ok 2 - $name"
fi
kill $chpid
rm -f $sleep
