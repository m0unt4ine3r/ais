// Index:
// brd_def
// brd_ini
// brd_set
// brd_uset
// brd_fee
// brd_mov
// brd_umov
// brd_sho
// brd_upd
// brd_pri
// brd_glay
// agt_def
// agt_ini
// agt_feed
// agt_mov
// agt_pri
// agt_go

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////
//Game class 
//[brd_def]
class board {
	int layout[4][4];
	int xpos, ypos;
	int arrows;
	int exit;
	char overlay[4][4];

	public:
		board();
		void setboard(int, int, int);
		void usersetb();
		int feedback();
		int move(char);
		int usermove();
		int shoot(char dir=0);
		char updatelayout();
		void print();
		int *getlayout();
};

//Constructor; inits board 
//[brd_con]
board::board()
{
	xpos = ypos = 0;
	arrows = 1;
	exit = 0;

	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++) {
			layout[i][j] = 0;
		}
	}

	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++) {
			overlay[i][j] = '?';
		}
	}
}

//Mutator for board 
//[brd_set]							//some codes:
void board::setboard(int flag, int i, int j) //-8 - gold; -1 - glitter; 0 - safe; 1 - unknown; 2 - breeze; 4 - stench; 16 - pit; 32 - wumpus (dead wumpus = 0)
									//1 - pit/glitter (theoretically); 3 - stench/glitter; 5 - breeze/stench/glitter; 6 - breeze/stench; 
{
		layout[i][j] = flag;
		if (i < 3) {
			if (layout[i+1][j] == 0)
				layout[i+1][j] = flag/8;
			else
				layout[i+1][j] += flag/8;
		}
		if (j < 3) {
			if (layout[i][j+1] == 0)
				layout[i][j+1] = flag/8;
			else
				layout[i][j+1] += flag/8;
		}
		if (i > 0) {
			if (layout[i-1][j] == 0)
				layout[i-1][j] = flag/8;
			else
				layout[i-1][j] += flag/8;
		}
		if (j > 0) {
			if (layout[i][j-1] == 0)
				layout[i][j-1] = flag/8;
			else
				layout[i][j-1] += flag/8;
		}
}

//UI for board mutator 
//[brd_uset]
void board::usersetb()
{
	int i, j, flag;
	char thing;

	while (1) {
		do {
			flag = -9;
			cin.clear();
			cout << "Enter a flag to set: ((w)umpus|(p)it|(g)old|(d)one)\n-> ";
			cin >> thing;
			switch (thing) {
				case 'w':	flag = 32;
						break;
				case 'p':	flag = 16;
						break;
				case 'g':	flag = -8;
						break;
				case 'd':	flag = -1;
						break;
				default:	cout << "Please enter w, p, g, or d\n";
			}
		} while (flag == -9);

		if (flag == -1)
			break;
		
		do {
			cout << "Enter a x coordinate (0-3): ";
			cin >> j;
		} while ((j > 3) || (j < 0));
		do {
			cout << "Enter a y coordinate (0-3): ";
			cin >> i;
		} while ((i > 3) || (i < 0));

		setboard(flag, i, j);
	}
}

//Sensor data reponse 
//[brd_fee]
int board::feedback()
{
	if (layout[ypos][xpos] >= 16)
		return 256;
	else
		return layout[ypos][xpos];
}

//Movement rules 
//[brd_mov]
int board::move(char dir)
{
	switch (dir) {
		case 'u':	if (ypos < 3) {
					ypos++;
					exit = 0;
				}
				else
					exit = 1;
				break;
		case 'r':	if (xpos < 3) {
					xpos++;
					exit = 0;
				}
				else
					exit = 1;
				break;
		case 'd':	if (ypos > 0) {
					ypos--;
					exit = 0;
				}
				else
					exit = 1;
				break;
		case 'l':	if (xpos > 0) {
					xpos--;
					exit = 0;
				}
				else
					exit = 1;
				break;
		case 's':	if (arrows > 0) {	
					do {
						shoot();
						if (exit > 0)
							cout << "cant shoot there\n";
					} while (exit > 0);
					break;
				}
		default:	exit = 1;
	}

	return exit;
}

//UI for movement (not used)
//[brd_umov]
int board::usermove()
{
	char dir;

	cout << "\nWhere will you move? (u, d, l, r, s)\n-> ";
	cin >> dir;

	return move(dir);
}


