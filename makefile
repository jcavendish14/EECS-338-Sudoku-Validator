all: sv_threads sv_forks

sv_threads:
	gcc -o sv_threads SudokuValidatorThreads.c -lpthread

sv_forks:
	gcc -o sv_forks SudokuValidatorForks.c -lrt
