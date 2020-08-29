# compile Q1a.c
Q1a: Q1a.c
	mpicc -std=c99 -lm Q1a.c -o Q1a

# run Q1a
run_Q1a: Q1a
	mpirun Q1a 100 10

# compile Q1b.c
Q1b: Q1b.c
	mpicc -std=c99 -lm Q1b.c -o Q1b

# run Q1b
run_Q1b: Q1b
	mpirun Q1b 100 10

# clean the directory
clean:
	rm -f Q1a Q1b
