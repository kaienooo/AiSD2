Algorytmy i struktury danych 2

Spis tresci:

DRZEWA:
1) kodv1.cpp	kod fatalny, wszystko wymieszane
2) bst.cpp		implementacja drzewa bst
3) avl.cpp		implementacja drzewa avl
4) huffman.cpp	nieskonczona implementacja kodowania huffmana (generuje drzewo, nic wiecej)
	
FUNKCJE HASHUJACE:

5) hash.cpp		zapis/odczyt/usuwanie z "bazy" opartej o hashowanie

WYSZUKIWANIE WZORCA:
1) wzorzec.cpp 	wyszukiwanie wzorca algorytmem Rabina-Karpa
2) automat.cpp 	wyszukiwanie wzorca automatem skonczonym
3) akmp.cpp 	wyszukiwanie wzorca algorytmem Knutha-Morrisa-Pratta

przyklad z wnetrza katalogu aisd2

	 .\Build\x64\Release\bin\Aisd2.exe .\Aisd2\dane 0
  
1) dla programow wzorzec.cpp, automat.cpp, akmp.cpp uruchomic poleceniem
{.exe path} {data path} {wzorzec} ?{hash int} {tryb}
	
gdzie:

-exe path	sciezka do pliku .exe

-data path	sciezka do pliku z tekstem

-wzorzec	wpisac wzorzec

UWAGA!
	- algorytm w wzorzec.cpp przyjmuje tylko niezerowe liczby jako wzorzec
	- automat skonczony przyjmuje we wzorcu litery i liczby, bez znakow specjalnych
 
-tryb:

0 - tryb odczyt/zliczanie

1 - tylko zliczanie
 
-hash int	tylko dla wzorzec.cpp, domyslnie =13, "wielkosc bazy" liczba ktora jest argumentem operacji modulo
