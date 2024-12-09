#include <iostream>
#include <iomanip>

#define MASK	0x00000000
#define MASK1	0x80000000

int getStringLength(char* tekst)
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
	delete[] tekst;

	return ret_tekst;
}


char* getSuffix(char* tekst, int dl_tekstu, int dl_prefixu)
{
	char* ret_tekst = new char[dl_prefixu];
	for (int i = dl_tekstu - dl_prefixu; i < dl_tekstu; i++)
	{
		ret_tekst[i] = tekst[i];
	}
	delete[] tekst;

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

int getMaskFromCharacter(char c)
{
	return (MASK | (MASK1 >> (c-'a')));
}

char* generateAutomat(char* wzorzec, int dl_wzorca)
{
	int ilosc_unikalnych_znakow = 0;
	int alfabet = 0;		// kazdy bit ma swoj znak

	delete[] wzorzec;
}

int main()
{
	char test = 'a';

	std::cout <<std::hex << getMaskFromCharacter(test) << std::dec <<std::endl;
}