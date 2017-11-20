
#include "LIBRAIRIES.h"
#include "CRYPTANALYSE.h"


/* Fonction qui compte le nombre de "lettre" dans un texte ets qui le renvoi en retour*/
int Compter_lettre(int * texte, int longueur_texte, int lettre)
{
    int i = 0; 
    int count = 0;
    while(i < longueur_texte)
    {
        if(texte[i] == lettre)
        {
            count++;
        }
        i++;
    }
    return count;
}

/* Fonction qui compte et affiche le nombre d'apparition de chaque lettre dans un texte */
void Comptage_nb_apparition(int * texte, int longueur_texte)
{
  int i,nb_lettre;
  
  for(i=0;i<26;i++)
  {
    nb_lettre = Compter_lettre(texte, longueur_texte, i);
    printf("\n Nbre apparition de '%c' =  %d ",(char)('a'+i), nb_lettre);
  }
  printf("\n");
}


/* Fonction qui calcule la fréquence d'apparition des lettres dans un texte donné */
void Comptage_frequence_apparition(int * texte, int longueur_texte)
{
  int i,nb_lettre;
  for(i=0;i<26;i++)
  {
    nb_lettre=Compter_lettre(texte, longueur_texte, i);
    printf("\n Frequence de '%c' =  %.2f ",(char)('a'+i),((float) nb_lettre)/((float) longueur_texte));

  }
  printf("\n");
}


/* Fonction qui calcule le nombre des bigrammes ou couples 'lettre1lettre2' dans un texte donné en entrée*/
int Compter_bigramme(int * texte, int longueur_texte, int lettre1, int lettre2)
{
    int i = 0; 
    int count = 0;
    while(i < longueur_texte-1)
    {
        if(texte[i] == lettre1 && texte[i+1] == lettre2)
        {
            count++;
        }
        i++;
    }
    return count;
}


/* Fonction qui affiche sur la sortie standard le texte clair associé au texte chiffré donné en entrée */
void Affiche_clair(int * texte, int longueur_texte, char * clef_analysee)
{
    int i,j;
    int correspondance_found;
    for(i = 0; i < longueur_texte; i++)
    {
        correspondance_found = 0;
        for(j = 0; j < 26; j++)
        {
            if(clef_analysee[j] != '*' && (int)(clef_analysee[j])-97 == texte[i])
            {
                correspondance_found = 1;
                printf("%c",j+97);
                break;
            }
        }
        if(correspondance_found == 0)
            printf("*");
    }
}

/* Fonction qui affiche un texte tel quel (alphabetique) sur la sortie standard */
void Affiche_alphabetique(int * texte, int longueur_texte)
{
  int i;
  printf("\n");
  for(i=0;i<longueur_texte;i++)
  {
    printf("%c",(char)(texte[i]+((int) 'a')));
  }
  printf("\n");
}


/* Fonction qui affiche en superposant le texte clair avec le texte chiffré */
void Affiche_clair_chiffre(int * texte, int longueur_texte, int longueur, char * clef_analysee)
{
  int i,j,longueur_ligne,nb_ligne, reste, fin;
  longueur_ligne=100;
  fin = longueur_ligne;

  if ((longueur > 0) && (longueur < longueur_ligne))
  {
    nb_ligne = 1; reste = longueur;
  }
  else if (longueur == longueur_ligne)
  {
    nb_ligne = 1; reste = 0;
  }
  else if(longueur > longueur_texte)
  {
      nb_ligne = longueur_texte/longueur_ligne;
      nb_ligne++;
      reste = longueur_texte - (longueur_ligne*(nb_ligne-1));
  }
  else if( longueur <= longueur_texte )
  {
      nb_ligne = longueur/longueur_ligne;
      nb_ligne++;
      reste = longueur - (longueur_ligne*(nb_ligne-1));
  }
 
  for(i=0;i<nb_ligne;i++)
  {
      if (i==(nb_ligne-1) && reste != 0)
      {
	fin = reste;
      }
      /* affichage de la ligne claire */
      for(j=0;j<fin;j++)
      {	  
	  printf("%c",clef_analysee[texte[i*longueur_ligne+j]]);
      }
      printf("\n");

      /* affichage de la ligne chiffre */
      for(j=0;j<fin;j++)
      {	  
	  printf("%c",(char)(texte[i*longueur_ligne+j]+((int) 'A')));
      }
      printf("\n\n");
  }
}


