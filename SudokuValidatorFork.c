/**
* Authors: Maya Nayak & Jesse Cavendish
* This program implements a sudoku validator using threads.
* November 29th, 2017
*
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>

#define SIZE 4096

typedef struct {
	int row;
	int column;
	int (* board)[9];
} parameters;

int checkAllRows(parameters * nums);

int checkAllColumns(parameters * nums);

int checkSubgrid(parameters * nums);

int checkIsValid(int isValid[]);

int main(void) {
	//create default board for now, later on we can take user input or make this part dynamic
	int shm_fd;
	int *board[9][9] = {
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

	int shm_id;
	key_t mem_key;
	int *isValid;

	mem_key = ftok(".", 'a');
	shm_id = shmget(mem_key, 11, IPC_CREAT | 0666);
	
	isValid = (int *)shmat(shm_id, NULL, 0);
	parameters data[10];

	//create data pointer for columns and rows
	parameters *data0 = (parameters *)malloc(sizeof(parameters));
	data0->row = 0;
	data0->column = 0;
	data0->board = board;
	data[0] = *data0;


	//create data pointer for 3x3 subgrid 1
	parameters *data1 = (parameters *)malloc(sizeof(parameters));
	data1->row = 0;
	data1->column = 0;
	data1->board = board;
	data[1] = *data1;


	//create data pointer for 3x3 subgrid 2
	parameters *data2 = (parameters *)malloc(sizeof(parameters));
	data2->row = 0;
	data2->column = 0;
	data2->board = board;
	data[2] = *data2;

	//create data pointer for 3x3 subgrid 3
	parameters *data3 = (parameters *)malloc(sizeof(parameters));
	data3->row = 0;
	data3->column = 6;
	data3->board = board;
	data[3] = *data3;

	//create data pointer for 3x3 subgrid 4
	parameters *data4 = (parameters *)malloc(sizeof(parameters));
	data4->row = 3;
	data4->column = 0;
	data4->board = board;
	data[4] = *data4;

	//create data pointer for 3x3 subgrid 5
	parameters *data5 = (parameters *)malloc(sizeof(parameters));
	data5->row = 3;
	data5->column = 3;
	data5->board = board;
	data[5] = *data5;

	//create data pointer for 3x3 subgrid 6
	parameters *data6 = (parameters *)malloc(sizeof(parameters));
	data6->row = 3;
	data6->column = 6;
	data6->board = board;
	data[6] = *data6;

	//create data pointer for 3x3 subgrid 7
	parameters *data7 = (parameters *)malloc(sizeof(parameters));
	data7->row = 6;
	data7->column = 0;
	data7->board = board;
	data[7] = *data7;

	//create data pointer for 3x3 subgrid 8
	parameters *data8 = (parameters *)malloc(sizeof(parameters));
	data8->row = 6;
	data8->column = 3;
	data8->board = board;
	data[8] = *data8;

	//create data pointer for 3x3 subgrid 9
	parameters *data9 = (parameters *)malloc(sizeof(parameters));
	data9->row = 6;
	data9->column = 6;
	data9->board = board;
	data[9] = *data9;

	pid_t pid;

	pid = fork();
	if(pid == 0) {
		isValid[0] = checkAllRows(&data[0]);
		exit(0);
	} else {
		pid = fork();
		if(pid == 0) {
			isValid[1] = checkAllColumns(&data[0]);
			exit(0);
		} else {
			for(int i = 1; i < 10; i++) {
				pid = fork();
				if(pid == 0) {
					isValid[i + 1] = checkSubgrid(&data[i]);
					exit(0);
				}
			}
		}
	}

	for(int i = 0; i < 11; i++) {
		wait(NULL);
	}


	//if each subregion contains 1-9, sudoku is valid. Else, it is in invalid.
	if(checkIsValid(isValid))
	{
		printf("This Sudoku Puzzle is valid.");
	}
	else 
	{
		printf("This Sudoku Puzzle is invalid. Better luck next time.");
	}

	shmdt(isValid);
	return 0;

}

int checkAllRows(parameters * nums) {
	int row = nums->row;
	int column = nums->column;
	for (int i = row; i < 9; i++) {
		int rowNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < 9; j++) {
			int val = nums->board[i][j];
			if (rowNumbers[val - 1] == 1) {
				return 0;
			}
			else
				rowNumbers[val - 1] = 1;
		}
	}
	return 1;
}

int checkAllColumns(parameters * nums) {
	int row = nums->row;
	int column = nums->column;
	for (int i = column; i < 9; i++) {
		int colNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = row; j < 9; j++) {
			int val = nums->board[i][j];
			if (colNumbers[val - 1] == 1) {
				return 0;
			}
			else
				colNumbers[val - 1] = 1;
		}
	}
	return 1;
}

int checkSubgrid(parameters * nums) {
	int row = nums->row;
	int column = nums->column;
	for (int i = row; i < row + 3; i++) {
		int subgridNumbers[] = {0,0,0,0,0,0,0,0,0};
		for (int j = column; j < column + 3; j++) {
			int val = nums->board[i][j];
			if (subgridNumbers[val - 1] == 1) {
				return 0;
			}
			else
				subgridNumbers[val - 1] = 1;
		}
	}
	return 1;
}

int checkIsValid(int isValid[]) {
	for(int i = 0; i < 11; i++) {
		printf("%d", isValid[i]);
		if(&isValid[i] == 0) {
			return 0;
		}
	}
	return 1;
}
