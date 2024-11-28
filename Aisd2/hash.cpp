#include <iostream>


bool is_string_equal(int dl_imienia, char* imie1, char* imie2)		// te same dlugosci
{
	int i = 0;
	while (true)
	{

		if (imie1[i] != imie2[i])
			return false;
		
		if (++i == dl_imienia)			// koniec tab char
			return true;
	}
}

void to_lower(char* imie, int dl_imienia)
{
	for (int i = 0; i < dl_imienia; i++)
	{
		imie[i] = imie[i] < ('Z' + 1) and imie[i] != ' ' ? imie[i] + 32 : imie[i];
	}
}


class Node
{
public:
	Node* right;
	char* imie;
	int dl_imienia;

	Node(char* imie_tab, int dl_imienia);
	~Node();

	void dodaj(char* imie_tab, int dl_imienia);
	void wyswietl();
};


class Baza
{
public:
	Node** tablica;

	int rozmiar_bazy;

	Baza(int rozmiar_bazy);
	~Baza();
	int hashuj(char* imie_tab, int dl_imienia);
	void dodaj(char* imie_tab, int dl_imienia);
	void usun(char* imie_tab, int dl_imienia);
	void szukaj(char* imie_tab, int dl_imienia);
	void wyswietl();
};

// Node

Node::Node(char* imie_tab, int dl_imienia) : right(0), dl_imienia(dl_imienia)
{
	imie = new char[dl_imienia];

	for (int i = 0; i < dl_imienia; i++)
	{
		imie[i] = imie_tab[i];
	}
}

Node::~Node()
{
	delete[] imie;
}

void Node::dodaj(char* imie_tab, int dl_imienia)
{
	if (this->dl_imienia == dl_imienia)
	{
		if (is_string_equal(dl_imienia, imie, imie_tab))
		{
			std::cout << "Osoba o takiej nazwie juz istnieje w bazie\n";
			return;
		}
	}

	if (right)
	{
		right->dodaj(imie_tab, dl_imienia);
		return;
	}

	right = new Node(imie_tab, dl_imienia);
}

void Node::wyswietl()
{
	for (int i = 0; i < dl_imienia; i++)
	{
		std::cout << imie[i];
	}
	std::cout << std::endl;
	if (right)
		right->wyswietl();
}


// Baza

Baza::Baza(int rozmiar_bazy) : rozmiar_bazy(rozmiar_bazy)
{
	tablica = new Node*[rozmiar_bazy];

	for (int i = 0; i < rozmiar_bazy; i++)
	{
		tablica[i] = nullptr;
	}
}

Baza::~Baza()
{
	for (int i = 0; i < rozmiar_bazy; i++)
	{
		delete[] tablica[i];
	}

	delete[] tablica;
}

int Baza::hashuj(char* imie_tab, int dl_imienia)
{
	int suma = 0;


	for (int i = 0; i < dl_imienia; i++)
	{
		suma += (int)imie_tab[i];
	}

	return suma%rozmiar_bazy;
}

void Baza::dodaj(char* imie_tab, int dl_imienia)
{
	int hash = hashuj(imie_tab, dl_imienia);

	if (tablica[hash])
	{
		tablica[hash]->dodaj(imie_tab, dl_imienia);
		return;
	}

	tablica[hash] = new Node(imie_tab, dl_imienia);
	std::cout << "Dodano osobe o nazwie: " << imie_tab << std::endl;
}

void Baza::usun(char* imie_tab, int dl_imienia)
{
	int hash = hashuj(imie_tab, dl_imienia);

	Node* temp = tablica[hash];

	if (!temp)	// pusta lista
	{
		std::cout << "Nie ma takiej osoby w bazie!\n";
		return;
	}

	if (temp->dl_imienia == dl_imienia)
	{
		if (is_string_equal(dl_imienia, temp->imie, imie_tab))
		{
			tablica[hash] = temp->right;
			delete temp;
			std::cout << "Usunieto pomyslnie!" << std::endl;
			return;
		}
	}

	while (temp->right)
	{
		if (temp->right->dl_imienia == dl_imienia)
		{
			if (is_string_equal(dl_imienia, temp->right->imie, imie_tab))
			{
				Node* prawy = temp->right;

				temp->right = prawy->right;
				delete prawy;
				std::cout << "Usunieto pomyslnie!" << std::endl;
				return;
			}
		}

		temp = temp->right;
	}
	std::cout << "Nie ma takiej osoby w bazie!\n";
}

