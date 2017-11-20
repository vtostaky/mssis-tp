#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehicule.h"


char tableau_marques[][20] = {
    "Audi",
    "BMW",
    "Citroen",
    "Ford",
    "Mercedes",
    "Mistubishi",
    "Nissan",
    "Peugeot",
    "Renault",
    "Subaru",
    "Toyota",
    "Volkswagen"
};
#define NB_MARQUES 12

VEHICULE init_vehicule_sans_pointeur()
{
    VEHICULE car;
    int alea;

//    srand(time(NULL));

    car.puissance = (rand()%600) + 100;
    car.vitesse_max = (rand()%220) + 180;
    strncpy(car.marque, tableau_marques[(rand()%NB_MARQUES)], 20*sizeof(char));
    car.next = NULL;
    return car;
}

void affiche_vehicule_sans_pointeur(VEHICULE v)
{
    printf("Marque %s, Puissance %d, Vitesse Max %d\n", v.marque, v.puissance, v.vitesse_max);
}

void init_vehicule_avec_pointeur_1(VEHICULE *v)
{
    *v = init_vehicule_sans_pointeur();
}

VEHICULE *init_vehicule_avec_pointeur_2()
{
    VEHICULE *car = calloc(1, sizeof(VEHICULE));

    init_vehicule_avec_pointeur_1(car);

    return car;
}

void affiche_vehicule_avec_pointeur(VEHICULE *v)
{
    if(v != NULL)
        affiche_vehicule_sans_pointeur(*v);
}

VEHICULE *alloc_tab_veh_dyn(int n)
{
    VEHICULE *tab_veh = calloc(n, sizeof(VEHICULE));
    return tab_veh;
}

void tri_a_bulles(VEHICULE *mesvehicules, int n)
{
    int estTrie = 0;
    VEHICULE temp;
    int i;

    do
    {
        estTrie = 1;
        for(i = 0; i < n-1; i++)
        {
            if(mesvehicules[i].puissance > mesvehicules[i+1].puissance)
            {
                temp = mesvehicules[i];
                mesvehicules[i] = mesvehicules[i+1];
                mesvehicules[i+1] = temp;
                estTrie = 0;
            }
        }
    }while(estTrie == 0);
}
