#!/bin/bash

DIFF_RES=""

for var in -v -n -h -i -s
do
  for var2 in -v -n -h -i -s
  do
      for var3 in -v -n -h -i -s
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="s21_grep.c s21_grep.h Makefile $var $var2 $var3"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$var $var2 $var3 success"
            else
              echo "$var $var2 $var3  FAILED"
          fi
          rm s21_grep.txt grep.txt

        fi
      done
  done
done

