#pragma once
#include <string>

class Node
{
public:
	int var1;
	int var2;

	Node* next;

	Node();
	Node(int val1, int val2);
	~Node();

	void dodaj(int val1, int val2);
};

//Macierz sasiedztwa
class MN
{
public:


	int* mn;
	int rozmiar;

	void inicjalizuj(std::string sciezka);
	void wypisz();

	MN();
	~MN();
};

class LN
{
public:
	Node** ln;
	int rozmiar;

	LN(MN& mn);			// konstruktor konwertujacy
	~LN();

	void wypisz();
};