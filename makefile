all: threads forks

threads:
	gcc -o sv_threads SudokuValidatorThreads.c -lpthread

forks:
	gcc -o sv_forks SudokuValidatorForks.c -lrt