//Shooting rules 
//[brd_sho]
int board::shoot(char dir)
{
	int i, j;

	if (dir == 0) {
		cout << "\nShoot where? (u, d, l, r, q)\n-> ";
		cin >> dir;
	}

	switch (dir) {
		case 'u':	if (ypos < 3) {
					arrows--;
					for (i=ypos; i < 4; i++) {
						if (layout[i][xpos] == 32) {
							exit = -1;
							j = xpos;
							break;
						}
					}
				}
				else
					exit = 1;
				break;
		case 'r':	if (xpos < 3) {
					arrows--;
					for (j=xpos; j < 4; j++) {
						if (layout[ypos][j] == 32) {
							exit = -1;
							i = ypos;
							break;
						}
					}
				}
				else
					exit = 1;
				break;
		case 'd':	if (ypos > 0) {
					arrows--;
					for (i=ypos; i < 0; i--) {
						if (layout[i][xpos] == 32) {
							exit = -1;
							j = xpos;
							break;
						}
					}
				}
				else
					exit = 1;
				break;
		case 'l':	if (xpos > 0) {
					arrows--;
					for (j=xpos; j > 0; j--) {
						if (layout[ypos][j] == 32) {
							exit = -1;
							i = ypos;
							break;
						}
					}
				}
				else
					exit = 1;
				break;
		case 'q': exit = -2;
				break;
		default:	exit = 1;
	}

	if (exit == -1) {
		exit = 0;
		setboard(0, i, j);
		cout << "You hear a scream\n";
	}
	return exit;
}

//Displays board (not used)
//[brd_pri]
void board::print()
{
	int code;
	for (int i=3; i >= 0; i--) {
		for (int j=0; j < 4; j++) {
			code = layout[i][j];
			if ((i == ypos) && (j == xpos))
					cout << "x";
			else
				cout << code;

			if ((j == 3) && (i > 0))
				cout << "\n-+-+-+-\n";

			if (j < 3)
				cout << "|";
		}
	}
	cout << "\n";
}

///////////////////////////////////////////////////////////////////////////////////
//AI class 
//[agt_def]
class agent {
	int x, y, flag, shots, misbehave;
	int knowledge[4][4];
	int movelist[4];
	char shoot;
	int wx, wy;
	ofstream pbp;

	public:
		agent();
		void genmoves();
		void assess(board);
		char calcmove();
		void print(board);
		int play(board);
};

//constructor; inits agent's view of the board
//[agt_ini]
agent::agent()
{
	x = y = flag = shoot = misbehave = 0;
	shots = 1;
	pbp.open("playbyplay.txt");

	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++)
			knowledge[i][j] = 1;
	}
}

//updates agents view of board based on previous and current percepts
//[agt_feed]
void agent::assess(board field)
{
	int wumpii = 0;
	int stenches = 0;
	int pits = 0;

	//board calculation section
	flag = knowledge[y][x] = field.feedback();

	if (flag > 34)			//quick fix for flags getting too high
		flag = 1;

	if ((flag > 1) || (flag == -1)) {			//guesses contents of nearby squares
		if (knowledge[y][x+1] == 1) 
			knowledge[y][x+1] = flag * 8;
		if (knowledge[y][x-1] == 1)
			knowledge[y][x-1] = flag * 8;
		if (knowledge[y+1][x] == 1)
			knowledge[y+1][x] = flag * 8;
		if (knowledge[y-1][x] == 1)
			knowledge[y-1][x] = flag * 8;
	}

	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++) {
			if (knowledge[i][j] > 34)		//quick fix for flags getting too high
				knowledge[i][j] = knowledge[i][j]/8;

			if (knowledge[i][j] == 0) {		//resolves initial guesses and cleans any spaces that are next to 0s
				if ((knowledge[i][j+1] > 6) || (knowledge[i][j+1] < -1))
					knowledge[i][j+1] = 1;
				if ((knowledge[i][j-1] > 6) || (knowledge[i][j-1] < -1))
					knowledge[i][j-1] = 1;
				if ((knowledge[i+1][j] > 6) || (knowledge[i+1][j] < -1))
					knowledge[i+1][j] = 1;
				if ((knowledge[i-1][j] > 6) || (knowledge[i-1][j] < -1))
					knowledge[i-1][j] = 1;
			}

			if (shots == 0) {
				if ((knowledge[i][j] == 32) || (knowledge[i][j] == 4))
					knowledge[i][j] = 0;
			}

			if ((knowledge[i][j] == 16) || (knowledge[i][j] == 8) || (knowledge[i][j] == 20))
				pits++;

			if (knowledge[i][j] >= 32) {		//for wumpus inference
				wumpii++;
				wx = j;
				wy = i;
			}
			if (knowledge[i][j] == 4)
				stenches++;
		}
	}

	if ((pits > 2)	|| (wumpii > 2))			//if the agent believes there are more than 2 pits or wumpii, it will engage in riskier behaviour
		misbehave = 1;

	if (stenches > 1) {						//for determining wumpus location
		int count = 0;
		for (int i=0; i < 4; i++) {
			for (int j=0; j < 4; j++) {
				if (knowledge[i][j] == 32) {
					if (knowledge[i][j+1] == 4)
						count++;
					if (knowledge[i][j-1] == 4)
						count++;
					if (knowledge[i+1][j] == 4)
						count++;
					if (knowledge[i-1][j] == 4)
						count++;
				}

				if (count == stenches) {
					wumpii = 1;
					wx = j;
					wy = i;
				}
			}
		}
	}

	if ((wumpii == 1) && (shots > 0)) {	//attempts to kill wumpus when sure of location
		for (int i=0; i < 4; i++) {
			if (knowledge[i][x] == 32) {
				if (i > y)
					shoot = 'u';
				else
					shoot = 'd';
			}
		}
		for (int j=0; j < 4; j++) {
			if (knowledge[y][j] == 32) {
				if (j > x)
					shoot = 'r';
				else
					shoot = 'l';
			}
		}
	}

	for (int i=0; i < 4; i++) {			//set movelist greater than highest flag to prevent out-of-bounds moves
		movelist[i] = 35;
	
	//generates legal moves
	if (x < 3)
		movelist[0] = knowledge[y][x+1];
	if (y < 3)
		movelist[1] = knowledge[y+1][x];
	if (x > 0)
		movelist[2] = knowledge[y][x-1];
	if (y > 0)
		movelist[3] = knowledge[y-1][x];
	}
}

