#compilo = gcc -g -Wall -W -Werror -Os -std=gnu11
compilo = clang -D_GNU_SOURCE -g -Os -std=gnu11 -Weverything -Werror -Wno-padded -Wno-disabled-macro-expansion -fcaret-diagnostics

executables = mini_shell

objets = $(patsubst %.c,%.o, $(wildcard *.c))

default: $(executables)

$(executables): %: $(objets)
	$(compilo) $^ -o $@

%.o: %.c $(wildcard *.h) Makefile
	$(compilo) -c $<

menage:
	rm -f *~ $(executables) *.o a.out core
