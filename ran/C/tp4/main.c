#include <stdio.h>
#include "vehicule.h"
#include "liste_vehicules.h"

int main(int argc, char **argv)
{
    VEHICULE car;
    VEHICULE car2;
    VEHICULE *car3;
    VEHICULE *mesvehicules;
    int nb_vehicules = 30;
    int i;
    LISTE *ma_liste;

    car = init_vehicule_sans_pointeur();
    affiche_vehicule_sans_pointeur(car);

    init_vehicule_avec_pointeur_1(&car2);
    affiche_vehicule_sans_pointeur(car2);

    car3 = init_vehicule_avec_pointeur_2();
    affiche_vehicule_avec_pointeur(car3);

    printf("Rentrer le nombre de véhicules : ");
    scanf("%d", &nb_vehicules);
    
    mesvehicules = alloc_tab_veh_dyn(nb_vehicules);

    ma_liste = creation_liste();
    init_liste(ma_liste);

    for(i = 0; i < nb_vehicules; i++)
    {
        init_vehicule_avec_pointeur_1(mesvehicules+i);
        printf("Véhicule à insérer => Marque : %10s | Puissance : %d | Vitesse max : %d\n",
                mesvehicules[i].marque, 
                mesvehicules[i].puissance,
                mesvehicules[i].vitesse_max);
        inserer_vehicule_dans_liste(ma_liste,mesvehicules+i);
        printf("Liste après insertion : (%d vehicules)\n",i+1);
        afficher_liste_vehicules(ma_liste);
    }
#if 0
    for(i = 0; i < nb_vehicules; i++)
    {
        init_vehicule_avec_pointeur_1(mesvehicules+i);
        inserer_vehicule_dans_liste(ma_liste,mesvehicules+i);
        printf("Liste après insertion : (%d vehicules)\n",i+1);
        afficher_liste_vehicules(ma_liste);
    }

    printf("AVANT TRI!!!!\n");
    for(i = 0; i < nb_vehicules; i++)
    {
        affiche_vehicule_avec_pointeur(mesvehicules+i);
    }

    printf("APRES TRI!!!!\n");
    tri_a_bulles(mesvehicules, nb_vehicules);
    for(i = 0; i < nb_vehicules; i++)
    {
        affiche_vehicule_avec_pointeur(mesvehicules+i);
    }
#endif

    return 0;
}
