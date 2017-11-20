#ifndef LISTE_VEHICULES_H
#define LISTE_VEHICULES_H

#include "vehicule.h"

typedef struct _liste LISTE;
struct _liste
{
    int taille;
    VEHICULE *debut;
    VEHICULE *fin;
};

LISTE *creation_liste();
void init_list(LISTE *l);
void inserer_vehicule_dans_liste(LISTE *l, VEHICULE *v);
void afficher_liste_vehicules(LISTE* l);

#endif
