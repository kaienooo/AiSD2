//drzewo bst

#include <iostream>

#define ENDL std::cout<<std::endl;

class Node;

Node* nastepnik(Node*);
Node* poprzednik(Node*);

class Tree			// Tree BST
{
public:
	Node* root;

	void dodaj(int val);
	void dodaj(int* values, int size);

	void preOrder();
	void inOrder();
	Node* findValue(int x);
	Node* findMax();
	Node* findMin();
	//lab2
	void usunLiscie();

	Tree();
};

Tree::Tree() : root(0) {};



class Node
{
public:
	int value;
	Node* up;
	Node* left;
	Node* right;

	Node(int val, Node* parent);
	~Node();

	void dodaj(int val, Node* parent);
	void preOrder();
	void inOrder();
	Node* findValue(int x);
	Node* findMax();
	Node* findMin();
	void usunLiscie();
	void usunLiscie(float srednia);
	void usun();
	void makeNull();
};

Node::Node(int val, Node* parent) : value(val), up(parent), left(0), right(0)
{}

Node::~Node()
{
	if (!up)
	{
		return;
	}
	if (up->right == this)
	{
		up->right = 0;
	}
	else if (up->left == this)
	{
		up->left = 0;
	}
}

//Tree

void Tree::dodaj(int val)
{
	if (!root)
	{
		root = new Node(val, 0);
	}
	else
	{
		root->dodaj(val, root);
	}
}
void Tree::dodaj(int* values, int size)
{
	int i = 0;
	if (!root)
	{
		root = new Node(values[i++], 0);
	}

	while (i < size)
	{
		root->dodaj(values[i++], root);
	}
}

void Tree::preOrder()
{
	if (root)
	{
		root->preOrder();
	}
	else
	{
		std::cout << "Nie ma pierwszego elementu!" << std::endl;
	}
}
void Tree::inOrder()
{
	if (root)
	{
		root->inOrder();
	}
	else
	{
		std::cout << "Nie ma pierwszego elementu!" << std::endl;
	}
}
Node* Tree::findValue(int val)
{
	if (root)
	{
		return root->findValue(val);
	}
	else
	{
		std::cout << "Drzewo nie istnieje! Zwrocono 0 (nullptr)" << std::endl;
		return 0;
	}
}

Node* Tree::findMax()
{
	return root->findMax();
}

Node* Tree::findMin()
{
	return root->findMin();
}

void Tree::usunLiscie()
{
	if (root)
	{
		root->usunLiscie();
	}
	else
	{
		std::cout << "Nie ma drzewa" << std::endl;
	}
}

// Node

void Node::dodaj(int val, Node* parent)
{
	if (val >= value)
	{
		if (right)
		{
			right->dodaj(val, this);
		}
		else
		{
			right = new Node(val, this);
		}
	}
	else
	{
		if (left)
		{
			left->dodaj(val, this);
		}
		else
		{
			left = new Node(val, this);
		}
	}
}

void Node::preOrder()
{
	std::cout << value << std::endl;
	if (left)
	{
		left->preOrder();
	}
	if (right)
	{
		right->preOrder();
	}
}

void Node::inOrder()
{
	if (left)
	{
		left->inOrder();
	}
	std::cout << value << std::endl;
	if (right)
	{
		right->inOrder();
	}
}

Node* Node::findValue(int x)
{

	if (value == x)
	{
		return this;
	}
	else if (x > value)
	{
		if (right)
			return right->findValue(x);
	}
	else
	{
		if (left)
			return left->findValue(x);
	}

	return 0;
}

Node* Node::findMax()
{
	if (right)
		return right->findMax();

	return this;
}

Node* Node::findMin()
{
	if (left)
		return left->findMin();

	return this;
}

void Node::usunLiscie()
{
	if (!(left || right))
	{
		delete this;
		return;
	}
	if (left)
		left->usunLiscie();
	if (right)
		right->usunLiscie();
}

void Node::usunLiscie(float srednia)
{
	if (!(left || right) && (float)value > srednia)
	{
		delete this;
		return;
	}
	if (left)
		left->usunLiscie(srednia);
	if (right)
		right->usunLiscie(srednia);
}

void Node::usun()
{
	if (!(left || right))
	{
		delete this;
		return;
	}
	else if (!(left && right) && (left || right))
	{
		if (up->right == this)
		{
			if (right)
			{
				up->right = right;
				right->up = up;
			}
			else if (left)
			{
				up->right = left;
				left->up = up;
			}
		}
		else if (up->left == this)
		{
			if (right)
			{
				up->left = right;
				right->up = up;
			}
			else if (left)
			{
				up->left = left;
				left->up = up;
			}
		}
		delete this;
		return;
	}
	else if (left && right)
	{
		Node* y = nastepnik(this);
		value = y->value;
		y->usun();
		return;
	}
}

void Node::makeNull()
{
	if (left)
	{
		left->makeNull();
	}
	if (right)
	{
		right->makeNull();
	}
	delete this;
}

// z wykladu \/			- zainspirowane wykladem, ale zrozumiane

Node* nastepnik(Node* x)
{
	if (!x)
		return 0;

	if (x->right)
		return x->right->findMin();


	Node* y = x->up;

	while (y && y->right == x)
	{
		x = y;
		y = y->up;
	}

	return y;
}

Node* poprzednik(Node* x)
{
	if (!x)
		return 0;
	if (x->left)
		return x->left->findMin();

	Node* y = x->up;

	while (y && y->left == x)
	{
		x = y;
		y = y->up;
	}

	return y;
}

// z wykladu /\

