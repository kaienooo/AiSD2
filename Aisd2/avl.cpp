//drzewo AVL

#include <iostream>

#define ENDL std::cout<<std::endl;


class Node;

Node* nastepnik(Node*);
Node* poprzednik(Node*);

class Tree			// Tree AVL
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
	void usun(int val);

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

	int balance_factor;

	Node(int val, Node* parent);
	~Node();

	void dodaj(int val, Tree& tree);
	void preOrder();
	void inOrder();
	Node* findValue(int x);
	Node* findMax();
	Node* findMin();
	// lab 
	void usunLiscie();
	void usunLiscie(float srednia);
	void usun();
	int usun(int val, Tree& tree);
	void makeNull();
};

Node::Node(int val, Node* parent) : value(val), up(parent), left(0), right(0), balance_factor(0)
{}

Node::~Node()	// poprawic zeby przy usuwaniu naprawial balans [DONE] - w funkcji
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
		root->dodaj(val, *this);
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
		root->dodaj(values[i++], *this);
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

void Tree::usun(int val)
{
	if (root)
	{
		root->usun(val, *this);
	}
	else
	{
		std::cout << "Nie ma drzewa" << std::endl;
	}
}

// Node

void rotacjaL(Node*, Tree&);
void rotacjaR(Node*, Tree&);
void rotacjaRL_wlewo(Node*, Tree&);
void rotacjaLR_wprawo(Node*, Tree&);

