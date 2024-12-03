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

int main(int argc, const char* argv[])	// arg 1 - sciezka do pliku, arg 2 - szukany wzorzec, arg 3 - hash_int (rozmiar bazy hashowania), arg 4 tryb (0  - wypisz wszystkie, 1 - tylko zlicz)
																																		// wypisywanie spowalnia przeszukiwanie pliku
																																		// gdyz wypisanie do strumienia wyjsciowego prawdopodobnie wypisuje z ramu
																																		// + do tego napis zajmuje duzo wiecej pamieci niz pare liczb
{
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
	int napis_int = 0;
	int ten_to_the_x = potega(10, dl_wzorca - 1);

	int ilosc_wystapien = 0;

	char c;
	while (plik.get(c))
	{
		//std::cout << c << " " << i <<std::endl;
		// sekcja poczatkowa
		if (c == ' ' or c == '\n' or c == '\t' or c == '-')
		{
			i++;
			continue;
		}
		if (i < dl_wzorca)
		{
			temp_napis[i] = c;
		}
		else
		{
			if (i == dl_wzorca)
			{
				napis_int = charPtrToInt(temp_napis, dl_wzorca, blad);		//int charPtrToInt(const char* napis, int dl_napisu, bool& blad)
				if (blad)
				{
					std::cout << " Poczatkowe " << dl_wzorca << "zawiera nieprawidlowy znak ERROR 6\n";
					return napis_int;
				}
				delete[] temp_napis;
			}		
			if (hashuj(napis_int, hash_int) == hashuj(wzorzec_int, hash_int))
			{
				if (naiwny(napis_int, wzorzec_int))
				{
					switch (tryb)
					{
					case 0: // zliczanie i wypisywanie
						std::cout << "Znaleziono wzorzec: " << wzorzec_int << " w miejscu " << i - dl_wzorca << " znaku. Jest to " << ilosc_wystapien + 1<< " wystapienie\n";
					case 1: // tylko zliczanie
						ilosc_wystapien++;
						break;
					default:
						break;
					}


				}
			}
			//napis_int(i + 1) = ...

			int cInt = AsciiToInt(c, blad);
			if (blad)
			{
				std::cout << "Nieprawidlowy znak na " << i << " miejscu ERROR 6\n";
				return cInt;
			}

			napis_int = nextNapisInt(cInt, ten_to_the_x, napis_int);
		}
		i++;
	}

	std::cout << "\nLacznie " << ilosc_wystapien << " trafien";

	plik.close();
}