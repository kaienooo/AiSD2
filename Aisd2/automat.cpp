#include <iostream>
#include <fstream>

#define MASK0	0x0000000000000000					//	Liczby 8 bajtowe
#define MASK1	0x8000000000000000					//	-

class Automat
{
public:
	int stan;
	int ilStanow;
	unsigned long long alfabet;
	int wielkoscAlfabetu;

	char* alfabetTab;
	char* maszynaStanow;

	Automat(const char* wzorzec, int dl_wzorca);
	~Automat();

	void nextStan(char c);
	int findX(char c);
	void wypiszMaszyneStanow();
};

int AsciiToInt(char c, bool& blad)
{
	int temp = c - '0';
	if (temp < 0 or temp > 9)
	{
		std::cout << "We wzorcu wystepuja znaki inne niz liczby ERROR 3\n";
		blad = true;
		return 3;
	}

	blad = 0;
	return temp;
}

void printString(char* tekst, int len)
{
	for (int i = 0; i < len; i++)
	{
		std::cout << tekst[i];
	}
	std::cout << std::endl;
}

bool compareStrings(char* cmp1, char* cmp2, int minLen)
{
	for (int i = 0; i < minLen; i++)
	{
		if (cmp1[i] != cmp2[i])
		{
			return false;
		}
	}
	return true;
}

int getStringLength(const char* tekst)
{
	int i = 0;
	while (tekst[i] != 0)
	{
		i++;
	}

	return i;
}

char* getPrefix(char* tekst, int dl_prefixu)
{
	char* ret_tekst = new char[dl_prefixu];
	for (int i = 0; i < dl_prefixu; i++)
	{
		ret_tekst[i] = tekst[i];
	}

	return ret_tekst;
}

char* getPrefix(const char* tekst, int dl_prefixu)
{
	char* ret_tekst = new char[dl_prefixu];
	for (int i = 0; i < dl_prefixu; i++)
	{
		ret_tekst[i] = tekst[i];
	}

	return ret_tekst;
}

char* getSuffix(char* tekst, int dl_tekstu, int dl_suffixu)
{
	char* ret_tekst = new char[dl_suffixu];
	for (int i = dl_tekstu - dl_suffixu; i < dl_tekstu; i++)
	{
		ret_tekst[i - dl_tekstu + dl_suffixu] = tekst[i];
	}

	return ret_tekst;
}

char toLower(char c)
{
	if (c < 'A' or (c > 'Z' and c < 'a') or c > 'z')
	{
		return 0;
	}

	if (c >= 'A' and c <= 'Z')
	{
		return 'A' - c + 'a';
	}

	return c;
}

void toLower(char* imie, int dl_imienia)
{
	for (int i = 0; i < dl_imienia; i++)
	{
		imie[i] = toLower(imie[i]);
	}
}

unsigned long long getMaskFromCharacter(char c)
{
	if (c >= 'a' and c <= 'z')
	{
		return (MASK1 >> (c-'a'));
	}

	if (c >= 'A' and c <= 'Z')
	{
		return (MASK1 >> (toLower(c) - 'a'));
	}

	if (c >= '0' and c <= '9')
	{
		return (MASK1 >> (c - '0' + 26));
	}

	return MASK0;
}

int main(int argc, const char* argv[])	// arg 1 - sciezka do pliku, arg 2 - szukany wzorzec, arg 3 tryb (0  - wypisz wszystkie, 1 - tylko zlicz)
																																		// wypisywanie spowalnia przeszukiwanie pliku
																																		// gdyz wypisanie do strumienia wyjsciowego prawdopodobnie wypisuje z ramu
																																		// + do tego napis zajmuje duzo wiecej pamieci niz pare liczb
{
	int tryb = 0;
	bool blad = false;
	if (argc < 3)
	{
		std::cout << "Podaj argumenty programu\narg 1 - sciezka do pliku\narg 2 - szukany wzorzec\narg 3  - tryb (0 - wypisz i licz, 1 - tylko licz)";
		return -1;
	}

	if (argc == 3)
	{
		std::cout << "Nie ma podanego trybu, domyslnie 0 (wypisz wszystkie)\n";
		tryb = 0;
	}

	if (argc == 4)
	{
		tryb = AsciiToInt(argv[3][0], blad);
		if (blad)
		{
			std::cout << "Argument tryb nieprawidlowy ERROR 5\n";
			return tryb;
		}
	}

	
	std::fstream plik(argv[1], std::fstream::in);

	if (!plik)
	{
		std::cout << "Niepoprawny plik ERROR 4\n";
		return 4;
	}

	int dlWzorca = getStringLength(argv[2]);
	Automat automat(argv[2], dlWzorca);

	int ilosc_wystapien = 0;
	int i = 0;
	char c;
	while (plik.get(c))
	{
		if (getMaskFromCharacter(c) == MASK0)						// znak nie jest w obslugiwanym maksymalnym alfabacie (znak bialy lub nie przewidziano jego uzycia)
		{
			i++;
			continue;
		}

		if (!(getMaskFromCharacter(c) & automat.alfabet))		// znak jest w obslugiwanym (ogolnym) alfabecie ale nie w alfabecie (szczegolnym) wzorca 
		{
			automat.stan = 0;
			continue;
		}

		if (getMaskFromCharacter(c) & automat.alfabet)
		{
			automat.nextStan(c);
		}

		if (automat.stan == dlWzorca)
		{
			switch (tryb)
			{
			case 0: // zliczanie i wypisywanie
				std::cout << "Znaleziono wzorzec: " << argv[2] << " w miejscu " << i - dlWzorca << " znaku. Jest to " << ilosc_wystapien + 1 << " wystapienie\n";
			case 1: // tylko zliczanie
				ilosc_wystapien++;
				break;
			default:
				break;
			}
		}
		i++;
	}

	std::cout << "\nLacznie " << ilosc_wystapien << " trafien";

	plik.close();
}

