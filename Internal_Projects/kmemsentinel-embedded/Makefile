CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -Iinclude
SRC = src/kmemsentinel.c src/kmemleak_live.c

all: demo test

demo: $(SRC) src/main.c include/kmemsentinel.h
	$(CC) $(CFLAGS) $(SRC) src/main.c -o kmems_demo

test: $(SRC) tests/test_kmems.c include/kmemsentinel.h
	$(CC) $(CFLAGS) $(SRC) tests/test_kmems.c -o kmems_test
	$(CC) $(CFLAGS) $(SRC) tests/test_kmemleak_live.c -o kmemleak_parser_test

run-demo: demo
	./kmems_demo

run-test: test
	./kmems_test
	./kmemleak_parser_test

clean:
	rm -f kmems_demo kmems_test kmemleak_parser_test

.PHONY: all demo test run-demo run-test clean
