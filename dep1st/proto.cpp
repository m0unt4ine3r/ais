//needs param to dictate depth searched
#include<iostream>
using namespace std;


struct node {
	char spot;
	node *kid[2];
};


//stacks to store paths/track interesting nodes
struct stack {
	node *top = 0;
	int count = 0;
	
	void push(node *parent)
	{
		node *cur;
		cur = parent;
		
		if (top == 0)
			top = cur;
		else {
			cur->kid[0] = top;
			top = cur;
		}
		count++;
	}
	
	node *pop()
	{
		node *prev;
	
		if (count == 0) 
			return 0;
		else {
			prev = top;
			top = top->kid[0];
			return prev;
		}
		count--;
	}
	
	void flush()
	{
		top = 0;
		count = 0;
	}
	
	void display()
	{
		if (top != 0) {
			while (top->kid[0]) {
				cout << top->spot;
				top = top->kid[0];
			}
		}
		else
			cout << "Empty";
	}
};

/*
//generate given tree and return pointer to start node
node *createtree(char start)
{
	char list[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	node *map, *cur, *ret, *prev=0;
	int size = sizeof(list)/sizeof(list[0]);
	
	map = new node;
	cur = map;
	for (int i=0; i < size-2; i++) { //tree w/ all nodes but root having a child that is also parent
		if (start == list[i])
			ret = cur;
		cur->spot = list[i];
		cur->kid[0] = new node;
//		cout<<"\ncur: "<<cur<<"\n";
//		cout<<"prev: "<<prev<<"\n";
//		cout<<"kid0: "<<cur->kid[0]<<"\n";
//		cout<<"kid1: "<<cur->kid[1]<<"\n";
		prev = cur;
		cur = cur->kid[0];
		cur->kid[1] = prev;
	}
	cur = 0;
	
	cur = map;
	for (int i=size-2; i < size; i++) { //can be merged with above
		if (start == list[i])
			ret = cur;
		cur->kid[1] = new node;
		cur = cur->kid[1];
		cur->spot = list[i];
	}
	map->kid[1]->kid[0] = map;
	cur->kid[0] = map->kid[1];
	cur->kid[1] = 0;
	
	return ret;
}
*/

//node *a, *b, *c, *d, *e, *f;
//
//a = new node;
//b = new node;
//c = new node;
//d = new node;
//e = new node;
//f = new node;
//
//a->spot = 'a';
//a->kid[0] = b;
//a->kid[1] = e;
//
//b->spot = 'b';
//b->kid[0] = a;
//b->kid[1] = c;
//
//c->spot = 'c';
//c->kid[0] = b;
//c->kid[1] = d;
//
//d->spot = 'd';
//d->kid[0] = c;
//d->kid[1] = 0;
//
//e->spot = 'e';
//e->kid[0] = a;
//e->kid[1] = f;
//
//f->spot = 'f';
//f->kid[0] = e;
//f->kid[1] = 0;


//searches through tree, placing nodes with 2(+) children, neither being a parent, in stack
//and returns a stack of the path to goal
stack *checktree(node *root, char goal)
{
	int flag[] = {0, 0, 0}; //[0]fin found, [1]-kid index, [2]-searched
	node *cur, *prev;
	stack unexplored, *path;
	
	cur = root;
	prev = new node;
	prev->spot = 0;
	
	//needs to iterate thru tree, check if previous == left/right, if yes go opposite
	//else default left, check if next exists, goto next iff next != previous & next exists
	while ((flag[0] == 0) && (flag[2] == 0)) {
		if (cur->spot == goal) {
			flag[0] = 1;
			cout<<"found\n";
		}
		else {
			path.push(cur);
			if (cur->kid[0] == prev) {
				flag[1] = 1;
				cout<<"go right\n";
			}
			else if (cur->kid[1] == prev) {
				flag[1] = 0;
				cout<<"go left\n";
			}
			else {
				unexplored.push(cur);
				flag[1] = 0;
				cout<<"go left\n";
			}

			if ((cur->kid[flag[1]]) && (cur->kid[flag[1]] != prev)) {
				prev = cur;
				cur = cur->kid[flag[1]];
			}
			else {
				path.flush();
				cur = unexplored.pop();
				if (cur)
					cur = cur->kid[1];
				else
					flag[2] = 1;
			}
		}
	}
	
	return path;
}


int main()
{
	char start, goal;
	node *beg;
	stack *end;

	node *a, *b, *c, *d, *e, *f;

	a = new node;
	b = new node;
	c = new node;
	d = new node;
	e = new node;
	f = new node;
	
	a->spot = 'a';
	a->kid[0] = b;
	a->kid[1] = e;
	
	b->spot = 'b';
	b->kid[0] = a;
	b->kid[1] = c;
	
	c->spot = 'c';
	c->kid[0] = b;
	c->kid[1] = d;
	
	d->spot = 'd';
	d->kid[0] = c;
	d->kid[1] = 0;
	
	e->spot = 'e';
	e->kid[0] = a;
	e->kid[1] = f;
	
	f->spot = 'f';
	f->kid[0] = e;
	f->kid[1] = 0;

	cout << "Enter start position: ";
	cin >> start;
	
//	beg = createtree(start);

	cout << "Enter goal postition: ";
	cin >> goal;

	end = checktree(a, goal);
	
	end->display();

	return 0;
}













