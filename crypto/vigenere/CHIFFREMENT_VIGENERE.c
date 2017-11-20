/*******************************************************************************/
/****                                                                        ***/
/****                    CHIFFREMENT DE VIGENERE                             ***/
/****                                                                        ***/
/*******************************************************************************/

#include "LIBRAIRIES.h"
#include "CHIFFREMENT_VIGENERE.h"
#include "FONCTIONS_COMMUNES.h"


/******************** Fonction de Chiffrement Vigenere **********************/
void Chiffrement_vigenere(int * cle, int longueur_cle,  int * texte_clair, int * texte_chiffre, int longueur_texte)
{
    /*
       texte_clair contient le message en clair
       texte_chiffre va contenir le message une fois chiffré
       */

    int i, j, nb_blocks;

    if(longueur_cle > 0)
    {
        nb_blocks = longueur_texte/longueur_cle;

        for(i=0;i<nb_blocks;i++)
        {
            for(j=0;j< longueur_cle ;j++)
            {
                texte_chiffre[i*longueur_cle + j] = (texte_clair[i*longueur_cle + j] + cle[j])%26;
            }
        }


        for(j=0;j< longueur_texte - nb_blocks*longueur_cle ;j++)
        {
            texte_chiffre[nb_blocks*longueur_cle+j] = (texte_clair[nb_blocks*longueur_cle + j] + cle[j])%26;
        }
    }
    else /*gestion de l'erreur de la non positivité de la clé.*/
    {
        printf("\nErreur : la longueur de la clé doit être un entier strictement positif \n"); 
    }
}


/******************** Fonction de Déchiffrement Vigenere **********************/
void Dechiffrement_vigenere( int * cle, int longueur_cle,  int * texte_chiffre, int * texte_dechiffre, int longueur_texte)
{
    /*
       texte_chiffre contient le message chiffré
       texte_dechiffre va contenir le message dechiffré
       */

    int i, j, nb_blocks;

    if( longueur_cle > 0)
    {
        nb_blocks = longueur_texte/longueur_cle;

        for(i=0;i<nb_blocks;i++)
        {
            for(j=0;j< longueur_cle ;j++)
            {
                texte_dechiffre[i*longueur_cle + j] = (texte_chiffre[i*longueur_cle + j] + 26 - cle[j])%26;
            }
        }

        for(j=0;j< longueur_texte - nb_blocks*longueur_cle ;j++)
        {
            texte_dechiffre[nb_blocks*longueur_cle+j] = (texte_chiffre[nb_blocks*longueur_cle + j] + 26 - cle[j])%26;
        }
    }
    else /*gestion de l'erreur de la non positivite de la clef.*/
    {
        printf("\nErreur : la longueur de la clé doit être un entier strictement positif \n"); 
    }
}
