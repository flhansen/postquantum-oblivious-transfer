CC=gcc
CFLAGS=-I.

test: test/test_ot.c test/test.c src/math.c src/ot.c
	mkdir -p ./build/tests
	$(CC) -o build/tests/test_ot test/test_ot.c test/test.c src/math.c src/ot.c src/crypto.c -lgmp -lcrypto -lssl
