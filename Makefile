CFLAGS ?= -Wall -Wextra -Werror -O3 -std=gnu99 -s

gamma: gamma.c
	$(CC) $(CFLAGS) $< -o $@

install: gamma
	install gamma $(HOME)/.local/bin/gamma

clean:
	$(RM) gamma
