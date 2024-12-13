//https://github.com/kaienooo/AiSD2/
//niektore wyjasnienia i dane

// Wyszukuje liczbe w ciagu liczb - moim zdaniem zooptymalizowane pod wlasnie liczby, a nie traktowanie liczb jako ciagow znakow (stad niewystepowanie zwyklego algorytu naiwnego dla tablicy znakow)
// Nie optymalizowano algorytmu jako takiego, ale w to jaki sposob obsluguje dane

#include <iostream>
#include <fstream>

int potega(int base, int exponent)
{

	int return_value = base;
	for (int i = 1; i < exponent; i++)
	{
		return_value *= base;
	}

	return return_value;
}

int horner(char* liczba_tab, int dl_liczby)
{
	int i = 0;
	int ret_liczba = 0;
	while (i < dl_liczby)
	{
		ret_liczba = ret_liczba * 10 + liczba_tab[i];
		i++;
	}
	return ret_liczba;
}


inline int hashuj(char* liczba_tab, int dl_liczby, int hash_int)
{
	return horner(liczba_tab, dl_liczby) % hash_int;
}


inline int hashuj(int liczba, int hash_int)
{
	return (liczba % hash_int);
}


int charPtrToInt(const char* napis, int* dl_napisu, bool &blad)			//const char* gwarantuje, ze nie zmienimi ciagu znakow
{
	//szukamy dlugosc
	while (napis[*dl_napisu]) // dopoki nie zakonczony string
	{
		(*dl_napisu)++;
	}

	char* napisInt_tab = new char[(*dl_napisu)];

	for (int i = 0; i < (*dl_napisu); i++)
	{
		napisInt_tab[i] = napis[i] - '0';
		if (i == 0 && napisInt_tab[i] == 0)
		{
			std::cout << "Wzorzec zaczyna sie na 0 - ERROR 2\n";
			blad = true;
			delete[] napisInt_tab;
			return 2;
		}
		if (napisInt_tab[i] < 0 or napisInt_tab[i] > 9)
		{
			std::cout << "We wzorcu wystepuja znaki ERROR 3\n";
			blad = true;
			delete[] napisInt_tab;
			return 3;
		}
	}

	int napisInt = horner(napisInt_tab, *dl_napisu);
	delete[] napisInt_tab;

	blad = 0;
	return napisInt;
}

int charPtrToInt(const char* napis, int dl_napisu, bool& blad)	// wariant z znana dlugoscia
{
	char* napisInt_tab = new char[dl_napisu];

	for (int i = 0; i < dl_napisu; i++)
	{
		napisInt_tab[i] = napis[i] - '0';
		if (i == 0 && napisInt_tab[i] == 0)
		{
			std::cout << "Wzorzec zaczyna sie na 0 - ERROR 2\n";
			blad = true;
			delete[] napisInt_tab;
			return 2;
		}
		if (napisInt_tab[i] < 0 or napisInt_tab[i] > 9)
		{
			std::cout << "We wzorcu wystepuja znaki ERROR 3\n";
			blad = true;
			delete[] napisInt_tab;
			return 3;
		}
	}

	int napisInt = horner(napisInt_tab, dl_napisu);
	delete[] napisInt_tab;

	blad = false;
	return napisInt;
}

int AsciiToInt(char c, bool& blad)	// szczegolny przypadek int charPtrToInt(const char* napis, int dl_napisu, bool& blad) gdy dl_napisu = 1
{
	int temp = c - '0';
	if (temp < 0 or temp > 9)
	{
		std::cout << "We wzorcu wystepuja znaki ERROR 3\n";
		blad = true;
		return 3;
	}

	blad = 0;
	return temp;
}

void naiwny(int* liczbydowyszukania, int* wzorzec, int dl_wzorca)	// narazie puste bo chyba nie bedzie potrzeby porownywania tablic
{
}

inline bool naiwny(int liczba, int wzorzec)	// przy tej implementacji tylko taki jest chyba potrzebny
{
	return (liczba == wzorzec) ? true : false;
}


inline int nextNapisInt(int c, int ten_to_the_x, int napis_int)
{
	return (10 * (napis_int - (napis_int / ten_to_the_x) * ten_to_the_x) + c);
}


/*
void inicjalizujBufor(char*& bufor, int lenght)
{
	char* temp = new char[lenght];
	if (bufor)
	{
		for (int i = 0; i < lenght >> 1; i++)
		{
			temp[i] = bufor[i];			// tutaj mam nadzieje ze kompilator autoamtycznie zrobi kod assemblera na avx256, bo nie chce mi sie tego samemu
		}
		delete[] bufor;
	}
	
	bufor = temp;
}
*/