void Node::dodaj(int val, Tree &tree)
{
	if (val >= value)
	{
		if (right)
		{
			right->dodaj(val, tree);
			
			balance_factor += fabs(right->balance_factor);

			if (balance_factor > 1)
			{
				balance_factor = 0;
				if (right->balance_factor > 0)
				{
					right->balance_factor = 0;
					rotacjaL(this, tree);
				}
				else
				{
					right->balance_factor = 0;
					rotacjaRL_wlewo(this, tree);
				}
			}
		}
		else
		{
			right = new Node(val, this);
			balance_factor++;
		}
	}
	else
	{
		if (left)
		{
			left->dodaj(val, tree);

			balance_factor -= fabs(left->balance_factor);

			if (balance_factor < -1)
			{
				balance_factor = 0;
				if (left->balance_factor < 0)
				{
					left->balance_factor = 0;
					rotacjaR(this, tree);
				}
				else
				{
					left->balance_factor = 0;
					rotacjaLR_wprawo(this, tree);
				}
			}
		}
		else
		{
			left = new Node(val, this);
			balance_factor--;
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
	std::cout << value << " " << balance_factor << std::endl;
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

// ta funkcja zla do usuwania avl, nie ma wywolania rekurencyjnego <->
// latwego dostepu do ojca oraz informacji ktorym synem (lewym, prawym) jest usuwany wezel lub wezel w ktorym zmienia sie balance_factor
// uzyta do usuwanie wezla gdy juz zadbano o balans
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

// usuwania nie jestem pewny czy w kazdym przypadku zadziala prawidlowo
int Node::usun(int val, Tree& tree)
{
	if (val == value)
	{
		if (left && right)
		{
			Node* y = nastepnik(this);
			value = y->value;
			int wynik = right->usun(y->value, tree);

			if (wynik == -2)	// tutaj niestety potrzebne jest skopiowanie (lub stworzenie funkcji) tego co sie dzieje gdy wraca z prawej, poniewaz musze zasymulowac przypadek ze usuwano lisc
			{
				balance_factor--;
				if (balance_factor < -1)	// ponizsze z rysunku
				{
					if (left->balance_factor == 1)
					{
						balance_factor = 0;
						left->balance_factor = 0;
						rotacjaLR_wprawo(this, tree);
					}
					else
					{
						if (left->balance_factor == 0)
						{
							balance_factor = -1;
							left->balance_factor = 1;
						}
						else if (left->balance_factor == -1)
						{
							balance_factor = 0;
							left->balance_factor = 0;
						}
						rotacjaR(this, tree);
					}
					return false;	// czy po rotacji zawsze ok?
				}
				wynik /= 2;
				return wynik;
			}

			// -1, 1, 3	-> usunieto cokolwiek we wczesniejszych wywolaniach
			if (!wynik)
			{
				if (right->balance_factor == 0)	// pierwszy napotkany wezel gdzie balance_factor wezla po lewej po usuwaniu rowna sie 0
					// w tej sytuacji zawsze sie cos usunelo, wiec musiala nastapic zmiana dlugosci od ktoregos z jego dzieci ze teraz sie rowna 0
					// niemozliwa sytuacja po rotacji bo wtedy wynik == 0
					// czy to zawsze ojciec usuwanego wezla? - nie
				{
					balance_factor--;
					return wynik;
				}
			}

			// 0 - nie usunieto lub wykonano rotacje -> wszystko powyzej jest juz ok
			// Bedzie wracac caly czas zwracajac false -> nic sie nie stanie
			return false;

			//koniec kopii

		}
		else if (!(left && right) && (left || right))
		{
			usun();
		}
		else if (!(left || right))
		{
			delete this;
		}

		return -2;			// wartosc -2 zostala po poprzedniej implementacji, gdy jeszcze kazdy przypadek byl rozpatrzany osobno. Teraz juz wiem, ze kazdy da sie sprowadzic do przypadku ze usuwano lisc.
	}

	if (val < value)
	{
		if (!left)
		{
			goto end;
		}
		int wynik = left->usun(val, tree);

		// -2, 2, 6 -> usunieto w powyzszym wywolaniu

		if (wynik == -2) // -2, usunieto wezel w powyzszym wywolaniu
		{
			balance_factor++;
			if (balance_factor > 1)	// ponizsze operacje z rysunku
			{
				if (right->balance_factor == -1)
				{
					balance_factor = 0;
					right->balance_factor = 0;
					rotacjaRL_wlewo(this, tree);
				}
				else
				{
					if (right->balance_factor == 0)
					{
						balance_factor = 1;
						right->balance_factor = -1;
					}
					else if (right->balance_factor == 1)
					{
						balance_factor = 0;
						right->balance_factor = 0;
					}

					rotacjaL(this, tree);
				}
				return false;
			}
			wynik /= 2;
			return wynik;
		}


		// -1, 1, 3	-> usunieto cokolwiek we wczesniejszych wywolaniach
		if (!wynik)
		{
			if (left->balance_factor == 0)	// pierwszy napotkany wezel gdzie balance_factor wezla po lewej po usuwaniu rowna sie 0
											// w tej sytuacji zawsze sie cos usunelo, wiec musiala nastapic zmiana dlugosci od ktoregos z jego dzieci ze teraz sie rowna 0
											// niemozliwa sytuacja po rotacji bo wtedy wynik == 0
											// czy to zawsze ojciec usuwanego wezla? - nie, w takim razie kiedy zakonczyc? (po sprawdzeniu, nigdy)
			{
				balance_factor++;
				return wynik;
			}
		}

		// 0 - nie usunieto lub wykonano rotacje -> wszystko powyzej jest juz ok
		// Bedzie wracac caly czas zwracajac false -> nic sie nie stanie
		return false;
	}

	if (val > value)
	{
		if (!right)
		{
			goto end;
		}
		int wynik = right->usun(val, tree);

		if (wynik == -2)	// usunieto wezel w powyzszym wywolaniu
		{
			balance_factor--;
			if (balance_factor < -1)	// ponizsze z rysunku
			{
				if (left->balance_factor == 1)
				{
					balance_factor = 0;
					left->balance_factor = 0;
					rotacjaLR_wprawo(this, tree);
				}
				else
				{
					if (left->balance_factor == 0)
					{
						balance_factor = -1;
						left->balance_factor = 1;
					}
					else if (left->balance_factor == -1)
					{
						balance_factor = 0;
						left->balance_factor = 0;
					}
					rotacjaR(this, tree);
				}
				return false;	// czy po rotacji zawsze ok?
			}
			wynik /= 2;
			return wynik;
		}

		// -1, 1, 3	-> usunieto cokolwiek we wczesniejszych wywolaniach
		if (!wynik)
		{
			if (right->balance_factor == 0)	// pierwszy napotkany wezel gdzie balance_factor wezla po lewej po usuwaniu rowna sie 0
				// w tej sytuacji zawsze sie cos usunelo, wiec musiala nastapic zmiana dlugosci od ktoregos z jego dzieci ze teraz sie rowna 0
				// niemozliwa sytuacja po rotacji bo wtedy wynik == 0
				// czy to zawsze ojciec usuwanego wezla? - nie
			{
				balance_factor--;
				return wynik;
			}
		}

		// 0 - nie usunieto lub wykonano rotacje -> wszystko powyzej jest juz ok
		// Bedzie wracac caly czas zwracajac false -> nic sie nie stanie
		return false;
	}
	
	end:

	std::cout << "Nie ma wezla o takiej wartosci" << std::endl;
	return false;
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
	// wszystkie co dzialaly na BST powinny dzialac rowniez na drzewie AVL

	/*
	{
		int temp[] = {0,5,10,15,8,-2,-4};
		tree.dodaj(temp, sizeof(temp) / sizeof(int));
	}

	tree.inOrder();
	// OK
	*/

	tree.dodaj(0);
	tree.dodaj(5);

	tree.inOrder();	//ok
	ENDL;

	tree.dodaj(10);

	tree.inOrder();	//ok
	ENDL;

	tree.dodaj(15);
	tree.dodaj(8);

	tree.inOrder();	//ok
	ENDL;

	tree.dodaj(-2);

	tree.inOrder();	//ok
	ENDL;

	tree.dodaj(-4);

	tree.inOrder();	//ok
	ENDL;
	tree.preOrder();
	ENDL;


	tree.usun(0);	// usuwanie liscia ok

	tree.inOrder(); //ok
	ENDL;
	tree.preOrder();
	ENDL;

	tree.usun(-2);	// usuwanie wezla ktory ma jednego syna
	tree.inOrder(); //ok
	ENDL;
	tree.preOrder();
	ENDL;

	tree.usun(10);	// usuwanie wezla ktory ma dwoch synow
	tree.inOrder(); //wyglada ok
	ENDL;
	tree.preOrder();
	ENDL;

	tree.usun(-4); // usuwanie liscia, ale wymaga rotacji
	tree.inOrder(); //wyglada ok
	ENDL;
	tree.preOrder();
	ENDL;
}
