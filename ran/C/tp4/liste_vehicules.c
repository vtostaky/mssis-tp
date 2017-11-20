#include <stdio.h>
#include <stdlib.h>
#include "liste_vehicules.h"


LISTE *creation_liste()
{
    LISTE *l = calloc(1, sizeof(LISTE));
    return l;
}

void init_liste(LISTE *l)
{
    l->taille = 0;
    l->debut = NULL;
    l->fin = NULL;
}

void inserer_vehicule_dans_liste(LISTE *l, VEHICULE *v)
{
    VEHICULE *courant;
    VEHICULE *precedent;
    VEHICULE *temp;

    if(l->taille == 0)
    {
        l->debut = v;
        l->fin = v;
    }
    else if(l->taille == 1 && (v->puissance > l->fin->puissance))
    {
        l->debut->next = v;
        l->fin = v;
    }
    else if(v->puissance < l->debut->puissance)
    {
        v->next = l->debut;
        l->debut = v;
    }
    else if(v->puissance > l->fin->puissance)
    {
        courant = l->fin;
        courant->next = v;
        l->fin = v;
    }
    else
    {
        courant = l->debut->next;
        precedent = l->debut;
        while(courant != NULL && precedent != NULL)
        {
            if(v->puissance <= courant->puissance && v->puissance >= precedent->puissance)
            {
                temp = courant;
                courant = v;
                courant->next = temp;
                precedent->next = v;
                break;
            }
            else
            {
                precedent = courant;
                courant = courant->next;
            }
        }
    }
    l->taille++;
}

void afficher_liste_vehicules(LISTE* l)
{
    VEHICULE *courant = l->debut;
    int i = 1;
    while(courant != NULL)
    {
        printf("N° %2d => Marque : %10s | Puissance : %d | Vitesse max : %d\n", i++, courant->marque, courant->puissance, courant->vitesse_max);
        courant = courant->next;
    }
}
