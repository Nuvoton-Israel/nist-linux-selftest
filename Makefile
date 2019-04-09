all:
	$(CC) obmc-selftest.c -o obmc-selftest

.PHONY: clean
clean:
	rm -f obmc-selftest
