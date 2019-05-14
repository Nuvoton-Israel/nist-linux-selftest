all:
	$(CC) nist-linux-selftest.c -o nist-linux-selftest

.PHONY: clean
clean:
	rm -f nist-linux-selftest
