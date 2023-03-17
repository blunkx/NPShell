#! /bin/sh

cat demo/test_case/4.txt | ./npshell >demo/output/cat_4.txt 2>&1
echo "cat 4.txt done"

./npshell <demo/test_case/4.txt >demo/output/red_4.txt 2>&1
echo "redirect 4.txt done"
