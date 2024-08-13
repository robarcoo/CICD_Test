FLAGS=-std=c11 -Wall -Werror -Wextra -g
OS = $(shell uname)


all: clean s21_cat

s21_cat:
	gcc ${FLAGS} s21_cat.c -o s21_cat

test:
	bash s21_cat_test.sh

check: 
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -rf .clang-format
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./s21_cat
endif


clang:
	cp ../../materials/linters/.clang-format .
	clang-format -i *.c *.h
	rm -rf .clang-format


clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css s21_cat *.dSYM

