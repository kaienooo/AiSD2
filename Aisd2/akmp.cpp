#include <iostream>
#include <fstream>

class KMP
{
public:
	int* tablicaPrzesuniec;
	int dlTab;

	KMP(char* wzorzec, int dlWzorca);
	~KMP();

	void wypiszTablice();
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
	KMP kmp((char*)argv[2], dlWzorca);

	int ilosc_wystapien = 0;
	int i = 0;
	int s = 0;
	int q = 0;
	char c;

	while (plik.get(c))
	{
		while (i < s)
		{
			i++;
			continue;
		}

		if (c == argv[2][q])
		{
			q++;
			i++;
			s++;

			if (q == dlWzorca)
			{
				q = 0;
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
		}
		else
		{
			if (q == 0)
			{
				s++;
				i++;
			}
			else
			{
				s += q - kmp.tablicaPrzesuniec[q];
			}
		}
	}

	std::cout << "\nLacznie " << ilosc_wystapien << " trafien";

	plik.close();
}

KMP::KMP(char* wzorzec, int dlWzorca) : dlTab(dlWzorca)
{
	if (dlTab == 1)
	{
		*tablicaPrzesuniec = 0;
		return;
	}
	tablicaPrzesuniec = new int[dlTab];
	memset(tablicaPrzesuniec, 0, dlTab);

	for (int i = 2; i < dlTab + 1; i++)
	{
		for (int j = 1; j < i; j++)
		{
			char* cmp1 = getPrefix(wzorzec,j);
			char* cmp2 = getSuffix(wzorzec,i,j);

			if (compareStrings(cmp1, cmp2, j))
			{
				tablicaPrzesuniec[i - 1] = j;
			}
		}
	}
}

KMP::~KMP()
{
	delete[] tablicaPrzesuniec;
}

void KMP::wypiszTablice()
{
	for (int i = 0; i < dlTab; i++)
	{
		std::cout << tablicaPrzesuniec[i] << " ";
	}
	std::cout << std::endl;
}
