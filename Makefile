CC=clang
CFLAGS=-I.

tests: test/test_ot.c test/test.c src/math.c src/ot.c
	mkdir -p ./build/tests
	$(CC) $(CFLAGS) -o build/tests/test_ot test/test_ot.c test/test.c src/math.c src/ot.c src/pqot.c src/crypto.c -lgmp -lcrypto -lssl -Llibs -lsidh
	$(CC) $(CFLAGS) -o build/tests/test_math test/test_math.c test/test.c src/math.c

clean:
	rm -rf build/
