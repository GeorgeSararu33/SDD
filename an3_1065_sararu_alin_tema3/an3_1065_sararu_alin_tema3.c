#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Angajat {
    int id;
    char* nume;
    float* salariu;
    int varsta;
    char* departament;
    char initiala;
    double experienta;
} Angajat;

typedef struct Nod {
    Angajat info;
    struct Nod* next;
} Nod;

typedef struct HashNode {
    Angajat info;
    struct HashNode* next;
} HashNode;

#define DIM_HASH 5

char* copiaza(const char* src) {
    if (!src) return NULL;
    char* dest = (char*)malloc(strlen(src) + 1);
    if (dest) strcpy(dest, src);
    return dest;
}

Angajat citesteAngajat(FILE* f) {
    Angajat a;
    char bufferNume[100], bufferDepartament[100];
    a.salariu = (float*)malloc(sizeof(float));
    fscanf(f, "%d %s %f %d %s %c %lf", &a.id, bufferNume, a.salariu, &a.varsta, bufferDepartament, &a.initiala, &a.experienta);
    a.nume = copiaza(bufferNume);
    a.departament = copiaza(bufferDepartament);
    return a;
}

void enqueue(Nod** prim, Nod** ultim, Angajat a) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = a;
    nou->next = NULL;
    if (*ultim) (*ultim)->next = nou;
    else *prim = nou;
    *ultim = nou;
}

Angajat* ultimCriteriu(Nod* prim, float pragSalariu) {
    Angajat* rezultat = NULL;
    while (prim) {
        if (*(prim->info.salariu) > pragSalariu)
            rezultat = &prim->info;
        prim = prim->next;
    }
    return rezultat;
}

int hashFunction(const char* nume) {
    int sum = 0;
    while (*nume) sum += *(nume++);
    return sum % DIM_HASH;
}

void inserareHash(HashNode** tabela, Angajat a) {
    int poz = hashFunction(a.nume);
    HashNode* nou = (HashNode*)malloc(sizeof(HashNode));
    nou->info = a;
    nou->next = tabela[poz];
    tabela[poz] = nou;
}

void conversieInHash(Nod* coada, HashNode** tabela) {
    while (coada) {
        inserareHash(tabela, coada->info);
        coada = coada->next;
    }
}

Angajat** cautaCheie(HashNode** tabela, const char* cheie, int* nr) {
    int poz = hashFunction(cheie);
    HashNode* p = tabela[poz];
    *nr = 0;
    HashNode* temp = p;
    while (temp) {
        if (strcmp(temp->info.nume, cheie) == 0)
            (*nr)++;
        temp = temp->next;
    }
    if (*nr == 0) return NULL;
    Angajat** vector = (Angajat**)malloc(sizeof(Angajat*) * (*nr));
    int i = 0;
    while (p) {
        if (strcmp(p->info.nume, cheie) == 0)
            vector[i++] = &p->info;
        p = p->next;
    }
    return vector;
}

void afisareCoada(Nod* prim) {
    while (prim) {
        printf("ID: %d, Nume: %s, Salariu: %.2f, Varsta: %d, Dept: %s, Init: %c, Exp: %.2lf\n",
            prim->info.id, prim->info.nume, *(prim->info.salariu), prim->info.varsta,
            prim->info.departament, prim->info.initiala, prim->info.experienta);
        prim = prim->next;
    }
}

void afisareVector(Angajat** v, int n) {
    for (int i = 0; i < n; i++) {
        Angajat* a = v[i];
        printf("VECTOR -> ID: %d, Nume: %s, Salariu: %.2f\n", a->id, a->nume, *(a->salariu));
    }
}

void afisareTabela(HashNode** tabela) {
    for (int i = 0; i < DIM_HASH; i++) {
        HashNode* p = tabela[i];
        printf("[%d]: ", i);
        while (p) {
            printf("%s(%.2f) -> ", p->info.nume, *(p->info.salariu));
            p = p->next;
        }
        printf("NULL\n");
    }
}

void stergeDupaExperienta(HashNode** tabela, double prag) {
    for (int i = 0; i < DIM_HASH; i++) {
        HashNode* p = tabela[i];
        HashNode* prev = NULL;
        while (p) {
            if (p->info.experienta < prag) {
                if (prev) prev->next = p->next;
                else tabela[i] = p->next;
                free(p->info.nume);
                free(p->info.salariu);
                free(p->info.departament);
                HashNode* deSters = p;
                p = p->next;
                free(deSters);
            }
            else {
                prev = p;
                p = p->next;
            }
        }
    }
}

void elibereazaCoada(Nod* prim) {
    while (prim) {
        Nod* temp = prim;
        prim = prim->next;
        free(temp->info.nume);
        free(temp->info.salariu);
        free(temp->info.departament);
        free(temp);
    }
}

void elibereazaTabela(HashNode** tabela) {
    for (int i = 0; i < DIM_HASH; i++) {
        HashNode* p = tabela[i];
        while (p) {
            HashNode* temp = p;
            p = p->next;
            free(temp->info.nume);
            free(temp->info.salariu);
            free(temp->info.departament);
            free(temp);
        }
    }
}

int main() {
    Nod* prim = NULL, * ultim = NULL;
    FILE* f = fopen("angajati.txt", "r");
    while (!feof(f)) {
        Angajat a = citesteAngajat(f);
        enqueue(&prim, &ultim, a);
    }
    fclose(f);

    printf("\n[COADA INIT]\n");
    afisareCoada(prim);

    Angajat* gasit = ultimCriteriu(prim, 4000.0f);
    if (gasit)
        printf("\n[ULTIM CU SALARIU > 4000]: %s, %.2f\n", gasit->nume, *(gasit->salariu));

    HashNode* tabela[DIM_HASH] = { 0 };
    conversieInHash(prim, tabela);
    printf("\n[TABELA DISPERSIE]\n");
    afisareTabela(tabela);

    int nr = 0;
    Angajat** vector = cautaCheie(tabela, "Alin", &nr);
    printf("\n[VECTOR -> CHEIE: 'Alin']\n");
    afisareVector(vector, nr);

    printf("\n[STERGERE din tabela experienta < 5]\n");
    stergeDupaExperienta(tabela, 5.0);
    afisareTabela(tabela);

    free(vector);
    elibereazaCoada(prim);
    elibereazaTabela(tabela);

    return 0;
}
