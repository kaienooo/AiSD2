//kododwanie huffmana
#include <iostream>

class Node;

class Lista
{
public:
	Node* root;
	
	Lista();

	void inicjalizuj(int* tablicaWystapien,char* tablicaZnakow, int tablen);
	void find2min(Node*& st, Node*& nd);
	void koduj();
};

class Tree
{
public:
	Node* root;

	Tree();
};

class Node
{
public:
	Node* left;
	Node* right;
	Node* vertical;

	int value;

	Node() : value(0), left(0), right(0), vertical(0) {};
	Node(int value, Node* parent, bool isTreeNode);
	void dodaj(int val);
	void usun(Node* &root);	// wezel listy

	void inOrder();
	void preOrder();

};


//Lista

Lista::Lista() : root(0)
{}

void Lista::inicjalizuj(int* tablicaWystapien, char* tablicaZnakow, int tablen)
{

	root = new Node(tablicaWystapien[0], 0, 0);
	root->vertical = new Node(tablicaZnakow[0], root, 1);

	Node* temp = root;

	for (int i = 1; i < tablen; i++)
	{
		temp->right = new Node(tablicaWystapien[i], temp, 0);
		temp->right->vertical = new Node(tablicaZnakow[i], temp->right, 1);
		temp = temp->right;
	}
}

void Lista::find2min(Node*& st, Node*& nd)
{
	st = root;
	nd = root;

	Node* temp = root->right;

	while (temp)
	{
		if (temp->value < st->value)
		{
			st = temp;
		}
		temp = temp->right;
	}

	temp = root->right;

	while (temp)
	{
		if (temp != st)
		{
			if (temp->value < nd->value)
			{
				nd = temp;
			}
		}
		temp = temp->right;
	}
}

void Lista::koduj()
{
	Node* st = root;
	Node* nd = root;

	while (true)
	{
		find2min(st, nd);

		if (st == nd)
		{
			st->usun(root);
			break;
		}
		Node* pod_st = st->vertical;
		st->vertical = new Node(-1, st, 1);				// utworz pusty wezel drzewa pod wezlem listy
		st->vertical->left = pod_st;					// wezel ktory wczesniej byl pod pierwszym teraz jest lewym dzieckiem tego nowego wezla
		st->vertical->left->vertical = st->vertical;	// lewy syn wskazuje na ojca
		st->vertical->right = nd->vertical;
		st->vertical->right->vertical = st->vertical;

		
		std::cout << "\n\n";

		st->vertical->preOrder();

		std::cout << "\n\n";
		

		std::cout << "Najmniejsza: " << st->value << "\t\t2nd Najmniejsza: " << nd->value << "\n\n";

		st->value = st->value + nd->value;

		nd->usun(root);
		nd = st;
	}
}

//Tree

Tree::Tree() : root(0)
{}

//Node

Node::Node(int value, Node* parent, bool isTreeNode) : value(value), right(0)
{
	if (isTreeNode)
	{
		left = 0;
		vertical = parent;
	}
	else
	{
		left = parent;
		vertical = 0;
	}
}


void Node::dodaj(int val)	// Dodawanie wezla drzewa, bo tylko te beda dodawane w przeciwienstwie do wezlow listy, one beda usuwane
{
	if (val < value)
	{
		if (left)
		{
			left->dodaj(val);
		}
		else
		{
			left = new Node(val, this, true);
		}
	}
	else
	{
		if (right)
		{
			right->dodaj(val);
		}
		else
		{
			right = new Node(val, this, true);
		}
	}
}

void Node::usun(Node* &root)
{
	if (!left)	
	{
		if (!right) // ostatni element
		{
			root = vertical;
			delete this;
			return;
		}

		// usuwamy roota, ale jeszcze cos istnieje

		root = right;
		right->left = 0;
		delete this;
		return;
	}

	// Jest napewno lewy

	left->right = right;	// lewy po prawej wskazuje na prawego (niezalnie czy NULL czy Node* )
	if (right)	// sytuacja gdy this jest w srodku
	{
		right->left = left;	//jezeli po prawej, to ten po prawej wskazuje na naszego lewego
	}

	delete this;
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

int main()
{
	const int tablen = 5;
	int tabWyst[tablen] = { 92,18,5,3,3 };
	char tabZnak[tablen] = { 'a','b','c','d','e' };

	Lista lista;

	lista.inicjalizuj(tabWyst, tabZnak, tablen);


	lista.koduj();

	Tree tree;


	std::cout << "\n\n";
	tree.root = lista.root;
	tree.root->preOrder();

	// GENERUJE DRZEWO - funkcja Lista::koduj() generuje drzewo na podstawie tablic

	// BRAKUJE
	// - mapowanie ruchow na drzewie na kody znakow np. lewo, lewo, prawo -> 001
	// - wczytywanie tablic z stringa
	// - rozszyfrowywanie z ciagu zer i jedynek na znaki
	// zaliczone?
}


