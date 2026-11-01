obj=$(patsubst %.c,%.o,$(wildcard *.c))
a.out:$(obj)
	gcc -o $@ $^
clean:
	rm -f *.exe *.out *.o