#include <fstream>
#include <iostream>
#include <format>

#include "graf.h"


Node::Node() : next(0), var1(0), var2(0)
{}

Node::Node(int val1, int val2) : next(0), var1(val1), var2(val2)
{}

Node::~Node()
{}

void Node::dodaj(int val1, int val2)
{
	if (next)
	{
		next->dodaj(val1, val2);
	}
	else
	{
		next = new Node(val1, val2);
	}
}

void MN::inicjalizuj(std::string sciezka)
{
	std::fstream file(sciezka,std::fstream::in);

	file >> rozmiar;

	mn = new int[rozmiar * rozmiar];

	for (int i = 0; i < rozmiar * rozmiar; i++)
	{
		file >> mn[i];
	}

	file.close();
}

void MN::wypisz()
{
	for (int y = 0; y < rozmiar; y++)
	{
		for (int x = 0; x < rozmiar; x++)
		{
			std::cout << mn[y * rozmiar + x] << "\t";
		}
		std::cout << std::endl;
	}
}

MN::MN() : mn(0), rozmiar(0)
{}

MN::~MN()
{
	delete[] mn;
}

LN::LN(MN& mn)
{
	rozmiar = mn.rozmiar;

	ln = new Node * [rozmiar];
	
	for (int i = 0; i < rozmiar; i++)
	{
		ln[i] = 0;
	}

	for (int y = 0; y < mn.rozmiar; y++)
	{
		for (int x = 0; x < mn.rozmiar; x++)
		{
			if (mn.mn[y * mn.rozmiar + x] != 0)
			{
				if (ln[y] == 0)
				{
					ln[y] = new Node(x, mn.mn[y * mn.rozmiar + x]);
				}
				else
				{
					ln[y]->dodaj(x, mn.mn[y * mn.rozmiar + x]);
				}
			}
		}
	}
}

LN::~LN()
{
	for (int i = 0; i < rozmiar; i++)
	{
		delete ln[i];
	}
	delete[] ln;
}

void LN::wypisz()
{
	for (int i = 0; i < rozmiar; i++)
	{
		Node* temp = ln[i];
		std::cout << "Z:\t" << i << " do:\t";
		while (temp)
		{
			std::cout << temp->var1 << " " << temp->var2 << "\t| ";
			temp = temp->next;
		}
		std::cout << std::endl;
	}
}

int main()
{
	MN mn;

	mn.inicjalizuj("dane_graf");
	mn.wypisz();

	std::cout << std::endl;

	LN ln(mn);

	ln.wypisz();
}


