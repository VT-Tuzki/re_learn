#! /bin/sh

MAKE_OPT=""
GDB_OPT=""
parse_arguments() {
    if test $1 = "1"; then
        MAKE_OPT="MEMCHECK=1"
    elif test $1 = "2"; then
        MAKE_OPT="MEMCHECK=2"
    elif test $1 = "0"; then
        MAKE_OPT=""
    else
        echo "usage: run-test.sh [1|2]" 2>&1
        exit 1
    fi
}



run_test() {
    echo "\033[33m [$1] "$2" \033[0m"
    make clean
    make $MAKE_OPT $GDB_OPT build/$(basename $2 .c) || exit 1
    echo
    echo
}


if test -n $1 ; then
    parse_arguments $1
fi

if test $2 -eq 1; then
    GDB_OPT="GDBTEST=1"
    for f in test/gdb/*.c; do
        run_test "GDB" $f
    done
else
    for f in test/*.c; do
        run_test "TEST" $f
    done
fi

echo ALL test done.
