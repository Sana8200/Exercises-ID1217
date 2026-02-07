include ./make.def

EXES=fib-openmp fib-pthread fib-seq hello pi-seq matmul prod_cons linked

all: $(EXES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(EXES) *.o

