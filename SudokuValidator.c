/**
* Authors: Maya Nayak & Jesse Cavendish
* This program implements a sudoku validator using threads.
* November 29th, 2017
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int row;
	int column;
	int (* board)[9];
} parameters;

void * checkAllRows(void * params);

void * checkAllColumns(void * params);

void * checkSubgrid(void * params);

int main(void) {
	//create default board for now, later on we can take user input or make this part dynamic
	int board[9][9] = {
		{ 9,2,5,4,7,1,3,9,6 },
		{ 1,9,4,3,2,6,5,7,8 },
		{ 3,7,6,9,8,5,2,4,1 },
		{ 5,1,9,7,4,3,8,6,2 },
		{ 6,3,2,5,9,8,4,1,7 },
		{ 4,8,7,6,1,2,9,3,5 },
		{ 2,6,3,1,5,9,7,8,4 },
		{ 9,4,8,2,6,7,1,5,3 },
		{ 7,5,1,8,3,4,6,2,9 }
	};

	//create data pointer for columns and rows
	parameters *data0 = (parameters *)malloc(sizeof(parameters));
	data0->row = 0;
	data0->column = 0;
	data0->board = board;


	//create data pointer for 3x3 subgrid 1
	parameters *data1 = (parameters *)malloc(sizeof(parameters));
	data1->row = 0;
	data1->column = 0;
	data1->board = board;


	//create data pointer for 3x3 subgrid 2
	parameters *data2 = (parameters *)malloc(sizeof(parameters));
	data2->row = 0;
	data2->column = 3;
	data2->board = board;

	//create data pointer for 3x3 subgrid 3
	parameters *data3 = (parameters *)malloc(sizeof(parameters));
	data3->row = 0;
	data3->column = 6;
	data3->board = board;

	//create data pointer for 3x3 subgrid 4
	parameters *data4 = (parameters *)malloc(sizeof(parameters));
	data4->row = 3;
	data4->column = 0;
	data4->board = board;

	//create data pointer for 3x3 subgrid 5
	parameters *data5 = (parameters *)malloc(sizeof(parameters));
	data5->row = 3;
	data5->column = 3;
	data5->board = board;

	//create data pointer for 3x3 subgrid 6
	parameters *data6 = (parameters *)malloc(sizeof(parameters));
	data6->row = 3;
	data6->column = 6;
	data6->board = board;

	//create data pointer for 3x3 subgrid 7
	parameters *data7 = (parameters *)malloc(sizeof(parameters));
	data7->row = 6;
	data7->column = 0;
	data7->board = board;

	//create data pointer for 3x3 subgrid 8
	parameters *data8 = (parameters *)malloc(sizeof(parameters));
	data8->row = 6;
	data8->column = 3;
	data8->board = board;

	//create data pointer for 3x3 subgrid 9
	parameters *data9 = (parameters *)malloc(sizeof(parameters));
	data9->row = 6;
	data9->column = 6;
	data9->board = board;

	//initialize threads
	pthread_t thread_rows, thread_columns, thread_1, thread_2, thread_3, thread_4, thread_5, thread_6, thread_7, thread_8, thread_9;

	//initialize pointers
	void * rows;
	void * columns;
	void * subgrid1;
	void * subgrid2;
	void * subgrid3;
	void * subgrid4;
	void * subgrid5;
	void * subgrid6;
	void * subgrid7;
	void * subgrid8;
	void * subgrid9;

	//create threads
	pthread_create(&thread_rows, NULL, checkAllRows, (void *)data0);
	pthread_create(&thread_columns, NULL, checkAllColumns, (void *)data0);
	pthread_create(&thread_1, NULL, checkSubgrid, (void *)data1);
	pthread_create(&thread_2, NULL, checkSubgrid, (void *)data2);
	pthread_create(&thread_3, NULL, checkSubgrid, (void *)data3);
	pthread_create(&thread_4, NULL, checkSubgrid, (void *)data4);
	pthread_create(&thread_5, NULL, checkSubgrid, (void *)data5);
	pthread_create(&thread_6, NULL, checkSubgrid, (void *)data6);
	pthread_create(&thread_7, NULL, checkSubgrid, (void *)data7);
	pthread_create(&thread_8, NULL, checkSubgrid, (void *)data8);
	pthread_create(&thread_9, NULL, checkSubgrid, (void *)data9);

	//wait for threads to finish, then join them
	pthread_join(thread_rows, &rows);
	pthread_join(thread_columns, &columns);
	pthread_join(thread_1, &subgrid1);
	pthread_join(thread_2, &subgrid2);
	pthread_join(thread_3, &subgrid3);
	pthread_join(thread_4, &subgrid4);
	pthread_join(thread_5, &subgrid5);
	pthread_join(thread_6, &subgrid6);
	pthread_join(thread_7, &subgrid7);
	pthread_join(thread_8, &subgrid8);
	pthread_join(thread_9, &subgrid9);

	//if each subregion contains 1-9, sudoku is valid. Else, it is in invalid.
	if ((int)rows == 1 &&
		(int)columns == 1  &&
		(int)subgrid1 == 1 &&
		(int)subgrid2 == 1 &&
		(int)subgrid3 == 1 &&
		(int)subgrid4 == 1 &&
		(int)subgrid5 == 1 &&
		(int)subgrid6 == 1 &&
		(int)subgrid7 == 1 &&
		(int)subgrid8 == 1 &&
		(int)subgrid9 == 1)
	{
		printf("This Sudoku Puzzle is valid.");
	}
	else 
	{
		printf("This Sudoku Puzzle is invalid. Better luck next time.");
	}


	return 0;

}

void * checkAllRows(void * params) {
	parameters * nums = (parameters *)params;
	int row = nums->row;
	int column = nums->column;
	for (int i = row; i < 9; i++) {
		int rowNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < 9; j++) {
			int val = nums->board[i][j];
			if (rowNumbers[val - 1] == 1) {
				printf("%d, %d", i, j);
				return (void *) 0;
			}
			else
				rowNumbers[val - 1] = 1;
		}
	}
	return (void *) 1;
}

void * checkAllColumns(void * params) {
	parameters * nums = (parameters *)params;
	int row = nums->row;
	int column = nums->column;
	for (int i = column; i < 9; i++) {
		int colNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = row; j < 9; j++) {
			int val = nums->board[i][j];
			if (colNumbers[val - 1] == 1) {
				printf("%d, %d", i, j);
				return (void *) 0;
			}
			else
				colNumbers[val - 1] = 1;
		}
	}
	return (void *) 1;
}

void * checkSubgrid(void * params) {
	parameters * nums = (parameters *)params;
	int row = nums->row;
	int column = nums->column;
	for (int i = row; i < row + 3; i++) {
		int subgridNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < column + 3; j++) {
			int val = nums->board[i][j];
			if (subgridNumbers[val - 1] == 1) {
				printf("%d, %d", i, j);
				return (void *) 0;
			}
			else
				subgridNumbers[val - 1] = 1;
		}
	}
	return (void *) 1;
}
