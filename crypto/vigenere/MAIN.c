#include "LIBRAIRIES.h"
#include "FONCTIONS_COMMUNES.h"
#include "CHIFFREMENT_VIGENERE.h"
#include "CRYPTANALYSE_VIGENERE.h"


int main(int argc, char *argv[])
{ 
  int longueur_texte, periode, longueur_clef;
  char NOM_FIC_CLAIR[50];
  char NOM_FIC_CHIFF[50];
  char NOM_FIC_DECHIFF[50];
  char NOM_FIC_CLE[50];
  int * texte_clair;
  int * texte_chiffre;
  int * texte_dechiffre;
  int * clef;
  int * cle_analysee;
  
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
      Determination_long_texte(&longueur_texte, NOM_FIC_CLAIR); //Détermination de la longueur du texte clair
      Determination_long_texte(&longueur_clef, NOM_FIC_CLE);  //Détermination de la longueur de la clé
  
      printf("Longueur du texte = %d\n", longueur_texte);
      printf("Longueur de la clé = %d\n", longueur_clef);
  
      /** allocation de la mémoire **/
      texte_clair = malloc(longueur_texte * sizeof(int));
      texte_chiffre = malloc(longueur_texte * sizeof(int));
      clef = malloc(longueur_clef * sizeof(int));
  
      Lire_et_charger_texte(texte_clair, longueur_texte, NOM_FIC_CLAIR);
      Lire_et_charger_texte(clef, longueur_clef, NOM_FIC_CLE);
      Chiffrement_vigenere(clef, longueur_clef, texte_clair, texte_chiffre, longueur_texte);
  
      /** Écriture texte chiffre **/
      Ecrire_chiffre(texte_chiffre, longueur_texte, NOM_FIC_CHIFF);
      free(texte_clair);
      free(texte_chiffre);
      free(clef);
      break;
    }
    case 'D': /** Déchiffrement **/
    {
      printf("Saisir le nom du fichier du texte chiffré :\n");
      scanf("%49s", NOM_FIC_CHIFF);
      printf("Saisir le nom du fichier pour le texte une fois dechiffré :\n");
      scanf("%49s", NOM_FIC_DECHIFF);
      printf("Saisir le nom du fichier contenant la clé :\n");
      scanf("%49s", NOM_FIC_CLE);
      Determination_long_texte(&longueur_texte, NOM_FIC_CHIFF); //Détermination de la longueur du texte clair
      Determination_long_texte(&longueur_clef, NOM_FIC_CLE);  //Détermination de la longueur de la clé
      printf("Longueur du texte = %d\n", longueur_texte);
      printf("Longueur de la clé = %d\n", longueur_clef);
      texte_chiffre = malloc(longueur_texte * sizeof(int));
      texte_dechiffre = malloc(longueur_texte * sizeof(int));
      clef = malloc(longueur_clef * sizeof(int));
      Lire_et_charger_texte(texte_chiffre, longueur_texte, NOM_FIC_CHIFF);
      Lire_et_charger_texte(clef, longueur_clef, NOM_FIC_CLE);
      
      Dechiffrement_vigenere(clef, longueur_clef,  texte_chiffre, texte_dechiffre, longueur_texte);
      Ecrire_chiffre(texte_dechiffre, longueur_texte, NOM_FIC_DECHIFF);
      free(texte_chiffre);
      free(texte_dechiffre);
      free(clef);
      break;
    }
    case 'C':
    {
      printf("Nom du fichier du texte chiffré :\n");
      scanf("%49s", NOM_FIC_CHIFF);
      Determination_long_texte(&longueur_texte, NOM_FIC_CHIFF); //Détermination de la longueur du texte chiffré
      printf("Longueur du texte = %d\n", longueur_texte);
  
      texte_chiffre = malloc(longueur_texte * sizeof(int));
  
      Lire_et_charger_texte(texte_chiffre, longueur_texte, NOM_FIC_CHIFF); //Chargement du texte chiffré
  
      printf("\n\nCe programme est un outil qui aide à l'analyse d'un texte chiffré via la méthode de Vigenère\n");
      printf("L'analyse va se dérouler en deux temps :\n");
      printf("1ere étape => détermination de la longueur de la clé de chiffrement par analyse de coïncidences\n");
      printf("2eme étape => détermination de la clé par analyse de fréquences des sous textes\n");
      printf("Tapez sur entrée pour continuer :\n\n");
      getchar();

      printf("Le texte à déchiffrer est le suivant :");

      affichage_alphabetique_texte(texte_chiffre, longueur_texte); //Affichage alphabétique du texte chiffré

      printf("\n\nPREMIÈRE ÉTAPE : Détermination des coefficients de la longueur de la clé par analyse de coincidences");
      printf("\n\nPour avoir de l'aide taper help\n");
  
      analyse_coincidence(texte_chiffre, longueur_texte);
  
      printf("Entrez la longueur souhaitée de la clé :");
      scanf("%d",&periode);
  
      /* allocation de memoire pour cle_analysee */
      if( periode > 0) 
      {
        cle_analysee = malloc(sizeof(int) * periode);
      }
      else
      {
        return(EXIT_FAILURE);
      }
  
      printf("\n\nDEUXIÈME ETAPE : Détermination des coefficients de la clé par analyse de fréquence \n");
      printf("\n\nPour avoir de l'aide taper help\n");
      analyse_decalage(texte_chiffre,longueur_texte,periode, cle_analysee);
      free(texte_chiffre);
      free(cle_analysee);
      break;
    }
  }
  
 return 0;
}