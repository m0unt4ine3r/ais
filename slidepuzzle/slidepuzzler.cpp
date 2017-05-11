//A* slide puzzle solver

/*
-needs to gen possible branches from cur state as nodes
-needs heuristic for use with path cost
-step cost = 1
-need to figure out how to properly manipulate when represented as 0123456789
	-maybe 3d array as 012|345|678
-optimal path == f(n) = heuristic(n) + path cost(n) (where f(n) < all other f(n)'s)
-needs to output text file called states.txt of path to goal
-needs to check for ilegal states
*/

/*
Steps:
-gen possible nodes from current state
-compare to goal
-expand next node w/ lowest f(n) and push other nodes into stack w/ associated f(n)
	-if expanding to something on stack pop node off stack(?) 
	-push nodes to stack tracking path
-repeat until match or until stack is empty
-display nodes on path stack
*/

#include<iostream>
using namespace std;

#define sidelen 3
#define size (sidelen*sidelen)


////node def
//struct node {
//	char spot;
//	node *next;
//};
//
//
////stack def
//struct stack {
//	node *top = 0;
//	int count = 0;
//	
//	//push
//	void push(node *parent)
//	{
//		node *cur;
//		cur = parent;
//		
//		if (top == 0)
//			top = cur;
//		else {
//			cur->next = top;
//			top = cur;
//		}
//		count++;
//	}
//	
//	//pop
//	node *pop()
//	{
//		node *prev;
//	
//		if (count == 0) 
//			return 0;
//		else {
//			prev = top;
//			top = top->next;
//			return prev;
//		}
//		count--;
//	}
//	
//	//empty stack
//	void flush()
//	{
//		top = 0;
//		count = 0;
//	}
//	
//	//display
//	void display()
//	{
//		if (top != 0) {
//			while (top->next) {
//				cout << top->spot;
//				top = top->next;
//			}
//		}
//		else
//			cout << "Empty";
//	}
//};


//class for board
class board {
	int blank[2];
	board *self, *parent, *next[4];

public:
	char state[sidelen][sidelen];			//maybe switch to ptr notation to make private ie *state and index accordingly in setboard

	void setboard(char start[]);
	bool solvable();
	void setself(board *selfparam);
	void up();
	void down();
	void left();
	void right();
	void genstates();
	void getcur();
	void getkid(int);
};

//sets the board based on an array given as an arg (maybe change to constructor)
void board::setboard(char start[])
{
		int x = 0, y = 0;

		for (int i=0; i < size; i++) {
			if (start[i] == '0') {
				blank[0] = x;
				blank[1] = y;
			}

			if (x > sidelen-1) {
				y++;
				x = 0;
			}

			state[x][y] = start[i];

			x++;
		}

		for (int i=0; i < 4; i++)
			next[i] = 0;

		parent = 0;
}


bool board::solvable()
{
	int pos1, x, y, pos2, i, j, count;

	x = i = count = 0;
	for (pos1=0; pos1 < size; pos1++) {	//algorithm to check solvability
		x = pos1 % sidelen;

		if (state[y][x] > i) {
			for (int i=x+1; j < start[y][x]; i++) {
				i = pos2 % sidelen;

				if (start[i] < start[y][x])
					count++;
			}
		}

		if (x == sidelen)
			y++;
	}

	if (count % 2 == 0)
		return 1;
	else {
		cout << "Board is not solvable\n";
		return 0;
	}
}

//sets self
void board::setself(board *selfparam)
{
	self = selfparam;
}

//gen board node based on moving blank space right
void board::right()
{
	next[0] = new board;

	int x=0, y=0;

	for (int i=0; i < size; i++) {
		if (x > sidelen-1) {
			y++;
			x = 0;
		}

		next[0]->state[x][y] = state[x][y];

		x++;
	}

	next[0]->parent = self;
	next[0]->self = next[0];
	for (int i=0; i < 4; i++)
		next[0]->next[i] = 0;

	next[0]->state[blank[0]][blank[1]] = next[0]->state[blank[0]+1][blank[1]];
	next[0]->state[blank[0]+1][blank[1]] = '0';

	next[0]->blank[0] = blank[0] + 1;
	next[0]->blank[1] = blank[1];
}