void Baza::szukaj(char* imie_tab, int dl_imienia)
{
	int hash = hashuj(imie_tab, dl_imienia);

	Node* temp = tablica[hash];

	if (temp->dl_imienia == dl_imienia)
	{
		if (is_string_equal(dl_imienia, temp->imie, imie_tab))
		{
			std::cout << "Osoba o takiej nazwie istnieje!\n";
			return;
		}
	}

	while (temp->right)
	{
		if (temp->right->dl_imienia == dl_imienia)
		{
			if (is_string_equal(dl_imienia, temp->right->imie, imie_tab))
			{
				std::cout << "Osoba o takiej nazwie istnieje!\n";
				return;
			}
		}

		temp = temp->right;
	}
	std::cout << "Nie ma takiej osoby w bazie!\n";
}

void Baza::wyswietl()
{
	for (int i = 0; i < rozmiar_bazy; i++)
	{
		if (tablica[i])
			tablica[i]->wyswietl();
	}
}

int main()
{
	Baza baza(13);

	char napis[100] = { 0 };	//100 - dlugosc buforu

	while (true)
	{
	dodawanie:
		std::cout << "Podaj napis (jezeli zakonczono dodawanie napisz 0):\n";
		std::cin.getline(napis, 100);
		if (napis[0] == '0')
			goto end;
		int i = 0;
		while (napis[i])		// dopoki nie ma znaku EOL - 0 (zgodnie z konwencja kazdy string sie konczy na 0)
		{
			//std::cout << "Napis[" << i << "] = " << napis[i] << std::endl;
			i++;
		}

		// i = dlugosc napisu
		to_lower(napis, i);

		baza.dodaj(napis, i);

		/*
		std::cout << "\nZawartosc bazy\n";
		baza.wyswietl();
		std::cout << std::endl;
		*/
	}

	while (true)
	{
	usuwanie:
		std::cout << "Podaj napis (jezeli zakonczono usuwanie napisz 0):\n";
		std::cin.getline(napis, 100);
		if (napis[0] == '0')
			goto end;
		int i = 0;
		while (napis[i])		// dopoki nie ma znaku EOL - 0 (zgodnie z konwencja kazdy string sie konczy na 0)
		{
			//std::cout << "Napis[" << i << "] = " << napis[i] << std::endl;
			i++;
		}

		// i = dlugosc napisu
		to_lower(napis, i);

		baza.usun(napis, i);

		/*
		std::cout << "\nZawartosc bazy\n";
		baza.wyswietl();
		std::cout << std::endl;
		*/
	}

	while (true)
	{
	szukanie:
		std::cout << "Podaj napis ktory chcesz wyszukac (jezeli zakonczono szukanie napisz 0):\n";
		std::cin.getline(napis, 100);
		if (napis[0] == '0')
			goto end;
		int i = 0;
		while (napis[i])		// dopoki nie ma znaku EOL - 0 (zgodnie z konwencja kazdy string sie konczy na 0)
		{
			//std::cout << "Napis[" << i << "] = " << napis[i] << std::endl;
			i++;
		}

		// i = dlugosc napisu
		to_lower(napis, i);

		baza.szukaj(napis, i);

		/*
		std::cout << "\nZawartosc bazy\n";
		baza.wyswietl();
		std::cout << std::endl;
		*/
	}

end:
	int c;
	std::cout << "\nDodaj (1)\nUsun(2)\nWyswietl(3)\nZnajdz(4)\nExit(0):\n";
	std::cin >> c;
	std::cout << std::endl;
	switch (c)
	{
	case 0:
		return 0;
	case 1:
		goto dodawanie;
	case 2:
		goto usuwanie;
	case 3:
		baza.wyswietl();
		goto end;
	case 4:
		goto szukanie;
	default:
		std::cout << "Wybierz poprawna opcje\n";
		goto end;
	}
}