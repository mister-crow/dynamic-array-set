CFLAGS := -I./include -std=c++11 -pedantic -Wall -Wextra -Werror -g
CC:=g++ $(CFLAGS)

.PHONY: default
default: ./bin/test_app


HEADERS := \
	include/block_container.h \
	include/block_allocator.h \
	include/dynamic_set.h \
	include/test.h

./bin:
	mkdir ./bin

./obj:
	mkdir ./obj

./obj/test_app.o: ./src/test_app.cpp $(HEADERS) makefile ./bin ./obj
	$(CC) -c $< -o $@

./bin/test_app: ./obj/test_app.o
	$(CC) $^ -o $@


.PHONY: clean
clean:
	rm -f ./bin/* ./obj/*