//gen board node based on moving blank space left
void board::left()
{
	next[1] = new board;

	int x=0, y=0;

	for (int i=0; i < size; i++) {
		if (x > sidelen-1) {
			y++;
			x = 0;
		}

		next[1]->state[x][y] = state[x][y];

		x++;
	}

	next[1]->parent = self;
	next[1]->self = next[1];
	for (int i=0; i < 4; i++)
		next[1]->next[i] = 0;

	next[1]->state[blank[0]][blank[1]] = next[1]->state[blank[0]-1][blank[1]];
	next[1]->state[blank[0]-1][blank[1]] = '0';

	next[1]->blank[0] = blank[0] - 1;
	next[1]->blank[1] = blank[1];
}

//gen board node based on moving blank space down
void board::down()
{
	next[2] = new board;

	int x=0, y=0;

	for (int i=0; i < size; i++) {
		if (x > sidelen-1) {
			y++;
			x = 0;
		}

		next[2]->state[x][y] = state[x][y];

		x++;
	}

	next[2]->parent = self;
	next[2]->self = next[1];
	for (int i=0; i < 4; i++)
		next[2]->next[i] = 0;

	next[2]->state[blank[0]][blank[1]] = next[2]->state[blank[0]][blank[1]+1];
	next[2]->state[blank[0]][blank[1]+1] = '0';

	next[2]->blank[0] = blank[0];
	next[2]->blank[1] = blank[1] + 1;
}

//gen board node based on moving blank space up
void board::up()
{
	next[3] = new board;

	int x=0, y=0;

	for (int i=0; i < size; i++) {
		if (x > sidelen-1) {
			y++;
			x = 0;
		}

		next[3]->state[x][y] = state[x][y];

		x++;
	}

	next[3]->parent = self;
	next[3]->self = next[1];
	for (int i=0; i < 4; i++)
		next[3]->next[i] = 0;

	next[3]->state[blank[0]][blank[1]] = next[3]->state[blank[0]][blank[1]-1];
	next[3]->state[blank[0]][blank[1]-1] = '0';

	next[3]->blank[0] = blank[0];
	next[3]->blank[1] = blank[1] - 1;
}

//generates possible moves based on blank square position
void board::genstates()
{
	if (!(blank[0] >= sidelen-1)) {
		right();
	}
	
	if (!(blank[0] <= 0)) {
		left();
	}

	if (!(blank[1] >= sidelen-1)) {
		down();
	}

	if (!(blank[1] <= 0)) {
		up();
	}
}

//prints current board state
void board::getcur()
{
	int x, y;
	x = y = 0;

	for (int i=0; i < size; i++) {
		if (x > sidelen-1) {
			y++;
			x = 0;
			cout << "\n";
		}

		cout << self->state[x][y];
		x++;
	}

	cout << "\n\n";
}

//prints board of children nodes
void board::getkid(int kidi)	//add way to specify
{
	for (int j=0; j < 4; j++) {
		if (next[j]) {
			int x=0, y=0;

			for (int i=0; i < size; i++) {
				if (x > sidelen-1) {
					y++;
					x = 0;
					cout << "\n";
				}
		
				cout << next[j]->state[x][y];
		
				x++;
			}
		
			cout << "\n";
		}

		cout << "\n";
	}
}

/*

//node traversal
traverse()
{
	board *cur;
	
	genstates()
	for (int i=0; i < 4; i++) {
		if ((next[i]) && (unvisited))
			return next[i];
	}
}
//needs to have stack of all unvisited nodes

*/


/*
//convert into string for node
char *str;
*/

void crawler(board *start)
{
	
}

//main
int main()
{
	char init[]={'6','4','3','1','0','8','2','7','5'};

	board *begin = new board;
	begin->setboard(init);
	begin->setself(begin);


	cout << "\n";

	begin->genstates();
	begin->getcur();
	begin->getkid(1);

	return 0;
}
