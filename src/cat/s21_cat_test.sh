#!/bin/bash

DIFF_RES=""

for var in -b -e -n -s -t 
do
  for var2 in -b -e -n -s -t
  do
      for var3 in -b -e -n -s -t
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="$var $var2 $var3 test.txt"
          ./s21_cat $TEST1 > s21_cat.txt
          cat $TEST1 > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo "$var $var2 $var3 success"
            else
              echo "$var $var2 $var3  FAILED"
          fi
          rm s21_cat.txt cat.txt


        fi
      done
  done
done
