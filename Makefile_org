all: test

LDFLAGS = -lneo4j-client -lssl -lcrypto -lm


test: libneo4jwrapper.so
	$(CXX) driver.cpp -L. -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -lneo4jwrapper -o neo4j_wrapper_test $(LDFLAGS)

libneo4jwrapper.so: neo4j_wrapper.o
	$(CXX) -dylib -shared  -fpic  -undefined dynamic_lookup $^ -o $@

%.o: %.cpp
	$(CXX) -c  $^ -o $@
