#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InformatiaUtila {
    char nume[50];
    int varsta;
    float salariu;
    char* adresa;
    int* cod;
    struct InformatiaUtila* urmator;
} InformatiaUtila;

InformatiaUtila* creareNod(const char* nume, int varsta, float salariu, const char* adresa, int cod) {
    InformatiaUtila* nod = (InformatiaUtila*)malloc(sizeof(InformatiaUtila));
    strcpy(nod->nume, nume);
    nod->varsta = varsta;
    nod->salariu = salariu;
    nod->adresa = strdup(adresa);
    nod->cod = (int*)malloc(sizeof(int));
    *(nod->cod) = cod;
    nod->urmator = NULL;
    return nod;
}

void inserareInceput(InformatiaUtila** cap, InformatiaUtila* nodNou) {
    nodNou->urmator = *cap;
    *cap = nodNou;
}

void stergereSfarsit(InformatiaUtila** cap) {
    if (*cap == NULL) return;
    if ((*cap)->urmator == NULL) {
        free((*cap)->adresa);
        free((*cap)->cod);
        free(*cap);
        *cap = NULL;
        return;
    }
    InformatiaUtila* temp = *cap;
    while (temp->urmator->urmator != NULL) {
        temp = temp->urmator;
    }
    free(temp->urmator->adresa);
    free(temp->urmator->cod);
    free(temp->urmator);
    temp->urmator = NULL;
}

int cautareElement(InformatiaUtila* cap, const char* nume) {
    int pozitie = 0;
    while (cap) {
        if (strcmp(cap->nume, nume) == 0) {
            printf("Element găsit la poziția %d: %s, %d ani, %.2f lei, %s, cod %d\n", pozitie, cap->nume, cap->varsta, cap->salariu, cap->adresa, *(cap->cod));
            return pozitie;
        }
        cap = cap->urmator;
        pozitie++;
    }
    printf("Elementul nu a fost găsit.\n");
    return -1;
}

void afisareLista(InformatiaUtila* cap) {
    while (cap) {
        printf("%s, %d ani, %.2f lei, %s, cod %d\n", cap->nume, cap->varsta, cap->salariu, cap->adresa, *(cap->cod));
        cap = cap->urmator;
    }
}

InformatiaUtila** listaToVector(InformatiaUtila* cap, int* dim) {
    *dim = 0;
    InformatiaUtila* temp = cap;
    while (temp) {
        (*dim)++;
        temp = temp->urmator;
    }
    InformatiaUtila** vector = (InformatiaUtila**)malloc((*dim) * sizeof(InformatiaUtila*));
    temp = cap;
    for (int i = 0; i < *dim; i++) {
        vector[i] = temp;
        temp = temp->urmator;
    }
    return vector;
}

void citireFisier(const char* numeFisier, InformatiaUtila** cap) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fișierului!\n");
        return;
    }
    char nume[50], adresa[100];
    int varsta, cod;
    float salariu;
    while (fscanf(f, "%s %d %f %s %d", nume, &varsta, &salariu, adresa, &cod) == 5) {
        inserareInceput(cap, creareNod(nume, varsta, salariu, adresa, cod));
    }
    fclose(f);
}

void eliberareLista(InformatiaUtila** cap) {
    while (*cap) {
        InformatiaUtila* temp = *cap;
        *cap = (*cap)->urmator;
        free(temp->adresa);
        free(temp->cod);
        free(temp);
    }
}

int main() {
    InformatiaUtila* lista = NULL;
    citireFisier("date.txt", &lista);
    afisareLista(lista);
    inserareInceput(&lista, creareNod("Sararu", 25, 3000.50, "Bucuresti", 123));
    printf("Lista dupa inserare:\n");
    afisareLista(lista);
    stergereSfarsit(&lista);
    printf("Lista dupa stergere ultim element:\n");
    afisareLista(lista);
    cautareElement(lista, "Sararu");
    int dim;
    InformatiaUtila** vector = listaToVector(lista, &dim);
    printf("Lista convertita in vector:\n");
    for (int i = 0; i < dim; i++) {
        printf("%s\n", vector[i]->nume);
    }
    free(vector);
    eliberareLista(&lista);
    return 0;
}
