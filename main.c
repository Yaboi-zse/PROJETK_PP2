#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_NAZWA_PLIKU 100
void listujPliki(const char *sciezka) {
    DIR *katalog;
    struct dirent *plik;

    katalog = opendir(sciezka);
    if (katalog) {
        printf("Lista plikow w katalogu '%s':\n", sciezka);
        while ((plik = readdir(katalog)) != NULL) {
            if (plik->d_name[0] != '.') { // Pominięcie ukrytych plików
                printf("%s\n", plik->d_name);
            }
        }
        closedir(katalog);
    }
}

void wejdzDoFolderu(const char *sciezka) {
    if (chdir(sciezka) == 0) {
        printf("Wchodzenie do katalogu '%s' udane.\n", sciezka);
    } else {
        printf("Nie udalo sie wejsc do katalogu '%s'.\n", sciezka);
    }
}

void przegladajFolder(const char *sciezka) {
    DIR *katalog;
    struct dirent *plik;

    katalog = opendir(sciezka);
    if (katalog) {
        printf("Przegladanie katalogu '%s':\n", sciezka);
        while ((plik = readdir(katalog)) != NULL) {
            printf("%s\n", plik->d_name);
        }
        closedir(katalog);
    } else {
        printf("Nie udalo sie przegladac katalogu '%s'.\n", sciezka);
    }
}


void utworzPlik(const char *nazwaPliku) {
    FILE *plik = fopen(nazwaPliku, "w");
    if (plik == NULL) {
        printf("Nie udalo sie utworzyc pliku.\n");
    } else {
        printf("Utworzono plik %s.\n", nazwaPliku);
        fclose(plik);
    }
}

void usunPlik(const char *nazwaPliku) {
    if (remove(nazwaPliku) == 0) {
        printf("Usunieto plik %s.\n", nazwaPliku);
    } else {
        printf("Nie udalo sie usunac pliku %s.\n", nazwaPliku);
    }
}

void zmienNazwePliku(const char *staraNazwa, const char *nowaNazwa) {
    if (rename(staraNazwa, nowaNazwa) == 0) {
        printf("Zmieniono nazwe pliku z %s na %s.\n", staraNazwa, nowaNazwa);
    } else {
        printf("Nie udalo sie zmienic nazwy pliku.\n");
    }
}

void skopiujPlik(const char *zrodlo, const char *cel) {
    FILE *plikZrodlo = fopen(zrodlo, "r");
    FILE *plikCel = fopen(cel, "w");
    if (plikZrodlo == NULL || plikCel == NULL) {
        printf("Nie udalo sie skopiowac pliku.\n");
        return;
    }
    char bufor[1024];
    size_t bajty;
    while ((bajty = fread(bufor, 1, sizeof(bufor), plikZrodlo)) > 0) {
        fwrite(bufor, 1, bajty, plikCel);
    }
    fclose(plikZrodlo);
    fclose(plikCel);
    printf("Skopiowano plik %s do %s.\n", zrodlo, cel);
}

void szukajPlikow(const char *sciezka, const char *nazwaPliku) {
    DIR *katalog;
    struct dirent *plik;
    katalog = opendir(sciezka);
    if (katalog) {
        printf("Szukanie pliku '%s' w katalogu '%s':\n", nazwaPliku, sciezka);
        while ((plik = readdir(katalog)) != NULL) {
            if (strcmp(plik->d_name, nazwaPliku) == 0) {
                printf("Znaleziono plik %s w katalogu %s.\n", nazwaPliku, sciezka);
                closedir(katalog);
                return;
            }
        }
        printf("Nie znaleziono pliku %s w katalogu %s.\n", nazwaPliku, sciezka);
        closedir(katalog);
    }
}

int main() {
    int wybor;
    char nazwaPliku[MAX_NAZWA_PLIKU];
    char nowaNazwa[MAX_NAZWA_PLIKU];
    char zrodlo[MAX_NAZWA_PLIKU];
    char cel[MAX_NAZWA_PLIKU];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Lista plikow w katalogu\n");
        printf("2. Utworz nowy plik\n");
        printf("3. Usun plik\n");
        printf("4. Zmien nazwe pliku\n");
        printf("5. Skopiuj plik\n");
        printf("6. Szukaj pliku\n");
        printf("7. Wejdz do folderu\n");
        printf("8. Przegladaj folder\n");
        printf("9. Wyjscie\n");
        printf("Wybierz opcje: ");
        scanf("%d", &wybor);

         switch (wybor) {
            case 1:
                printf("Podaj sciezke do katalogu: ");
                scanf("%s", nazwaPliku);
                listujPliki(nazwaPliku);
                break;
            case 2:
                printf("Podaj nazwe nowego pliku: ");
                scanf("%s", nazwaPliku);
                utworzPlik(nazwaPliku);
                break;
            case 3:
                printf("Podaj nazwe pliku do usuniecia: ");
                scanf("%s", nazwaPliku);
                usunPlik(nazwaPliku);
                break;
            case 4:
                printf("Podaj aktualna nazwe pliku: ");
                scanf("%s", nazwaPliku);
                printf("Podaj nowa nazwe pliku: ");
                scanf("%s", nowaNazwa);
                zmienNazwePliku(nazwaPliku, nowaNazwa);
                break;
            case 5:
                printf("Podaj nazwe pliku zrodlowego: ");
                scanf("%s", zrodlo);
                printf("Podaj nazwe pliku docelowego: ");
                scanf("%s", cel);
                skopiujPlik(zrodlo, cel);
                break;
            case 6:
                printf("Podaj nazwe pliku do wyszukania: ");
                scanf("%s", nazwaPliku);
                printf("Podaj sciezke do katalogu do przeszukania: ");
                scanf("%s", zrodlo);
                szukajPlikow(zrodlo, nazwaPliku);
                break;
            case 7:
                printf("Podaj sciezke do folderu: ");
                scanf("%s", nazwaPliku);
                wejdzDoFolderu(nazwaPliku);
                break;
            case 8:
                printf("Podaj sciezke do folderu: ");
                scanf("%s", nazwaPliku);
                przegladajFolder(nazwaPliku);
                break;
            case 9:
                printf("Wyjscie z programu.\n");
                exit(0);
            default:
                printf("Nieprawidlowy wybor. Sprobuj ponownie.\n");
        }
    }

    return 0;
}
