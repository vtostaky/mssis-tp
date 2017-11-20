#ifndef VEHICULE_H
#define VEHICULE_H

struct voiture
{
    char marque[20];
    int puissance;
    int vitesse_max;
    struct voiture *next;
};

typedef struct voiture VEHICULE;


VEHICULE init_vehicule_sans_pointeur();
void affiche_vehicule_sans_pointeur();
void init_vehicule_avec_pointeur_1(VEHICULE *V);
VEHICULE *init_vehicule_avec_pointeur_2();
void affiche_vehicule_avec_pointeur(VEHICULE *V);

VEHICULE *alloc_tab_veh_dyn(int n);
void tri_a_bulles(VEHICULE *mesvehicules, int n);

#endif
