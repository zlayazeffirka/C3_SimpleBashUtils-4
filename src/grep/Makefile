SHELL:= /bin/bash
CC=gcc -Wall -Werror -Wextra -std=c11
EXE=s21_grep

all: s21_grep

clean: 
	rm -rf $(EXE)

rebuild: clean all

s21_grep: s21_grep.c s21_grep_functions.c
	$(CC) -o $(EXE) s21_grep.c s21_grep_functions.c

f: s21_grep.c s21_grep_functions.c
	$(CC) -o $(EXE) s21_grep.c s21_grep_functions.c -fsanitize=address
tests:
	-diff <(./s21_grep str hello.txt) <(grep str hello.txt)
	-diff <(./s21_grep -estr -e 8 hello.txt) <(grep -estr -e 8 hello.txt)
	-diff <(./s21_grep -i Str hello.txt) <(grep -i Str hello.txt)
	-diff <(./s21_grep -v str hello.txt) <(grep -v str hello.txt)
	-diff <(./s21_grep -c str hello.txt) <(grep -c str hello.txt)
	-diff <(./s21_grep -n str hello.txt) <(grep -n str hello.txt)
	-diff <(./s21_grep -l str hello.txt hello2.txt hello3.txt) <(grep -l str hello.txt hello2.txt hello3.txt)
	-diff <(./s21_grep -s str hello20.txt) <(grep -s str hello20.txt)
	-diff <(./s21_grep -f hello.txt hello3.txt) <(grep -f hello.txt hello3.txt)
	-diff <(./s21_grep -o str hello.txt) <(grep -o str hello.txt)