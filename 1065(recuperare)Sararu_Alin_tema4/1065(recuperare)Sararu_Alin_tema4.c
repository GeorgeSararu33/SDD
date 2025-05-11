#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int id;
    char* titlu;
    char* limbajProgramare;
    int durataZile;
    float buget;
    int* nivelDificultate;
    char* tehnologiiExtra;
} ProiectSoftware;


typedef struct NodArb {
    ProiectSoftware info;
    struct NodArb* st;
    struct NodArb* dr;
} NodArb;


typedef struct NodCoada {
    ProiectSoftware info;
    struct NodCoada* next;
} NodCoada;

ProiectSoftware creareProiect(int id, const char* titlu, const char* limbaj, int durata, float buget, int dificultate, const char* tehnologii) {
    ProiectSoftware p;
    p.id = id;
    p.titlu = strdup(titlu);
    p.limbajProgramare = strdup(limbaj);
    p.durataZile = durata;
    p.buget = buget;
    p.nivelDificultate = (int*)malloc(sizeof(int));
    *(p.nivelDificultate) = dificultate;
    p.tehnologiiExtra = strdup(tehnologii);
    return p;
}

void afisareProiect(ProiectSoftware p) {
    printf("ID: %d | Titlu: %s | Limbaj: %s | Durata: %d zile | Buget: %.2f | Dificultate: %d | Tech: %s\n",
        p.id, p.titlu, p.limbajProgramare, p.durataZile, p.buget, *(p.nivelDificultate), p.tehnologiiExtra);
}

NodArb* inserareBST(NodArb* rad, ProiectSoftware p) {
    if (rad == NULL) {
        NodArb* nou = (NodArb*)malloc(sizeof(NodArb));
        nou->info = p;
        nou->st = nou->dr = NULL;
        return nou;
    }
    if (p.id < rad->info.id)
        rad->st = inserareBST(rad->st, p);
    else
        rad->dr = inserareBST(rad->dr, p);
    return rad;
}

void citireDinFisier(const char* numeFisier, NodArb** rad) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return;

    int id, durata, dificultate;
    float buget;
    char buffer[256], titlu[50], limbaj[50], tech[50];

    while (fgets(buffer, 256, f)) {
        sscanf(buffer, "%d|%[,]|%[,]|%d,%f,%d,%[\n]", &id, titlu, limbaj, &durata, &buget, &dificultate, tech);
        ProiectSoftware p = creareProiect(id, titlu, limbaj, durata, buget, dificultate, tech);
        *rad = inserareBST(*rad, p);
    }
    fclose(f);
}

int numarFrunzeMaiMari(NodArb* rad, int valoare) {
    if (!rad) return 0;
    if (!rad->st && !rad->dr && rad->info.id > valoare)
        return 1;
    return numarFrunzeMaiMari(rad->st, valoare) + numarFrunzeMaiMari(rad->dr, valoare);
}

void inserareCoadaPrioritati(NodCoada** cap, ProiectSoftware p) {
    NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
    nou->info = p;
    nou->next = NULL;

    if (!*cap || p.buget > (*cap)->info.buget) {
        nou->next = *cap;
        *cap = nou;
    }
    else {
        NodCoada* temp = *cap;
        while (temp->next && temp->next->info.buget > p.buget)
            temp = temp->next;
        nou->next = temp->next;
        temp->next = nou;
    }
}

void arboreInCoada(NodArb* rad, NodCoada** cap) {
    if (!rad) return;
    inserareCoadaPrioritati(cap, rad->info);
    arboreInCoada(rad->st, cap);
    arboreInCoada(rad->dr, cap);
}

void afisareCoada(NodCoada* cap) {
    while (cap) {
        afisareProiect(cap->info);
        cap = cap->next;
    }
}

void traversareBST(NodArb* rad) {
    if (!rad) return;
    traversareBST(rad->st);
    afisareProiect(rad->info);
    traversareBST(rad->dr);
}

void eliberareProiect(ProiectSoftware p) {
    free(p.titlu);
    free(p.limbajProgramare);
    free(p.tehnologiiExtra);
    free(p.nivelDificultate);
}

void eliberareBST(NodArb* rad) {
    if (!rad) return;
    eliberareBST(rad->st);
    eliberareBST(rad->dr);
    eliberareProiect(rad->info);
    free(rad);
}

void eliberareCoada(NodCoada** cap) {
    while (*cap) {
        NodCoada* temp = *cap;
        *cap = (*cap)->next;
        eliberareProiect(temp->info);
        free(temp);
    }
}

int main() {
    NodArb* rad = NULL;
    citireDinFisier("proiecte.txt", &rad);

    printf("\n--- Traversare BST ---\n");
    traversareBST(rad);

    int val = 2;
    printf("\nNumar frunze cu id > %d: %d\n", val, numarFrunzeMaiMari(rad, val));

    NodCoada* coada = NULL;
    arboreInCoada(rad, &coada);

    printf("\n--- Coada de prioritati (dupa buget) ---\n");
    afisareCoada(coada);

    eliberareBST(rad);
    eliberareCoada(&coada);
    return 0;
}
