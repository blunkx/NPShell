#!/bin/bash

prog_name=npshell
if [[ -n "$1" ]]; then
    sum=$(expr $1 + 11)
    echo "removetag0 test.html !$sum" >4.txt
    for i in $(seq ${TEST_CASE_START} $1); do
        echo "noop" >>4.txt
    done
    echo "n00p" >>4.txt
    for i in $(seq ${TEST_CASE_START} 9); do
        echo "noop" >>4.txt
    done
    echo "number | cat" >>4.txt
    echo "exit" >>4.txt
    echo "" >>4.txt
else
    echo "removetag0 test.html !999" >4.txt
    for i in $(seq ${TEST_CASE_START} 988); do
        echo "noop" >>4.txt
    done
    echo "n00p" >>4.txt
    for i in $(seq ${TEST_CASE_START} 9); do
        echo "noop" >>4.txt
    done
    echo "number | cat" >>4.txt
    echo "exit" >>4.txt
    echo "" >>4.txt
fi

cat 4.txt | ./npshell >demo/output/cat_4_self_gen.txt 2>&1
./npshell <4.txt >demo/output/red_4_self_gen.txt 2>&1
