all: test

LDFLAGS = -lneo4j-client -lssl -lcrypto -lm


test:
	$(CXX) neo4j_wrapper.cpp  -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -o neo4j_wrapper $(LDFLAGS)
