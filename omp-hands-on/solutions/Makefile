
include ../make.def

EXES=hello_par pi pi_spmd_simple pi_spmd_final pi_loop \
     matmul_par prod_cons_par linked_omp3_tasks

all: $(EXES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(EXES) *.o