//Automat
Automat::Automat(const char* wzorzec, int dlWzorca) : wielkoscAlfabetu(dlWzorca),stan(0),ilStanow(dlWzorca + 1)
{
	alfabet = MASK0;		// kazdy bit ma swoj znak, nie rozroznia sie znakow duzych/malych. Dostepne znaki to male litery (26), liczby (10), 28 (wolne - w przyszlosci mozna zaimplementowac znaki specjalne '/','-',';','('.')')

	for (int i = 0; i < dlWzorca; i++)
	{
		alfabet = alfabet | getMaskFromCharacter(wzorzec[i]);
	}

	wielkoscAlfabetu = (int)__popcnt64(alfabet);	// wbudowana funkcja w procesor (zlicza ilosc 1), oszczedza kilka cykli (chat powiedzial ze procesory po okolo 2007/8 obsluguja ta instrukcje wiec nie powinno byc problemu)

	alfabetTab = new char[wielkoscAlfabetu];
	{
		int i = 0;
		char c = 'a';
		unsigned long long alfabetCp = alfabet;

		//std::cout << std::hex << alfabetCp << std::dec << std::endl;

		while (alfabetCp && i < wielkoscAlfabetu)
		{
			if (alfabetCp & MASK1)
			{
				alfabetTab[i++] = c;
			}
			alfabetCp = alfabetCp << 1;
			c++;
			if (c == 'z' + 1)
			{
				c = '0';
			}
		}
	}

	/*
	for (int i = 0; i < wielkoscAlfabetu; i++)
	{
		std::cout << alfabetTab[i];
	}
	std::cout << std::endl;
	*/
	

	maszynaStanow = new char[wielkoscAlfabetu * (dlWzorca+1)];
	for (int i = 0; i < wielkoscAlfabetu * (dlWzorca+1); i++)
	{
		maszynaStanow[i] = 0;
	}
	// kolumny w maszynieStanow zawsze sa znakami alfaberu idac od lewej do prawej ( znaki male -> liczby )

	// przyklad wzorzec = "abc" | "bca" | "cba" ..
	// automat[n*wielkosc_alfabetu + m]			// n = 2, m = 1
	//		a	b	c
	//	0	
	//	1
	//	2		x
	

	//-------------------------------WYPELNIANIE MASZYNY STANOW-------------------------------------------------------------------------

	char* temp = new char[dlWzorca + 1];					// tablica pomocnicza do obliczania stanow
	for (int i = 0; i < dlWzorca + 1; i++)
	{
		temp[i] = 0;
	}

	for (int y = 0; y < dlWzorca + 1; y++)
	{
		for (int x = 0; x < wielkoscAlfabetu; x++)
		{
			temp[y] = alfabetTab[x];
			bool kon = false;
			for (int i = y + 1; i > 0 && (!kon); i--)
			{
				char* cmp2 = getPrefix(wzorzec, i);
				char* cmp1 = getSuffix(temp, y + 1, i);

				if (compareStrings(cmp1, cmp2, i))
				{
					maszynaStanow[y * wielkoscAlfabetu + x] = i;
					kon = true;
				}

				delete[] cmp1;
				delete[] cmp2;
			}
		}
		temp[y] = wzorzec[y];
	}

	delete[] temp;
	//delete[] wzorzec;
}

Automat::~Automat()
{
	delete[] alfabetTab;
	delete[] maszynaStanow;
}

void Automat::nextStan(char c)
{
	int x = findX(c);

	stan = (int)maszynaStanow[stan * wielkoscAlfabetu + x];
}

int Automat::findX(char c)
{
	for (int i = 0; i < wielkoscAlfabetu; i++)
	{
		if (alfabetTab[i] == c)
		{
			return i;
		}
	}
}

void Automat::wypiszMaszyneStanow()
{
	for (int y = 0; y < ilStanow; y++)
	{
		for (int x = 0; x < wielkoscAlfabetu; x++)
		{
			std::cout << (int)maszynaStanow[y * wielkoscAlfabetu + x] << " ";
		}
		std::cout << std::endl;
	}
}
