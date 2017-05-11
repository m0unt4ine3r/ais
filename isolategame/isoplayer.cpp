#include<iostream>
using namespace std;

#define side 4
#define size (side*side)

class board {
	char spaces[16];	//should change if decide to define size in main;

	public:
		board();
		void display();
		void move(char, int);
		int *genmoves(int);
};

board::board()
{
	for (int i=0; i < size; i++)
		spaces[i] = '0';
}

void board::display()
{
	for (int i=0; i < size; i++) {
		if (!(i % 4) && (i > 0)) 
			cout << "\n-+-+-+-\n"; //needs to be abstracted
		else if ((i > 0) && (i%4 < side))
			cout << "|";
		cout << spaces[i];
	}
	
	cout << "\n";
}

void board::move(char piece, int location)
{
	bool valid = 0;
	if (spaces[location] == '0') {
		spaces[location] = piece;
//		spaces[previos_spot] = '-';
	}
	else
		cout << "Invalid move, try another.";
}

//
int *board::genmoves(int pos)
{
	//needs to account for move direction; also, maybe move above space validation here
	//x dir
	//get row by dividing pos by sidelen (4)
	int row = pos / 4; //4 = side
	//get col by pos modulo sidelen (4)
	int col = pos % 4;
	//# of diags:
	int cross = 0;
	int numdiag = 0;
	int i, j;
	i = j = pos;
	int diagarray[5];	//both counts excluding pos b/c cant move to same sq
	int mainarray[6];
	int *moves;

	//everything below needs to be checked for '-'
	int x = row * 4;
	while ((x < row * 4 - 1) && (y < 16)) {
	for ( x < 16; x++) {
		if (x != pos) {
			mainarray[x] = x;
			x++;
	}

	for (int y = col; y < 16; y += 4) {
		if (y != pos)
			mainarray[y] = y;
	}
	}
	
	while (((((i - 3) / 4) != (i / 4)) && (i >= 0)) && ((((j - 5) / 4) != (j / 4)) && (j >= 0))) {
		if (i > 0)
			i -= 3;
		if (j > 0)
			j -= 5;
	}

	if (i < 0)
		i += 3;
	if (j < 0)
		j += 5;

	while (((((i + 3) / 4) != (i / 4)) && (i < 16)) && (((((j + 5) / 4) - (j / 4)) >= 2) && (j < 16))) {
		if ((i != pos) && (i < 15)) {
			diagarray[numdiag] = i;
			numdiag++;
		}
		if ((j != pos) && (i < 15)) {
			diagarray[numdiag] = j;
			numdiag++;
		}
		i += 3;
		i += 5;
	}

	int maini = diagi = 0;
	for (int x=0; x < cross+numdiag; x++) {
		if (mainarray[x] < diagarray[x]) {
			moves[x] = mainarray[maini];
			maini++;
		}
		else {
			moves[x] = diagarray[diagi];
			diagi++;
		}
	}
	return next; //placeholder
}

class allknowing {	//will move based on pre-genned map
	
};

class lateral {	//will move gen states on the fly

};

int main()
{

	board field;
	field.display();
	return 0;
}
