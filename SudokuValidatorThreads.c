/**
* Authors: Maya Nayak & Jesse Cavendish
* This program implements a sudoku validator using threads.
* November 29th, 2017
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FALSE 0
#define TRUE 1

typedef struct {
	int row;
	int column;
	int (* board)[9];
} parameters;

void * checkAllRows(void * params);

void * checkAllColumns(void * params);

void * checkSubgrid(void * params);

int allValid(int checked[]);

int main(void) {
	//create default board for now, later on we can take user input or make this part dynamic
	int board[9][9] = {
		{ 8,2,5,4,7,1,3,9,6 },
		{ 1,9,4,3,2,6,5,7,8 },
		{ 3,7,6,9,8,5,2,4,1 },
		{ 5,1,9,7,4,3,8,6,2 },
		{ 6,3,2,5,9,8,4,1,7 },
		{ 4,8,7,6,1,2,9,3,5 },
		{ 2,6,3,1,5,9,7,8,4 },
		{ 9,4,8,2,6,7,1,5,3 },
		{ 7,5,1,8,3,4,6,2,9 }
	};

	// initialize data to be used in each thread
	parameters data[10];
	// which row each thread needs to start on
	int rows[10] = {0, 0, 0, 0, 3, 3, 3, 6, 6, 6};
	// which column eeach thread needs to start on
	int columns[10] = {0, 0, 3, 6, 0, 3, 6, 0, 3, 6};

	// assign data to each parameter struct
	for(int i = 0; i <= 9; i++) {
		data[i].row = rows[i];
		data[i].column = columns[i];
		data[i].board = board;
	}

	//initialize threads
	pthread_t thread[11];

	//create threads
	pthread_create(&thread[0], NULL, checkAllRows, &data[0]);
	pthread_create(&thread[1], NULL, checkAllColumns, &data[0]);
	for(int i = 1; i <= 9; i++) {
		pthread_create(&thread[i + 1], NULL, checkSubgrid, &data[i]);
	}

	//initialize pointers
	void * isValid = NULL;
	int checked[11];

	//wait for threads to finish, then join them
	for(int i = 0; i <= 10; i++) {
		pthread_join(thread[i], &isValid);
		if(isValid) {
			checked[i] = TRUE;
		} else {
			checked[i] = FALSE;
		}
	}

	//if each subregion contains 1-9, sudoku is valid. Else, it is in invalid.
	if (allValid(checked)) {
		printf("This Sudoku Puzzle is valid.\n");
	} else {
		printf("This Sudoku Puzzle is invalid. Better luck next time.\n");
	}

	return 0;

}

void * checkAllRows(void * params) {
	parameters * data = (parameters *)params;
	int row = data->row;
	int column = data->column;
	for (int i = row; i < 9; i++) {
		int rowNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < 9; j++) {
			int val = data->board[i][j];
			if (rowNumbers[val - 1] == 1) {
				printf("%d, %d", i, j);
				return (void *) FALSE;
			}
			else
				rowNumbers[val - 1] = 1;
		}
	}
	return (void *) TRUE;
}

void * checkAllColumns(void * params) {
	parameters * data = (parameters *)params;
	int row = data->row;
	int column = data->column;
	for (int i = column; i < 9; i++) {
		int colNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = row; j < 9; j++) {
			int val = data->board[i][j];
			if (colNumbers[val - 1] == 1) {
				return (void *) FALSE;
			}
			else
				colNumbers[val - 1] = 1;
		}
	}
	return (void *) TRUE;
}

void * checkSubgrid(void * params) {
	parameters * data = (parameters *)params;
	int row = data->row;
	int column = data->column;
	for (int i = row; i < row + 3; i++) {
		int subgridNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < column + 3; j++) {
			int val = data->board[i][j];
			if (subgridNumbers[val - 1] == 1) {
				return (void *) FALSE;
			}
			else
				subgridNumbers[val - 1] = 1;
		}
	}
	return (void *) TRUE;
}

int allValid(int checked[]) {
	for(int i = 0; i < 11; i++) {
		if(!checked[i]) {
			return FALSE;
		}
	}
	return TRUE;
}