void rotacjaL(Node* q, Tree& tree)
{
	Node* p = q->right;
	q->right = p->left;
	p->left = q;
	if (q->right)
	{
		q->right->up = q;
	}
	p->up = q->up;
	if (p->up)
	{
		if (p->up->left == q)
		{
			p->up->left = p;
		}
		else if (p->up->right == q)
		{
			p->up->right = p;
		}
	}
	else
	{
		tree.root = p;
	}
	q->up = p;
}

void rotacjaR(Node* q, Tree& tree)
{
	Node* p = q->left;
	q->left = p->right;
	p->right = q;
	if (q->left)
	{
		q->left->up = q;
	}
	p->up = q->up;
	if (p->up)
	{
		if (p->up->left == q)
		{
			p->up->left = p;
		}
		else if (p->up->right == q)
		{
			p->up->right = p;
		}
	}
	else
	{
		tree.root = p;
	}
	q->up = p;
}

// niepotrzebne tutaj, ale ok

void rotacjaRL_wlewo(Node* q, Tree& tree)
{
	rotacjaR(q->right, tree);
	rotacjaL(q, tree);
}

void rotacjaRL_wprawo(Node* q, Tree& tree)
{
	rotacjaR(q, tree);
	rotacjaL(q, tree);
}

void rotacjaLR_wprawo(Node* q, Tree& tree)
{
	rotacjaL(q->left, tree);
	rotacjaR(q, tree);
}

void rotacjaLR_wlewo(Node* q, Tree& tree)
{
	rotacjaL(q, tree);
	rotacjaR(q, tree);
}




// Wyszukaj element o wartoœci x.
// Je¿eli ten element istnieje i ma dwóch
// synów oblicz œredni¹ SR wartoœci
// minimalnej oraz maksymalnej wartoœci na
// tym poddrzewie a nastêpnie usuñ
// wszystkie liœcie na tym poddrzewie wiêksze
// od SR.
void zadInne1(int x, Tree& tree)
{
	Node* foundNode = tree.findValue(x);

	if (foundNode && (foundNode->left && foundNode->right))
	{
		float srednia = ((float)foundNode->findMin()->value + (float)foundNode->findMax()->value) / 2;
		foundNode->usunLiscie(srednia);
		return;
	}

	std::cout << "Nie znaleziono takiego wezla o zadanej wartosci " << x << " ktory mialby 2 synow!\n";
	return;
}

//Oblicz œredni¹ wartoœæ z wartoœci najmniejszej,
//najwiêkszej na drzewie oraz wartoœci root.
//Nastêpnie je¿eli wartoœæ œrednia jest wiêksza
//do root usuñ ca³e prawe poddrzewo root->R,
//natomiast je¿eli nie jest wiêksza to usuñ liœcie z
//lewego poddrzewa root->L.
void zadInne2(Tree& tree)
{
	float srednia = ((float)tree.findMin()->value + (float)tree.root->value + (float)tree.findMax()->value) / 3.0;

	if (srednia > (float)tree.root->value)
	{
		tree.root->right->makeNull();
	}
	else
	{
		tree.root->left->usunLiscie();
	}

}

int main()
{
	Tree tree;

	// rozne polecenia na drzewie sprawdzajace dzialanie funkcji

	/*
	tree.dodaj(30);
	tree.dodaj(5);
	tree.dodaj(6);
	tree.dodaj(7);
	tree.dodaj(8);
	tree.dodaj(9);

	tree.inOrder();
	std::cout << "Test poprzednik(x).value oraz nastepnik(x).value, x = tree.findvalue(9)\npoprzednik(x).value expected 8 = " << poprzednik(tree.findValue(9))->value << std::endl
		<< "nastepnik(x).value expected 30 = " << nastepnik(tree.findValue(9))->value << std::endl;

	tree.usunLiscie();

	tree.inOrder();

	*/



	{
		int temp[] = { 22,18,57,33,24,44,82 };
		tree.dodaj(temp, sizeof(temp) / sizeof(int));
	}

	std::cout << "Max: " << tree.findMax()->value << std::endl;
	std::cout << "Min: " << tree.findMin()->value << std::endl;

	std::cout << "\t" << tree.root->value << std::endl
		<< tree.root->left->value << "\t\t\t" << tree.root->right->value << std::endl
		<< "\t\t" << tree.root->right->left->value << "\t\t" << tree.root->right->right->value << std::endl
		<< "\t" << tree.root->right->left->left->value << "\t" << tree.root->right->left->right->value << std::endl;

	rotacjaRL_wlewo(tree.root, tree);
	ENDL
		std::cout << "\t\t" << tree.root->value << std::endl
		<< "\t" << tree.root->left->value << "\t\t" << tree.root->right->value << std::endl
		<< tree.root->left->left->value << "\t\t" << tree.root->left->right->value << " " << tree.root->right->left->value << "\t\t" << tree.root->right->right->value << std::endl;
	rotacjaRL_wprawo(tree.root, tree);
	ENDL
		std::cout << "\t" << tree.root->value << std::endl
		<< tree.root->left->value << "\t\t\t" << tree.root->right->value << std::endl
		<< "\t\t" << tree.root->right->left->value << "\t\t" << tree.root->right->right->value << std::endl
		<< "\t" << tree.root->right->left->left->value << "\t" << tree.root->right->left->right->value << std::endl;


	/*
	{
		int temp[] = { 15,6,-5,-8,14,8,12,13,22,18,17,27,37 };
		tree.dodaj(temp, sizeof(temp) / sizeof(int));
	}

	tree.inOrder();

	zadInne1(6, tree);
	*/

	/*
	tree.findMax()->value = 100;
	zadInne2(tree);
	*/

	ENDL
		tree.inOrder();
}
