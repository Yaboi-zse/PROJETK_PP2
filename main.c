#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
    #define OPEN_COMMAND "start"
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
    #if __APPLE__
        #define OPEN_COMMAND "open"
    #else
        #define OPEN_COMMAND "xdg-open"
    #endif
#endif

#define MAX_NAZWA_PLIKU 100
#define MAX_WYNIKI 1000

char aktualnyFolder[MAX_NAZWA_PLIKU] = ".";
char znalezionePliki[MAX_WYNIKI][MAX_NAZWA_PLIKU];
int liczbaZnalezionychPlikow = 0;

void listujPliki() {
    DIR *katalog;
    struct dirent *plik;

    katalog = opendir(aktualnyFolder);
    if (katalog) {
        printf("Lista plikow w katalogu '%s':\n", aktualnyFolder);
        while ((plik = readdir(katalog)) != NULL) {
            if (plik->d_name[0] != '.') { // Pominiêcie ukrytych plików
                printf("%s\n", plik->d_name);
            }
        }
        closedir(katalog);
    }
}

void wejdzDoFolderu(const char *sciezka) {
    if (chdir(sciezka) == 0) {
        printf("Wchodzenie do katalogu '%s' udane.\n", sciezka);
        strcpy(aktualnyFolder, sciezka);
    } else {
        printf("Nie udalo sie wejsc do katalogu '%s'.\n", sciezka);
    }
}

void przegladajAktulanyFolder() {
    system(CLEAR_SCREEN);
    DIR *katalog;
    struct dirent *plik;
    struct stat st;

    katalog = opendir(aktualnyFolder);
    if (katalog) {
        printf("Przegladanie katalogu '%s':\n", aktualnyFolder);
        while ((plik = readdir(katalog)) != NULL) {
            char pelnaSciezka[MAX_NAZWA_PLIKU];
            snprintf(pelnaSciezka, sizeof(pelnaSciezka), "%s/%s", aktualnyFolder, plik->d_name);
            if (stat(pelnaSciezka, &st) == 0) {
                printf("%s\t%.3f KB\n", plik->d_name, (float)st.st_size / 1024);
            } else {
                printf("%s\tNie udalo sie odczytac rozmiaru pliku.\n", plik->d_name);
            }
        }
        closedir(katalog);
    } else {
        printf("Nie udalo sie przegladac katalogu '%s'.\n", aktualnyFolder);
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




void wyswietlZnalezionePliki() {
    if (liczbaZnalezionychPlikow > 0) {
        printf("Znalezione pliki:\n");
        for (int i = 0; i < liczbaZnalezionychPlikow; i++) {
            printf("%s\n", znalezionePliki[i]);
        }
    } else {
        printf("Nie znaleziono zadnych plikow.\n");
    }
}

void otworzPlik(const char *nazwaPliku) {
    #ifdef _WIN32
        ShellExecute(NULL, "open", nazwaPliku, NULL, NULL, SW_SHOWNORMAL);
    #else
        char komenda[MAX_NAZWA_PLIKU + 50];
        snprintf(komenda, sizeof(komenda), "%s \"%s\"", OPEN_COMMAND, nazwaPliku);
        if (system(komenda) == -1) {
            printf("Nie udalo sie otworzyc pliku %s.\n", nazwaPliku);
        }
    #endif
}


void szukajOtworzFolder(const char *sciezka) {
    strcpy(aktualnyFolder, sciezka);


    liczbaZnalezionychPlikow = 0;
    memset(znalezionePliki, 0, sizeof(znalezionePliki));

    DIR *katalog;
    struct dirent *plik;

    katalog = opendir(sciezka);
    if (katalog) {
        printf("Szukanie plikow w folderze '%s'...\n", sciezka);
        while ((plik = readdir(katalog)) != NULL) {
            if (plik->d_name[0] != '.') {
                strcpy(znalezionePliki[liczbaZnalezionychPlikow], plik->d_name);
                liczbaZnalezionychPlikow++;
            }
        }
        closedir(katalog);

        if (liczbaZnalezionychPlikow > 0) {
            printf("Znaleziono pliki:\n");
            for (int i = 0; i < liczbaZnalezionychPlikow; i++) {
                printf("%s\n", znalezionePliki[i]);
            }
        } else {
            printf("Nie znaleziono zadnych plikow.\n");
        }
    } else {
        printf("Nie udalo sie otworzyc folderu '%s'.\n", sciezka);
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
        printf("1. Lista plikow w aktualnym katalogu\n");
        printf("2. Utworz nowy plik\n");
        printf("3. Usun plik\n");
        printf("4. Zmien nazwe pliku\n");
        printf("5. Skopiuj plik\n");
 printf("6. Szukaj folderu\n");
        printf("7. Wejdz do folderu\n");
        printf("8. Przegladaj folder\n");
        printf("9. Otworz plik\n");

        printf("10. Wyjscie\n");
        printf("Wybierz opcje: ");
        scanf("%d", &wybor);

        switch (wybor) {
            case 1:
                listujPliki();
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
                printf("Podaj nazwe folderu do sprawdzenia i otwarcia: ");
                scanf("%s", nazwaPliku);
                szukajOtworzFolder(nazwaPliku);
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
                printf("Podaj nazwe pliku do otwarcia: ");
                scanf("%s", nazwaPliku);
                otworzPlik(nazwaPliku);
                break;

            case 10:
                printf("Wyjscie z programu.\n");
                exit(0);
            default:
                printf("Nieprawidlowy wybor. Sprobuj ponownie.\n");
        }

        przegladajAktulanyFolder();
    }

    return 0;
}
