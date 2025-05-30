#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Asistent {
    int id;
    char* nume;
    float* salariu;
    int varsta;
    char initialaPrenume;
    char* specializare;
    double vechime;
} Asistent;

typedef struct Nod {
    Asistent info;
    struct Nod* next;
    struct Nod* prev;
} Nod;

typedef struct NodSimplu {
    Asistent info;
    struct NodSimplu* next;
} NodSimplu;

Asistent citireAsistentDinFisier(FILE* f) {
    Asistent a;
    char buffer[100];
    a.nume = (char*)malloc(100);
    a.salariu = (float*)malloc(sizeof(float));
    a.specializare = (char*)malloc(100);

    fscanf(f, "%d", &a.id);
    fscanf(f, "%s", a.nume);
    fscanf(f, "%f", a.salariu);
    fscanf(f, "%d", &a.varsta);
    fscanf(f, " %c", &a.initialaPrenume);
    fscanf(f, "%s", a.specializare);
    fscanf(f, "%lf", &a.vechime);

    return a;
}

Nod* inserareInceput(Nod* cap, Asistent a) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = a;
    nou->prev = NULL;
    nou->next = cap;
    if (cap)
        cap->prev = nou;
    return nou;
}

Nod* citireListaDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;
    Nod* cap = NULL;
    while (!feof(f)) {
        Asistent a = citireAsistentDinFisier(f);
        cap = inserareInceput(cap, a);
    }
    fclose(f);
    return cap;
}

void afisareLista(Nod* cap) {
    Nod* p = cap;
    while (p) {
        printf("ID: %d, Nume: %s, Salariu: %.2f, Varsta: %d, Initiala: %c, Specializare: %s, Vechime: %.2lf\n",
            p->info.id, p->info.nume, *(p->info.salariu), p->info.varsta, p->info.initialaPrenume,
            p->info.specializare, p->info.vechime);
        p = p->next;
    }
}

Nod* filtrareSalariu(Nod* cap, float prag) {
    Nod* nouaLista = NULL;
    Nod* p = cap;
    while (p) {
        if (*(p->info.salariu) < prag) {
            Asistent a;
            a.id = p->info.id;
            a.nume = strdup(p->info.nume);
            a.salariu = (float*)malloc(sizeof(float));
            *(a.salariu) = *(p->info.salariu);
            a.varsta = p->info.varsta;
            a.initialaPrenume = p->info.initialaPrenume;
            a.specializare = strdup(p->info.specializare);
            a.vechime = p->info.vechime;
            nouaLista = inserareInceput(nouaLista, a);
        }
        p = p->next;
    }
    return nouaLista;
}

Nod* transformaInCirculara(Nod* cap) {
    if (!cap) return NULL;
    Nod* p = cap;
    while (p->next)
        p = p->next;
    p->next = cap;
    cap->prev = p;
    return cap;
}

void afisareCirculara(Nod* cap) {
    if (!cap) return;
    Nod* p = cap;
    do {
        printf("ID: %d, Nume: %s, Salariu: %.2f, Varsta: %d, Initiala: %c, Specializare: %s, Vechime: %.2lf\n",
            p->info.id, p->info.nume, *(p->info.salariu), p->info.varsta, p->info.initialaPrenume,
            p->info.specializare, p->info.vechime);
        p = p->next;
    } while (p && p != cap);
}

NodSimplu* conversieLaSimpla(Nod* cap) {
    NodSimplu* capS = NULL;
    Nod* p = cap;
    while (p) {
        NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
        nou->info.id = p->info.id;
        nou->info.nume = strdup(p->info.nume);
        nou->info.salariu = (float*)malloc(sizeof(float));
        *(nou->info.salariu) = *(p->info.salariu);
        nou->info.varsta = p->info.varsta;
        nou->info.initialaPrenume = p->info.initialaPrenume;
        nou->info.specializare = strdup(p->info.specializare);
        nou->info.vechime = p->info.vechime;
        nou->next = capS;
        capS = nou;
        p = p->next;
    }
    return capS;
}

void afisareListaSimpla(NodSimplu* cap) {
    NodSimplu* p = cap;
    while (p) {
        printf("ID: %d, Nume: %s, Salariu: %.2f, Varsta: %d, Initiala: %c, Specializare: %s, Vechime: %.2lf\n",
            p->info.id, p->info.nume, *(p->info.salariu), p->info.varsta, p->info.initialaPrenume,
            p->info.specializare, p->info.vechime);
        p = p->next;
    }
}

Nod* adaugaLangaCriteriu(Nod* cap, int varstaCautata, Asistent nouAsistent) {
    if (!cap) return NULL;
    Nod* p = cap;
    do {
        if (p->info.varsta == varstaCautata) {
            Nod* nou = (Nod*)malloc(sizeof(Nod));
            nou->info = nouAsistent;
            nou->next = p->next;
            nou->prev = p;
            p->next->prev = nou;
            p->next = nou;
            break;
        }
        p = p->next;
    } while (p && p != cap);
    return cap;
}

void eliberareLista(Nod* cap) {
    while (cap) {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->info.nume);
        free(temp->info.salariu);
        free(temp->info.specializare);
        free(temp);
    }
}

void eliberareListaSimpla(NodSimplu* cap) {
    while (cap) {
        NodSimplu* temp = cap;
        cap = cap->next;
        free(temp->info.nume);
        free(temp->info.salariu);
        free(temp->info.specializare);
        free(temp);
    }
}

int main() {
    Nod* lista = citireListaDinFisier("asistenti.txt");
    printf("\nLista initiala:\n");
    afisareLista(lista);

    Nod* listaFiltrata = filtrareSalariu(lista, 3500.0);
    printf("\nLista filtrata (salariu < 3500):\n");
    afisareLista(listaFiltrata);

    Nod* listaCirculara = transformaInCirculara(lista);
    printf("\nLista circulara:\n");
    afisareCirculara(listaCirculara);

    NodSimplu* listaSimpla = conversieLaSimpla(lista);
    printf("\nLista simplu inlantuita:\n");
    afisareListaSimpla(listaSimpla);

    Asistent aNou;
    aNou.id = 100;
    aNou.nume = strdup("Adaugat");
    aNou.salariu = (float*)malloc(sizeof(float));
    *(aNou.salariu) = 3700.0f;
    aNou.varsta = 50;
    aNou.initialaPrenume = 'A';
    aNou.specializare = strdup("Cardiologie");
    aNou.vechime = 8.2;

    listaCirculara = adaugaLangaCriteriu(listaCirculara, 45, aNou);
    printf("\nLista circulara dupa adaugare langa varsta=45:\n");
    afisareCirculara(listaCirculara);

    eliberareLista(listaFiltrata);
    eliberareListaSimpla(listaSimpla);

    Nod* p = listaCirculara;
    if (p) {
        p->prev->next = NULL;
        p->prev = NULL;
    }
    eliberareLista(p);

    return 0;
}
