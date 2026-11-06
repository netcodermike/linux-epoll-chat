CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Werror -std=c11
app: main.c
	$(CC) $(CFLAGS) main.c -o app
test: app
	python3 tests/integration.py
clean:
	rm -f app
.PHONY: clean test