/* Fonction de cryptanalyse */
void Cryptanalyse(int * texte_chiffre, int longueur_texte, char * clef)
{
  int i,j,longueur;
  int * Nbres_bigrammes = NULL;
  double frequence_lettre[26];
  char commande[100],lettre1,lettre2,lettre;
  char clef_analysee[26];
  char lettre_vers_lettre[10];

  Nbres_bigrammes= malloc(26*26*sizeof(int)); //exples: aa, ef, gg, dk, em, xp, etc.

  
    for(i=0;i<26;i++)
    {
      for(j=0;j<26;j++)
      {
	Nbres_bigrammes[i+26*j] = Compter_bigramme(texte_chiffre, longueur_texte,i,j); /* Calcul de la fréquence des bigrammes */
      }
    }

    for(i=0;i<26;i++)
    {
      frequence_lettre[i] = ((double) Compter_lettre(texte_chiffre,longueur_texte ,i) )/((double) longueur_texte); /* Calcul de la fréquence des lettres */
    }


  /* On initialise la clé par des '*', cela indique que la clé est vide */
  for(i=0;i<26;i++)
  {
     clef_analysee[i]='*';
  }


  printf("\n\nDébut de la Cryptanalyse : \nEn tapant help vous obtiendrez les descriptions des commandes permettant d'analyser les frequences des lettres, et des bigrammes, et de remplacer chaque lettre du chiffre par sa lettre en clair correspondante\n");
  // printf("Pour avoir de l'aide taper help");

  /* Boucle de decryptage par analyse de frequence */
  while(strcmp(commande,"exit") != 0)
  {
      /* Entree de la commande */
      printf("\nTapez une commande :");
      scanf("%s",commande);
 
      
      /*Analyse et traitement de la commande*/

      /* demande d'aide */
      if(strcmp(commande,"help") == 0)
      {
	 printf("\nListe de commmandes disponibles :\n");
	 printf("exit                       => quitter le programme\n"); 
	 printf("help                       => affiche la liste des commandes disponibles\n"); 
	 printf("affecter lettre1lettre2    => lettre1 sera substituée par lettre2 (dans la clé)\n"); 
	 printf("desaffecter lettre         => on enleve l'affectation associée à lettre dans la clé\n"); 
	 printf("clair                      => affiche le message partiellement déchiffré\n"); 
	 printf("chiffre                    => affiche le message chiffré\n"); 
	 printf("clairchiffre longueur      => affiche les longueur-premieres lettres du message clair et en parallele le message chiffré \n");
	 printf("cle                        => affiche la clé partiellement retrouvée \n");
	 printf("bigramme                   => affiche les fréquences des biagrammes \n");
	 printf("monogramme                 => affiche les fréquences des monogrammes\n");	  
      }
      else if(strcmp(commande,"exit") == 0) /*  fin d'analyse */
      {
	  printf("\n");
	  break;
      }
      else if(strcmp(commande,"clair") == 0) /* affiche clair */
      {
	  printf("\n");
	  Affiche_clair(texte_chiffre, longueur_texte, clef_analysee);
      }
      else if(strcmp(commande,"chiffre") == 0) /* affiche chiffre */
      {
	  Affiche_alphabetique(texte_chiffre,longueur_texte);
      }
      else if(strcmp(commande,"clairchiffre") == 0) /* affiche clair chiffre */
      {
	  printf("Donner la longueur à afficher : \n");
	  scanf("%d",&longueur);
	  Affiche_clair_chiffre(texte_chiffre,longueur_texte, longueur, clef_analysee);
      }
      else if(strcmp(commande,"cle") == 0) /* affichage de la clef */
      {
	  printf("\n");
	  for(i=0;i<26;i++)
	  {
	      printf("%c  ",'a'+ ((char)i));
	  }
	  printf("\n");
	  for(i=0;i<26;i++)
	  {
	      printf("%c  ",clef_analysee[i]);
	  }
	  printf("\n\n");
      }
      else if(strcmp(commande,"desaffecter") == 0) /* permet de corriger la correspondance dans la clé, en cas d'erreurs de saisies */
      {
	  scanf("%s",lettre_vers_lettre);
	  lettre=lettre_vers_lettre[0];
	  if((lettre >= 'a') && (lettre <= 'z'))
	  {
	      clef_analysee[(int)(lettre-'a')] = '*';
	  }
      }
      else if(strcmp(commande,"affecter") == 0) /* affectation des éléments pour la clé*/
      {
	  scanf("%s",lettre_vers_lettre);
	  lettre1 =  lettre_vers_lettre[0];
	  lettre2=lettre_vers_lettre[1];
	  if((lettre1 <= 'z') && (lettre1 >= 'a')  && (lettre2 <= 'z') && (lettre2 >= 'a') )
	  {
	      clef_analysee[(int) (lettre1-'a')] = lettre2;
	      Affiche_clair_chiffre(texte_chiffre, longueur_texte,longueur_texte, clef_analysee);
	  }
	  else
	  {
	      printf("\nATTENTION : lettre(s) non valide");
	  }
      }
      else if(strcmp(commande,"bigramme") == 0) /* affichage de la fréquence pour les bigrammes*/
      {
	  printf("\nLes bigrammes affichés ici, sont ceux dont le nombre d'apparution est >= 9 !\n");
	  printf("\n");
	  for(i=0;i<26;i++)
	  {
	      for(j=0;j<26;j++)
	      {
		  if(Nbres_bigrammes[i+26*j] >= 9)
		  {
		      printf("La fréquence du bigramme %c%c vaut %d \n",(char)(((int) 'a') + i),(char)(((int) 'a') + j), Nbres_bigrammes[i+26*j]);
		  }
	      }
	  }	  
      }
      else if(strcmp(commande,"monogramme") == 0) 
      {
	  printf("\n");
	  for(i=0;i<26;i++)
	  {
	     printf("La fréquence de %c vaut %f \n",(char)(((int) 'a') + i), frequence_lettre[i]);
	  }	  
	  printf("\n");
      }
      else /* cas de commande non valide */
      {
	  printf("\nErreur : Nom de commande Invalide");
      }
  }
}
