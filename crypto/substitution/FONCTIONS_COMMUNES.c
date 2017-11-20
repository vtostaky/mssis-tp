#include "LIBRAIRIES.h"
#include "FONCTIONS_COMMUNES.h"


/***********            Fonction pour déterminer la taille du texte donné en entrée             ************/
void Determination_long_texte(int * LT, char * nom_fichier)
{
  char current_char;
  FILE *fp = fopen(nom_fichier,"r");

    do
    {
        current_char=fgetc(fp);
        if((current_char >= 'a' && current_char <= 'z') ||
          (current_char >= 'A' && current_char <= 'Z'))
        {
            (*LT)++;
        }
    }while(current_char != EOF);
    fclose(fp);
}

/***********            Fonction de Lecture/chargement du texte                ************/
/*********** Attention !!! on ne stocke que les valeurs de l'alphabet comprises entre 0 et 25 inclus ************/

void Lire_et_charger_texte(int * texte, int longueur_texte, char * nom_fichier)
{
    int i = 0;
    char current_char;
    FILE *fp = fopen(nom_fichier,"r");

    fseek(fp, 0, SEEK_SET);

    if(texte == NULL)
    {
        fclose(fp);
        return;
    }

    while(i < longueur_texte)
    {
        current_char = fgetc(fp);
        if(current_char >= 'a' && current_char <= 'z')
        {
            texte[i] = ((int)current_char-97);
            i++;
        }
        else if(current_char >= 'A' && current_char <= 'Z')
        {
            texte[i] = ((int)current_char-65);
            i++;
        }
        if(current_char == EOF)
            break;
    }
    fclose(fp);
}


/******************* Ecriture du Texte Chiffré ***************************/
void Ecrire_chiffre(int * texte_chiffre, int longueur_texte, char * nom_fichier)
{
    int i = 0;
    char current_char;
    FILE *fp = fopen(nom_fichier,"w");

    if(texte_chiffre == NULL)
    {
        fclose(fp);
        return;
    }

    while(i < longueur_texte)
    {
        fputc((char)(texte_chiffre[i]+65),fp);
        i++;
    }
  fclose(fp); 
}



/************Fonction pour lire et charger la clé contenue dans le fichier "fichier_cle" ********************/
void Lire_cle(int cle[26], char * fichier_cle)
{
    Lire_et_charger_texte(cle, 26, fichier_cle);
}

