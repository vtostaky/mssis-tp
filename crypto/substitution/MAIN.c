
#include "LIBRAIRIES.h"
#include "FONCTIONS_COMMUNES.h"
#include "CHIFFREMENT.h"
#include "CRYPTANALYSE.h"



int main(int argc, char *argv[])
{
  int longueur_texte;
  
  char NOM_FIC_CLAIR[50];
  char NOM_FIC_CHIFF[50];
  char NOM_FIC_DECHIFF[50];
  char NOM_FIC_CLE[50];
  int * texte_clair;
  int * texte_chiffre;
  int * texte_dechiffre;
  int cle[26];
  char cle_analysee[26];
 
  char choix;
  
 printf("Faites votre choix :\n");
  printf("E : Chiffrement \n");
  printf("D : Déchiffrement (connaissant la clé)\n");
  printf("C : Cryptanalyse (sans connaissance de la clé)\n");
  scanf("%c", &choix);
   
  switch (choix)
  { 
    case 'E':
    {
      printf("Saisir le nom du fichier du texte clair :\n");
      scanf("%49s", NOM_FIC_CLAIR);
      printf("Saisir le nom du fichier du texte chiffré :\n");
      scanf("%49s", NOM_FIC_CHIFF);
      printf("Saisir le nom du fichier contenant la clé :\n");
      scanf("%49s", NOM_FIC_CLE);
      Determination_long_texte(&longueur_texte, NOM_FIC_CLAIR);
  
      printf("Longueur du texte = %d\n", longueur_texte);
  
      /** allocation de la mémoire **/
      texte_clair = malloc(longueur_texte * sizeof(int));
      texte_chiffre = malloc(longueur_texte * sizeof(int));
  
      Lire_et_charger_texte(texte_clair, longueur_texte, NOM_FIC_CLAIR); /** Lecture du texte clair **/
      Lire_cle(cle, NOM_FIC_CLE); /** Lecture de la clef **/
  
      /** Chiffrement **/
      Chiffrer_substitution(cle, texte_clair, texte_chiffre, longueur_texte);
  
      Ecrire_chiffre(texte_chiffre, longueur_texte, NOM_FIC_CHIFF); /** Écriture texte chiffre **/
     
      free(texte_clair);
      free(texte_chiffre);
      break;
    }
    
    case 'D':
    {
      printf("Saisir le nom du fichier du texte chiffré :\n");
      scanf("%49s", NOM_FIC_CHIFF);
      printf("Saisir le nom du fichier pour le texte une fois dechiffré :\n");
      scanf("%49s", NOM_FIC_DECHIFF);
      printf("Saisir le nom du fichier contenant la clé :\n");
      scanf("%49s", NOM_FIC_CLE);
      Determination_long_texte(&longueur_texte, NOM_FIC_CHIFF); //Détermination de la longueur du texte clair
      
      printf("long texte = %d\n", longueur_texte);
     
      texte_chiffre = malloc(longueur_texte * sizeof(int));
      texte_dechiffre = malloc(longueur_texte * sizeof(int));
      
      Lire_et_charger_texte(texte_chiffre, longueur_texte, NOM_FIC_CHIFF);
      Lire_cle(cle, NOM_FIC_CLE);
      
      Dechiffrer_substitution(cle,  texte_chiffre, texte_dechiffre, longueur_texte);
      Ecrire_chiffre(texte_dechiffre, longueur_texte, NOM_FIC_DECHIFF);
      free(texte_chiffre);
      free(texte_dechiffre);
      break;
    }
    case 'C':
    {
      /*ATTENTION : Cryptanalyse par analyse de fréquence */
      printf("\n\nCe programme est un outil qui aide à l'analyse d'un texte chiffré avec une méthode par substitution.\n");
      printf("Ici, les lettres sont substituées par paire, exemple : a est substitué par h, et h est substitué par a, b par m et m par b, etc. \n\n");
      
      printf("Saisir le nom du fichier du texte chiffré :\n");
      scanf("%s", NOM_FIC_CHIFF);
      
      Determination_long_texte(&longueur_texte, NOM_FIC_CHIFF); //Détermination de la longueur du texte clair
      printf("Longueur texte = %d\n", longueur_texte);
  
      texte_chiffre = malloc(longueur_texte * sizeof(int));
  
      Lire_et_charger_texte(texte_chiffre, longueur_texte, NOM_FIC_CHIFF);
      
      printf("\nLe texte à déchiffrer est le suivant :\n");
      printf("-----------------------------------------------------------------------------------------\n");
      Affiche_alphabetique(texte_chiffre,longueur_texte);
      printf("-----------------------------------------------------------------------------------------\n");
      
      
      /************************** ANALYSE ********************************/
      Cryptanalyse(texte_chiffre, longueur_texte, cle_analysee);
      free(texte_chiffre);
      break;
    }
  }
  return 0;
}





