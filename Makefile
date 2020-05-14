all: test

LDFLAGS=-lneo4j-client -lssl -lcrypto -lm

test:
        $(CXX)  test.cpp -o test $(LDFLAGS)





