#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <locale.h>

struct StrukturaZdjecia
{
	char magic[3];
	char nazwa[20];
	int kolumna;
	int wiersz;
	int skala_szarosci;
	int **tabela_pixeli;
};
void podaj_nazwe(char *nazwa)
{
	printf("Podaj nazwę pliku:\n");
	scanf("%123s", nazwa);
	strcat(nazwa, ".PGM");
}
void zapis(struct StrukturaZdjecia *o)
{
	FILE *plik;
	char nowa_nazwa[100];
	int i = 0, j = 0;
	printf("Podaj nazwę pliku do zapisania:\n");
	scanf("%s", &nowa_nazwa);
	strcat(&nowa_nazwa, ".pgm");
	plik = fopen(nowa_nazwa, "w");
	fprintf(plik, "%s\n%d %d\n%d\n", o->magic, o->kolumna, o->wiersz, o->skala_szarosci);
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			fprintf(plik, "%d ", o->tabela_pixeli[i][j]);
		}
		fprintf(plik, "\n");
	}
	fclose(plik);
}
void filtr_gaussa(struct StrukturaZdjecia *o)
{
	int tab_wag[3][3] = { {1,2,1}, {2,4,2}, {1,2,1} };
	int sumWaga = 16, sum = 0;
	int i = 0, j = 0, k = 0, l = 0;
	int **buffor;
	buffor = (int**)malloc((*o).wiersz * sizeof(int));
	for (i = 0; i < (*o).wiersz; i++)
	{
		buffor[i] = (int*)calloc((*o).kolumna, sizeof(int));
		if (buffor[i] == NULL)
		{
			for (j = i - 1; j >= 0; j--)
				free(buffor[j]);
			free(buffor);
			printf("Błąd.\n");
		}
	}
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			buffor[i][j] = o->tabela_pixeli[i][j];
		}
	}
	for (i = 0; i < (*o).wiersz - 2; i++)
	{
		for (j = 0; j < (*o).kolumna - 2; j++)
		{
			sum = 0;
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					sum = sum + (buffor[i + k][j + l] * tab_wag[k][l]);
				}
			}
			o->tabela_pixeli[i][j] = (int)(sum / sumWaga);
		}
	}
	for (i = 0; i < (*o).wiersz; i++)
	{
		free(buffor[i]);
	}
	free(buffor);
}
void obrot(struct StrukturaZdjecia *o)
{
	int i = 0, j = 0, k = 0;
	int **buffor_tab = NULL;
	int buffor, obro;
	printf("Ile obrotów o 90 stopni chciałbyś wykonać?\n");
	while (scanf("%i", &obro) == 0)
	{
		printf("Wybierz wartosc liczbowa: ");
		while ('\n' != getchar());
	}
	obro = obro % 4;
	for (k = 0; k < obro; k++)
	{
		buffor_tab = (int**)malloc(o->kolumna * sizeof(int*));
		if (buffor_tab != NULL)
		{
			for (i = 0; i < o->kolumna; i++)
			{
				buffor_tab[i] = (int*)malloc(o->wiersz * sizeof(int));
				if (buffor_tab[i] == NULL)
				{
					for (i; i >= 0; i--)
					{
						free(buffor_tab[i]);
					}
					free(buffor_tab);
				}
			}
		}
		for (i = 0; i < o->wiersz; i++)
		{
			for (j = 0; j < o->kolumna; j++)
			{
				buffor_tab[j][o->wiersz - i - 1] = o->tabela_pixeli[i][j];
			}
		}
		for (i = 0; i < o->wiersz; i++)
		{
			free(o->tabela_pixeli[i]);
		}
		free(o->tabela_pixeli);
		buffor = o->kolumna;
		o->kolumna = o->wiersz;
		o->wiersz = buffor;
		o->tabela_pixeli = buffor_tab;
	}
}
void pieprz_sol(struct StrukturaZdjecia *o)
{
	int i = 0, j = 0;
	int poziom_zaszumienia;
	do
	{
		printf("Podaj poziom zaszumienia od 0 do 100:\n");
		while (scanf("%i", &poziom_zaszumienia) == 0)
		{
			printf("Wybierz wartosc liczbowa: ");
			while ('\n' != getchar());
		}
	} while ((poziom_zaszumienia < 0) || (poziom_zaszumienia > 100));
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			if (rand() % 100 <= poziom_zaszumienia)
			{
				if (rand() % 2 == 1)
				{
					o->tabela_pixeli[i][j] = o->skala_szarosci;
				}
				else
				{
					o->tabela_pixeli[i][j] = 0;
				}
			}
		}
	}
}
void negatyw(struct StrukturaZdjecia *o)
{
	int i = 0, j = 0;
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			o->tabela_pixeli[i][j] = o->skala_szarosci - o->tabela_pixeli[i][j];
		}
	}
}
void progowanie(struct StrukturaZdjecia *o)
{
	int i = 0, j = 0, prog;
	printf("Podaj wartość progowania:\n");
	while (scanf("%i", &prog) == 0)
	{
		printf("Wybierz wartosc liczbowa: ");
		while ('\n' != getchar());
	}
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			if (o->tabela_pixeli[i][j] < prog)
				o->tabela_pixeli[i][j] = 0;
			else
				o->tabela_pixeli[i][j] = o->skala_szarosci;

		}
	}
}
void stworz_histogram(struct StrukturaZdjecia *o)
{
	int histogram[256][2] = { 0 };
	int i = 0, j = 0, a = 0;
	FILE* plik;
	plik = fopen("histogram.csv", "w");
	if (stworz_histogram == NULL)
	{
		printf("Wystapil błąd.\n");
	}
	else
	{
		printf("Udało sie otworzyć plik histogram.csv\n");
	}
	for (i = 0; i < 256; i++)
	{
		histogram[i][0] = i;
	}
	while (a <= 255)
	{
		histogram[a][1] = 0;
		for (i = 0; i < o->wiersz; i++)
		{
			for (j = 0; j < o->kolumna; j++)
			{
				if (a == o->tabela_pixeli[i][j])
				histogram[a][1]++;
			}
		}
		fprintf(plik, "%d;%d\n", histogram[a][0], histogram[a][1]);
		a++;
	}
	fclose(plik);
}
void odbicie(struct StrukturaZdjecia *o)
{
	int **buffor;
	int jaki_obrot = 0, i = 0, j = 0;
	buffor = malloc((*o).wiersz * sizeof(int));
	for (i = 0; i < (*o).wiersz; i++)
	{
		buffor[i] = calloc((*o).kolumna, sizeof(int));
		if (buffor[i] == NULL)
		{
			for (j = i - 1; j >= 0; j--)
			{
				free(buffor[j]);
			}
			free(buffor);
			printf("Błąd.\n");
		}
	}
	for (i = 0; i < o->wiersz; i++)
	{
		for (j = 0; j < o->kolumna; j++)
		{
			buffor[i][j] = o->tabela_pixeli[i][j];
		}
	}
	printf("Względem której osi obraz ma byc odbity?\n 1. Y \n Inna liczba. X \n");
	while (scanf("%i", &jaki_obrot) == 0)
	{
		printf("Wybierz wartość liczbową: ");
		while ('\n' != getchar());
	}
	if (jaki_obrot == 1)
	{
		for (i = 0; i < (*o).wiersz; i++)
		{
			for (j = 0; j < (*o).kolumna; j++)
			{
				o->tabela_pixeli[i][j] = buffor[i][(*o).kolumna - j - 1];
			}
		}
	}
	else
	{
		for (i = 0; i < (*o).kolumna; i++)
		{
			for (j = 0; j < (*o).wiersz; j++)
			{
				o->tabela_pixeli[j][i] = buffor[o->wiersz - j - 1][i];
			}
		}
	}
	for (i = 0; i < (*o).wiersz; i++)
	{
		free(buffor[i]);
	}
	free(buffor);
}
void lista_obrazow(struct StrukturaZdjecia *zbior_zdjec, int n)
{
	int i = 0;
	if (n <= 0)
	{
		printf("Lista obrazów jest pusta.\nDodaj obraz.\n");
	}
	for (i = 0; i < n; i++)
	{
		printf("%d. %s\n", i + 1, zbior_zdjec[i].nazwa);
	}
}
void usuwanie(struct StrukturaZdjecia *o)
{
	int i = 0;
	for (i = 0; i < (*o).wiersz; i++)
	{
		free((*o).tabela_pixeli[i]);
	}
	free((*o).tabela_pixeli);
}
void wczytajObraz(struct StrukturaZdjecia *o)
{
	int i = 0, j = 0;
	FILE* plikObraz = NULL;
	while (plikObraz == NULL)
	{
		podaj_nazwe((*o).nazwa);
		plikObraz = fopen((*o).nazwa, "r");
	}
	fscanf(plikObraz, "%s", (*o).magic);
	(*o).kolumna = (intScanf(plikObraz));
	(*o).wiersz = (intScanf(plikObraz));
	(*o).skala_szarosci = (intScanf(plikObraz));
	(*o).tabela_pixeli = malloc((*o).wiersz * sizeof(int));
	for (i = 0; i < (*o).wiersz; i++)
	{
		(*o).tabela_pixeli[i] = calloc((*o).kolumna, sizeof(int));
		if ((*o).tabela_pixeli[i] == NULL)
		{
			printf("ERROR\n");
			for (j = 0; j < i; j++)
			{
				free((*o).tabela_pixeli[j]);
			}
			free((*o).tabela_pixeli);
		}
	}
	while (!feof(plikObraz))
	{
		for (i = 0; i < (*o).kolumna; i++) 
		{
			fscanf(plikObraz, "%d", &(*o).tabela_pixeli[j][i]);
		}
		j++;
	}
	printf("Ilość kolumn wynosi: %d\n", (*o).kolumna);
	printf("Ilosc wierszy wynosi: %d\n", (*o).wiersz);
	printf("Skala szarości wynosi: %d\n", (*o).skala_szarosci);
	fclose(plikObraz);
}
int intScanf(FILE* plik)
{
	int x;
	char komentarz;
	if (fscanf(plik, "%d", &x))
		return x;
	else
	{
		if (fgetc(plik) == '#')
		{
			do
			{
				komentarz = fgetc(plik);
			} while (komentarz != '\n');
			return intScanf(plik);
		}
	}
}
int main() 
{
	int i = 0;
	struct StrukturaZdjecia *dane;
	int ilosc_obrazow = 0;
	int losowanie = NULL;
	struct StrukturaZdjecia *zbior_zdjec;
	dane = (struct StrukturaZdjecia*)malloc(1 * sizeof(struct StrukturaZdjecia));
	zbior_zdjec = (struct StrukturaZdjecia*)malloc(1 * sizeof(struct StrukturaZdjecia));
	dane = NULL;
	srand(losowanie);
	setlocale(LC_ALL, "polish_poland");
	int wybor;
	printf("Co chciałbyś zrobić:\n 1. Wczytać obraz.\n Inna liczba. Zakończyć. \n");
	while (scanf("%i", &wybor) == 0)
	{
		printf("Wybierz wartość liczbową: ");
		while ('\n' != getchar());
	}
	switch (wybor)
	{
	case 1:
		system("cls");
		zbior_zdjec = realloc(zbior_zdjec, (ilosc_obrazow + 1) * sizeof(struct StrukturaZdjecia));
		wczytajObraz(zbior_zdjec + ilosc_obrazow);
		ilosc_obrazow ++;
		int wybor1;
		do
		{
			printf("Co dalej:\n 1. Dodaj kolejny obraz.\n 2. Wyświetl listę obrazów.\n 3. Usuń obraz z tablicy.\n 4. Wybierz aktywny obraz.\n Inna liczba. Zakończ.\n");
			while (scanf("%i", &wybor1) == 0)
			{
				printf("Wybierz wartość liczbową: ");
				while ('\n' != getchar());
			}
			switch (wybor1)
			{
			case 1:
				system("cls");
				zbior_zdjec = realloc(zbior_zdjec, (ilosc_obrazow + 1) * sizeof(struct StrukturaZdjecia));
				wczytajObraz(zbior_zdjec + ilosc_obrazow);
				ilosc_obrazow++;
				break;
			case 2:
				system("cls");
				lista_obrazow(zbior_zdjec, ilosc_obrazow);
				break;
			case 3:
				system("cls");
				if (ilosc_obrazow == 0)
				{
					printf("Lista obrazów jest pusta.\n");
				}
				else
				{ 
					printf("Wybierz obraz, który chcesz usunąć:\n");
					lista_obrazow(zbior_zdjec, ilosc_obrazow);
					while (scanf("%i", &wybor) == 0)
					{
						printf("Wybierz wartość liczbową: ");
						while ('\n' != getchar());
					}
					wybor--;
					while (wybor >= ilosc_obrazow || wybor < 0)
					{
						printf("Pod takim numerem nie ma obrazu.\n");
						printf("Wybierz nowy obraz:\n");
						while (scanf("%i", &wybor) == 0)
						{
							printf("Wybierz wartość liczbową: ");
							while ('\n' != getchar());
						}
						wybor--;
					}
					usuwanie(zbior_zdjec + wybor);
					for (i = wybor; i < ilosc_obrazow; i++)
					zbior_zdjec[i] = zbior_zdjec[i + 1];
					ilosc_obrazow--;
					zbior_zdjec = realloc(zbior_zdjec, (ilosc_obrazow) * sizeof(struct StrukturaZdjecia));
					dane = NULL;
				}
				break;
			case 4:
				system("cls");
				if (ilosc_obrazow == 0)
				{
					printf("Lista obrazow jest pusta.\n");
					break;
				}
				else
				{
					printf("Wybierz aktywny obraz:\n");
					lista_obrazow(zbior_zdjec, ilosc_obrazow);
					while (scanf("%i", &wybor) == 0)
					{
						printf("Wybierz wartość liczbową: ");
						while ('\n' != getchar());
					}
					wybor--;
					while (wybor >= ilosc_obrazow || wybor < 0)
					{
						printf("Pod takim numerem nie ma obrazu.\n");
						printf("Wybierz nowy obraz:\n");
						while (scanf("%i", &wybor) == 0)
						{
							printf("Wybierz wartość liczbową: ");
							while ('\n' != getchar());
						}
						wybor--;
					}
					dane = zbior_zdjec + wybor;
					printf("Co chcesz zrobić z danym obrazem:\n 1. Obrócić o 90k stopni.\n 2. Zrobić histogram.\n 3. Wykonać progowanie/negatyw/odbicie.\n 4. Dodać pieprz i sól.\n 5. Przefiltrować filtrem Gaussa.\n Inna liczba. Zapisać.\n");
					while (scanf("%i", &wybor) == 0)
					{
						printf("Wybierz wartość liczbową: ");
						while ('\n' != getchar());
					}
					switch (wybor)
					{
					case 1:
						system("cls");
						obrot(dane);
						printf("Czy chcesz zapisać obrócony obraz do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
						while (scanf("%i", &wybor) == 0)
						{
							printf("Wybierz wartość liczbową: ");
							while ('\n' != getchar());
						}
						switch (wybor)
						{
						case 1:
							system("cls");
							zapis(dane);
							break;
						default:
							break;
						}
						break;
					case 2:
						system("cls");
						stworz_histogram(dane);
						break;
					case 3:
						system("cls");
						printf("Co dokładnie chcesz wykonać:\n 1. Progowanie.\n 2. Negatyw.\n Inna liczba: Odbicie.\n");
						while (scanf("%i", &wybor) == 0)
						{
							printf("Wybierz wartość liczbową: ");
							while ('\n' != getchar());
						}
						switch (wybor)
						{
						case 1:
							system("cls");
							progowanie(dane);
							printf("Czy chcesz zapisać obraz po progowaniu do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
							while (scanf("%i", &wybor) == 0)
							{
								printf("Wybierz wartość liczbową: ");
								while ('\n' != getchar());
							}
							switch (wybor)
							{
							case 1:
								system("cls");
								zapis(dane);
								break;
							default:
								break;
							}
							break;
						case 2:
							system("cls");
							negatyw(dane);
							printf("Czy chcesz zapisać negatyw do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
							while (scanf("%i", &wybor) == 0)
							{
								printf("Wybierz wartość liczbową: ");
								while ('\n' != getchar());
							}
							switch (wybor)
							{
							case 1:
								system("cls");
								zapis(dane);
								break;
							default:
								break;
							}
							break;
						default:
							system("cls");
							odbicie(dane);
							printf("Czy chcesz zapisać odbity obraz do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
							while (scanf("%i", &wybor) == 0)
							{
								printf("Wybierz wartość liczbową: ");
								while ('\n' != getchar());
							}
							switch (wybor)
							{
							case 1:
								system("cls");
								zapis(dane);
								break;
							default:
								break;
							}
							break;
						}
						break;
					case 4:
						system("cls");
						pieprz_sol(dane);
						printf("Czy chcesz zapisać obraz z filtrem pieprz i sól do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
						while (scanf("%i", &wybor) == 0)
						{
							printf("Wybierz wartość liczbową: ");
							while ('\n' != getchar());
						}
						switch (wybor)
						{
						case 1:
							system("cls");
							zapis(dane);
							break;
						default:
							break;
						}
						break;
					case 5:
						system("cls");
						filtr_gaussa(dane);
						printf("Czy chcesz zapisać obraz odfiltrowany Gaussem do pliku:\n 1. Tak.\n Inna liczba: Nie.\n");
						while (scanf("%i", &wybor) == 0)
							{
								printf("Wybierz wartość liczbową: ");
								while ('\n' != getchar());
							}
						switch (wybor)
							{
							case 1:
								system("cls");
								zapis(dane);
								break;
							default:
								break;
							}
						break;
					default:
						system("cls");
						zapis(dane);
						break;
					}
					break;
				}
			default:
				break;
			}
		} while ((wybor1 > 0) && (wybor1 < 5));
		break;
	default:
		break;
	}
	for (i = 0; i < ilosc_obrazow; i++)
	{
		usuwanie(zbior_zdjec + i);
	}
	system("pause"); 
	return 0;
}