int main(int argc, char* argv[])	// arg 1 - sciezka do pliku, arg 2 - szukany wzorzec, arg 3 - hash_int (rozmiar bazy hashowania), arg 4 tryb (0  - wypisz wszystkie, 1 - tylko zlicz)
// wypisywanie spowalnia przeszukiwanie pliku
// gdyz wypisanie do strumienia wyjsciowego prawdopodobnie wypisuje z ramu
// + do tego napis zajmuje duzo wiecej pamieci niz pare liczb
{

	/*
	for (int i = 0; i < argc; i++)
	{
		delete[] argv[i];
	}
	*/

	//ARGUMENTY TESTOWE
	/*
	argc = 5;

	char** argt = new char*[5];
	argt[0] = new char[10];
	memmove(argt[0], "Aisd2.exe", 10);

	argt[1] = new char[25];
	memmove(argt[1], "C:\\aisd2\\Aisd2\\dane_cop2", 25);

	argt[2] = new char[4];
	memmove(argt[2], "213", 4);

	argt[3] = new char[3];
	memmove(argt[3], "13", 3);

	argt[4] = new char[2];
	memmove(argt[4], "0", 2);


	argv = argt;
	*/


	int hash_int = 0;
	bool blad = 0;
	int tryb = 0;
	if (argc < 3)
	{
		std::cout << "Podaj argumenty programu\narg 1 - sciezka do pliku\narg 2 - szukany wzorzec\narg 3 - hash_int (rozmiar bazy hashowania) default = 13\narg 4 - tryb (0 - wypisz i licz, 1 - tylko licz)";
		return -1;
	}
	else if (argc == 3)
	{
		hash_int = 13;			// defaultowe
		tryb = 0;
	}
	else if (argc == 4)
	{
		int temp = 0;
		hash_int = charPtrToInt(argv[3], &temp, blad);
		if (blad)
		{
			std::cout << "Argument hash_int nieprawidlowy ERROR 5\n";
			return hash_int;
		}

		tryb = 0;
	}
	else if (argc == 5)
	{
		int temp = 0;
		hash_int = charPtrToInt(argv[3], &temp, blad);
		if (blad)
		{
			std::cout << "Argument hash_int nieprawidlowy ERROR 5\n";
			return hash_int;
		}
		tryb = AsciiToInt(argv[4][0], blad);
		if (blad)
		{
			std::cout << "Argument tryb nieprawidlowy ERROR 5\n";
			return tryb;
		}
	}
	
	std::cout << "ARG1: " << argv[1] << "\nARG2: " << argv[2] << "\nARG3: " << argv[3] << std::endl;

	std::fstream plik(argv[1], std::fstream::in);

	if (!plik)
	{
		std::cout << "Niepoprawny plik ERROR 4\n";
		return 4;
	}

	int dl_wzorca = 0;

	int wzorzec_int = charPtrToInt(argv[2], &dl_wzorca, blad);			//int charPtrToInt(const char* napis, int* dl_napisu, bool &blad)

	if (blad)
	{
		return wzorzec_int;	
	}

	char* temp_napis = new char[dl_wzorca];

	int i = 0;
	int z = 0;	// ile poczatkowych znakow zaladowano
	int napis_int = 0;
	int ten_to_the_x = potega(10, dl_wzorca - 1);

	int ilosc_wystapien = 0;

	char c;
	while (plik.get(c))
	{
		//std::cout << c << " " << i <<std::endl;
		// sekcja poczatkowa
		if (c == ' ' or c == '\n' or c == '\t')
		{
			i++;
			continue;
		}

		if (z < dl_wzorca)				// 0,1,2, .. ,dl_wzorca - 1
		{
			temp_napis[z] = c;
			z++;

			if (z == dl_wzorca)
			{
				napis_int = charPtrToInt(temp_napis, dl_wzorca, blad);		//int charPtrToInt(const char* napis, int dl_napisu, bool& blad)
				if (blad)
				{
					std::cout << " Poczatkowe " << dl_wzorca << "zawiera nieprawidlowy znak ERROR 6\n";
					return napis_int;
				}
				delete[] temp_napis;
			}
		}
		else
		{
			int cInt = AsciiToInt(c, blad);
			if (blad)
			{
				std::cout << " Nieprawidlowy znak na " << i << " miejscu\n";
				return cInt;
			}
			napis_int = nextNapisInt(cInt, ten_to_the_x, napis_int);
		}

		if (hashuj(napis_int, hash_int) == hashuj(wzorzec_int, hash_int))
		{
			if (naiwny(napis_int, wzorzec_int))						// no i uzycie algorytmu algorytmu naiwnego mimo ze on tylko porownuje dwie liczby, ale zeby bylo jasne
			{
				switch (tryb)
				{
				case 0: // zliczanie i wypisywanie
					std::cout << "Znaleziono wzorzec: " << wzorzec_int << " w miejscu " << i - dl_wzorca + 1<< " znaku. Jest to " << ilosc_wystapien + 1 << " wystapienie\n";
				case 1: // tylko zliczanie
					ilosc_wystapien++;
					break;
				default:
					break;
				}
			}
		}
		
		i++;

	}

	std::cout << "\nLacznie " << ilosc_wystapien << " trafien";

	plik.close();
}