#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>

using namespace std;

#define sidelen 3				//sqrt of board size; change value for different sizes
#define size (sidelen*sidelen)

//represents each n*n block on the board
struct block {
	int space[size];
};

//class for representing the board
class board {
	block layout[size];

	public:
		board();
		void setrand(int, int);
		void setallrand();
		void printboard(ofstream&);
		bool conres(int, int);
		void solver();
};

//constructor
board::board()
{
	for (int i=0; i < size; i++) {
		for (int j=0; j < size; j++)
			layout[i].space[j] = 0;
	}
}

//sets specified position to a random number
void board::setrand(int blpos, int sqpos)
{
	int num;
	num = rand() % 9 + 1;
	layout[blpos].space[sqpos] = num;
}

//sets all spaces to random numbers
void board::setallrand()
{
	for (int i=0; i < size; i++) {
		for (int j=0; j < size; j++) {
			setrand(i, j);
		}
	}
}

//prints board to screen
void board::printboard(ofstream &outfile)
{
	int sqpos, blpos;

	sqpos = blpos = 0;
	for (int i=0; i < size*size; i++) {
		if (i > 0) {
			if (i % (size*sidelen) == 0) {
				sqpos = 0;
				blpos++;
				outfile << "\n";
				for (int j=0; j < size; j++) {
					if ((j % sidelen == 0) && (j > 0))
						outfile << "+";
					outfile << "-";
				}
				outfile << "\n";
			}
			else if (i % size == 0) {
				blpos -= sidelen - 1;
				outfile << "\n";
			}
			else if (i % sidelen == 0) {
				sqpos -= sidelen;
				outfile << "|";
				blpos++;
			}
		}
		outfile << layout[blpos].space[sqpos];
		sqpos++;
	}
	outfile << "\n";
}

//checks position's surroundings for conflicts; resolves to number with minimum conflicts if any
bool board::conres(int blpos, int sqpos)
{
	int count, min, array[size], *numlist, num, blrow, sqrow, blcol, sqcol;

	for (int i=0; i < size; i++)
		array[i] = 0;
	
	count = 0;
	for (int i=0; i < (size*3); i++) { 
		if (i < size) {								//For conflics in block:
			array[layout[blpos].space[i]-1]++;
		}
		else if (i < size*2) {							//For conflicts in row:
			if (i % size == 0) {
				blrow = blpos / sidelen;
				sqrow = sqpos / sidelen;
				blcol = blrow * sidelen;
				sqcol = sqrow * sidelen;
			}
			if (sqcol >= (sqrow * sidelen + sidelen)) {
				sqcol = sqrow * sidelen;
				blcol++;
			}
			if (blcol != blpos) {
				array[layout[blcol].space[sqcol]-1]++;
			}
			sqcol++;
		}
		else {										//For conflicts in column:
			if (i % size == 0) {
				blrow = blpos % sidelen;
				sqrow = sqpos % sidelen;
				sqcol = sqpos % sidelen;
			}
			if (sqrow >= size) {
				sqrow = sqcol;
				blrow += sidelen;
			}
			if (blrow != blpos) {
				array[layout[blrow].space[sqrow]-1]++;
			}
			sqrow += sidelen;
		}
	}
	array[layout[blpos].space[sqpos]-1]--;
	min = sidelen * 3;
	for (int i=0; i < size; i++) {
		if (array[i] < min) {
			count = 0;
			min = array[i];
		}
		if (array[i] == min)
			count++;
	}
	numlist = new int[count];
	count = 0;
	for (int i=0; i < size; i++) {
		if (array[i] == min) {
			numlist[count] = i+1;
			count++;
		}
	}
	if ((count == 1) && (layout[blpos].space[sqpos] == numlist[0]))
		return true;
	else if (count == 1)
		layout[blpos].space[sqpos] = numlist[0];
	else {
		do {
			num = rand() % count;
		} while (numlist[num] == layout[blpos].space[sqpos]);
		layout[blpos].space[sqpos] = numlist[num];
	}
	return false;
}

//checks random spots for conflicts; if conflict, its resolved and counter is set to 0; else counter is incremented
// exits when counter equals board size
void board::solver()
{
	int blpos, sqpos, count;
	bool array[size][size];
	while (count < size*size) {
		blpos = rand() % 8;
		sqpos = rand() % 8;
		for (int i=0; i < size; i++) {
			for (int j=0; j < size; j++) {
				if (conres(i, j))
					count++;
				else
					count = 0;
			}
		}
	}
}

//if program doesnt stop immediatly, exit and run again
// it only takes a fraction of a second to solve, but it sometimes gets stuck
int main()
{
	srand(time(NULL));

	board sudoku;
	ofstream file;

	file.open("output.txt", ofstream::in | ofstream::trunc);

	sudoku.setallrand();
	sudoku.printboard(file);
	sudoku.solver();
	file << "\n";
	sudoku.printboard(file);

	file.close();
	
	return 0;
}