//calculates and returns next move for agent
//[agt_mov]
char agent::calcmove()
{
	int min = 0;
	int place = 0;
	char dir = 0;
	int count = 0;
	int zeroes = 0;

	for (int i=0; i < 4; i++) {
		if (movelist[i] < min) {			//flags that are definitely gold
			min = movelist[i];
			place = i;
			break;
		}

		if ((movelist[i] == 24) || (movelist[i] == 8)  || (movelist[i] == 5) || (movelist[i] == 3)) { //flags that imply gold
			min = -1;
			place = i;
		}
	}

	if (misbehave) { 			//agent lives dangerously
		do {
			place = rand() % 4;
		} while (movelist[place] > 20);
	}
	else if (min >= 0) {		//agent acts conservatively
		do {
			place = rand() % 4;
		} while (movelist[place] > 6);
	}
	//else agent goes for gold
	
	//converts chosen move array index to char for piping to board class
	switch (place) {			//0 - r; 1 - u; 2 - l; 3 - d
		case 0:	if (x < 3) {
					dir = 'r';
					x++;
					break;
				}
		case 1:	if (y < 3) {
					dir = 'u';
					y++;
					break;
				}
		case 2:	if (x > 0) {
					dir = 'l';
					x--;
					break;
				}
		case 3:	if (y > 0) {
					dir = 'd';
					y--;
					break;
				}
	}

	return dir;
}

//prints what the agent sees
//[agt_pri]
void agent::print(board field)
{
	int code;
	for (int i=3; i >= 0; i--) {
		for (int j=0; j < 4; j++) {
			code = knowledge[i][j];

			if ((i == y) && (j == x)) {
				if (code == 2)
					pbp << "b";
				else if (code == 4)
					pbp << "s";
				else
					pbp << "x";
			}
			else
				pbp << code;

			if ((j == 3) && (i > 0))
				pbp << "\n-+-+-+-\n";

			if (j < 3)
				pbp << "|";
		}
	}
	pbp << "\n\n";
}

//function to automate the agent
//[agt_go]
int agent::play(board field)
{
	int data = 0;
	int code = 0;

	while (1) {
		data = field.feedback();
		assess(field);
		print(field);
//		cin.ignore();
//		cin.get();
		if (shoot != 0) {
			field.shoot(shoot);
			shoot = 0;
			shots--;
		}
		else
			field.move(calcmove());

		if ((data == 16) || (data == 32)) {
			data = 256;
			break;
		}
		if (data == -8) {
			data = -256;
			break;
		}
	} 

	return data;
}

///////////////////////////////////////////////////////////////////////////////////
//Main
int main()
{
	board field;
	agent ai;
	int xdir, ydir, data;
	xdir = ydir = 0;

	srand(time(NULL));

	field.usersetb();

	if (ai.play(field) == -256)
		cout << "\nThe agent won\n";
	else
		cout << "\nThe agent lost\n";
	cout << "For details, refer to playbyplay.txt\n";

	return 0;
}
