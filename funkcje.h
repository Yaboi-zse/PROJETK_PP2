#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED
#include <wchar.h>
#define MAX_NAME_LENGTH 256
// Struktura reprezentująca pojedynczy plik
typedef struct {
    char name[MAX_NAME_LENGTH];
    int size;
    // Dodaj dodatkowe atrybuty pliku, jeśli to konieczne
} File;

// Struktura reprezentująca katalog
typedef struct Node {
    File file;
    struct Node* next;
} Node;
void displayMenu() {
    printf("\nMenedzer Plikow - Menu:\n");
    printf("1. Wyswietl pliki w katalogu\n");
    printf("2. Dodaj nowy plik\n");
    printf("3. Usun plik\n");
    printf("4. Zmiana nazwy pliku\n");
    printf("5. Zmiana atrybutow pliku\n");
    printf("6. Przeniesienie pliku\n");
    printf("7. Kopiowanie pliku\n");
    printf("8. Wyszukiwanie pliku\n");
    printf("0. Wyjscie\n");
    printf("Wybierz opcje: ");
}
void addFile(const char* filename, const char* name, int size) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Błąd przy otwieraniu pliku.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %d\n", name, size);
    fclose(file);
    printf("Plik %s dodany.\n", name);
}

void displayFiles(Node* head) {
    printf("\nLista plikow w katalogu:\n");
    while (head != NULL) {
        printf("Nazwa: %s, Rozmiar: %d\n", head->file.name, head->file.size);
        head = head->next;
    }
}
void deleteFile(Node** head, const char* name) {
    Node* current = *head;
    Node* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->file.name, name) == 0) {
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Plik %s usuniety.\n", name);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Plik %s nie zostal znaleziony.\n", name);
}
void renameFile(Node* head, const char* oldName, const char* newName) {
    while (head != NULL) {
        if (strcmp(head->file.name, oldName) == 0) {
            strncpy(head->file.name, newName, MAX_NAME_LENGTH - 1);
            printf("Nazwa pliku zmieniona z %s na %s.\n", oldName, newName);
            return;
        }
        head = head->next;
    }

    printf("Plik %s nie zostal znaleziony.\n", oldName);
}

#endif // FUNKCJE_H_INCLUDED
