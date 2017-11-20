/**************************************************************************/
/****                                                                  ****/
/****                   Cryptanalyse Vigenere                          ****/
/****                                                                  ****/
/**************************************************************************/

#include "LIBRAIRIES.h"
#include "CRYPTANALYSE_VIGENERE.h"


int comp_int(int * a, int * b)
{
  return((*a) - (*b));
}



/****                 Affichage numérique d'un texte                 ***/
void affiche_numerique(int * texte, int longueur_texte)
{
  int i;
  printf("\n");
  for(i=0; i<longueur_texte; i++)
  {
     printf(" %d ",texte[i]);
  }
  printf("\n");
}



/****                 Affichage alphabetique d'un texte                 ***/
/*ATTENTION : Les lettres sont normalement en minuscule*/
void affichage_alphabetique_texte(int * texte, int longueur_texte)
{
  int i;
  printf("\n");
  for(i=0; i<longueur_texte; i++)
    {
      printf("%c",(char)(texte[i]+((int) 'a')));
    }
  
  printf("\n\nLa longueur du texte est de %d caractères\n\n", longueur_texte);
}



/****                 Affichage côte à côte du texte clair partiellement retrouvé et du texte chiffré correspondant                 ***/
void affiche_clair_vigenere(int * texte, int longueur_texte,  int longueur, int * clef_analysee, int periode)
{
  int i,j,longueur_ligne,nb_ligne,lettre, reste, fin;
  longueur_ligne=100;
  fin = longueur_ligne;

  //pour adapter la longueur du texte à afficher en fonction de la longueur d'une ligne qui est ici de 100
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
  else if(longueur <= longueur_texte )
  {
      nb_ligne = longueur/longueur_ligne;
      nb_ligne++;
      reste = longueur - (longueur_ligne*(nb_ligne-1));
  }
  
  for(i=0;i<nb_ligne;i++)
  {
      /* affichage de la ligne claire */
      if (i==(nb_ligne-1) && reste != 0)
      {
	fin = reste;
      }
      for(j=0;j<fin;j++)
      {	  
	  if(clef_analysee[((i*longueur_ligne+j) % periode)] == -1)
	  {
	      printf("*");
	  }
	  else
	  {
	    lettre=(texte[(i*longueur_ligne+j)] - clef_analysee[((i*longueur_ligne+j) % periode)]);
	    if(lettre >= 26)
	    {
		  printf("%c",((char) (lettre - 26)) + 'a');
	    }
	    if( lettre < 0 )
	    {
		  printf("%c",((char) (lettre + 26)) + 'a');
	    }
	    else
	    {
		  printf("%c",((char) (lettre)) + 'a');
	    }
	  }
      }

      printf("\n");

      /* affichage de la ligne chiffre */
      for(j=0;j<fin;j++)
      {	  
	printf("%c",(char)(texte[i*longueur_ligne+j]+((int) 'a')));
      }
      printf("\n\n");
  }
}





/****       Fonction affiche le texte clair complètement retrouvé                    ****/
void affiche_clair(int *texte_chiffre, int longueur_texte, int * clef_analysee, int periode)
{
  int j, lettre;
  printf("\n");
  for(j=0;j<longueur_texte;j++)
  {	  
	  if(clef_analysee[j % periode] == -1)
	  {
	      printf("*");
	  }
	  else
	  {
	    lettre=(texte_chiffre[j] - clef_analysee[j % periode]);
	    if(lettre >= 26)
	    {
		  printf("%c",((char) (lettre - 26)) + 'a');
	    }
	    if( lettre < 0 )
	    {
		  printf("%c",((char) (lettre + 26)) + 'a');
	    }
	    else
	    {
		  printf("%c",((char) (lettre)) + 'a');
	    }
	  }
  }
  printf("\n");
}


  
/****       Fonction qui compte le nombre de "lettre" dans un texte                    ****/
int compter_lettre(int *texte, int longueur_texte, int lettre)
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



/****       Fonction de calcul d'indices de coincidences                          *******/
double indice_coincidence( int * texte, int longueur_texte)
{
  int lettre;
  double indice,p;
  indice=0.0;
  /*
   * 
   * 
   * A remplir (La formule est simple et est donnée par la somme des Pi^2 où i=a, b, ..., z et 
   * Pm désigne par exemple la probabilité de tomber sur la lettre m)
   * Vous pouvez utiliser la fonction compter_lettre juste au dessus.
   * 
   * 
   */
  for(lettre = 0; lettre < 26; lettre++)
  {
     p = (double)compter_lettre(texte, longueur_texte, lettre) / (double)longueur_texte;
     indice += p*p;
  }
  return(indice);

}


/****       Fonction pour extraire un sous-texte                         *******/
/*On tient compte de la période et du décalage à effectuer à chaque fois pour bien se positionner*/
void extraction_sous_texte( int * texte, int longueur_texte, int * sous_texte, int longueur_sous_texte, int periode, int indice_dec)
{
    int i;

    for(i = 0; i < longueur_sous_texte; i++)
    {
        if(i * periode + indice_dec < longueur_texte)
            sous_texte[i] = texte[i * periode + indice_dec];
    }
}



/****       Fonction qui cherche les indices de coincidences pour toutes le périodes données                        *******/
void analyse_coincidence(int *texte_chiffre, int longueur_texte)
{
  int periode, i, Taille_bloc, longueur_sous_texte, Tranche_Periode;
  char commande[100];
  longueur_sous_texte = 0;
  
  int * sous_texte;
  // char clef_analysee[26];

  /* Boucle de déchiffrement par analyse de frequence */
  while(strcmp(commande,"exit") != 0)
  {
      /* Entree de la commande */
      printf("\nTapez une commande : ");
      scanf("%99s",commande);
 
      /*Analyse et traitement de la commande*/

      /* demande d'aide */
      if(  strcmp(commande,"help") == 0)
      {
	  printf("\nListe de commmandes disponibles :\nexit  => quitter le programme, \nsuite => suite de l'analyse, \nhelp  => affiche la liste des commandes, \ntest  => pour une tranche de periodes données, ca va afficher les indices de coincidences des sous-textes correspondants à la periode \n");	  
      }

      /* demande de quitter */
      else if(strcmp(commande,"exit") == 0)
      {
	  exit(-1);
      }
      /* demande de fin d'analyse */
      else if(strcmp(commande,"suite") == 0)
      {
	  break;
      }

      /* analyse de coincidence */
      else if(strcmp(commande,"test") == 0)
      {
	  printf("Donner une tranche positive des periodes à tester :\n");
	  scanf("%d",&Tranche_Periode);
	  while (Tranche_Periode <= 0)
	  {
	    printf("Donner une tranche positive des periodes à tester :\n");
	    scanf("%d",&Tranche_Periode);
	  }
	  printf("P : Indices de coincidence pour les sous-textes correspondants \n");
	  printf("------------------------------------------------------------------------------------------------------------- \n");
	  for (periode=1; periode<=Tranche_Periode; periode++)
	  {
	     
	      Taille_bloc=longueur_texte/periode; //pour trouver la taille initiale des blocs 
	      sous_texte = malloc((Taille_bloc+1) * sizeof(int));
	      printf("%2d : ", periode);
	      for(i=0;i<periode;i++)
	      {
		  //cette condition sert à trouver la bonne longueur du sous-texte pour ne pas déborder
		  if(Taille_bloc * periode + i >= longueur_texte)
                  {
                     longueur_sous_texte = Taille_bloc;
                  }
                  else
                  {
                     longueur_sous_texte = Taille_bloc+1;
                  }
                  
		  extraction_sous_texte(texte_chiffre, longueur_texte, sous_texte, longueur_sous_texte, periode, i);//Pour extraire le sous-texte correspondant
		  printf(" %.4f ", indice_coincidence(sous_texte, longueur_sous_texte));//pour affricher l'indice de coincidence du sous-texte correspondant
	      }
	      printf("\n");
              free(sous_texte);
	  }
	  printf("------------------------------------------------------------------------------------------------------------- \n\n");
	  printf("Il faut imaginer la bonne longueur de clé, avant de taper 'suite' pour continuer la cryptanalyse \n");
      }
      else /* cas de commande non valide */
	{
	  printf("\n Erreur : Nom de commande Invalide");
	}
    }
}


/****       Fonction qui recherche les éléments de la clé par rapport à sa longueur                       *******/
/*Cette fonction tente également le déchiffrement associée à la clé choisie*/
void analyse_decalage(int *texte_chiffre, int longueur_texte,int periode,int * clef_analysee)
{
  int i,j,entree, Taille_bloc;
  char commande[100];
  int longueur_sous_texte,longueur;
  longueur_sous_texte=0;
  
  int * sous_texte;

  for(i=0; i<periode; i++)
    {
      clef_analysee[i]=-1; //initialisation à -1 de tous les éléments de la clé
    }

  /* Boucle de déchiffrement par analyse de fréquence */
  while(strcmp(commande,"exit") != 0)
    {
      /* Entree de la commande */
      printf("\nTapez une commande : ");
      scanf("%99s",commande);
 

      /*Analyse et traitement de la commande*/

      /* demande d'aide */
      if(strcmp(commande,"help") == 0)
      {
	  printf("\nListe de commmandes :");
	  printf("\nexit         = quitter le programme,");
	  printf("\nhelp         = affiche la liste des commandes,");
	  printf("\nfrequence j  = affiche la fréquence des lettres du sous texte d'indice j");
	  printf("\naffichage l  = affichage des l premières lettres du texte clair partiellement retrouvées");	
	  printf("\nclair        = affichage uniquement du texte clair");
	  printf("\ncle          = affichage de la clé partiellement retrouvée \n");	  

      }
      else if(strcmp(commande,"exit") == 0) /* demande de fin d'analyse */
      {
	 exit(-1);
      }
      else if(strcmp(commande,"affichage") == 0) /* affichage texte clair et chiffré */
      {
	  scanf("%d",&longueur);
	  affiche_clair_vigenere(texte_chiffre,longueur_texte,longueur,clef_analysee,periode);
      }
      else if(strcmp(commande,"clair") == 0) /* affichage texte clair uniquement*/
      {
	  affiche_clair(texte_chiffre, longueur_texte, clef_analysee, periode);
      }
      else if(strcmp(commande,"cle") == 0)/* affichage de la clef */
      {
	  printf("\n");
	  for(i=0;i<periode;i++) /* pour afficher la correspondance en chiffre */
	    {
	      if(clef_analysee[i] == -1)
	      {
		 printf(" * ");
	      }
	      else
	      {
		 printf(" %d ",clef_analysee[i]);
	      }
	    }
	    printf(" (");
	  for(i=0;i<periode;i++) /* pour afficher la correspondance en lettre */
	  {
	      if( clef_analysee[i] == -1)
	      {
		 printf("*");
	      }
	      else
	      {
		 printf("%c", (char)(clef_analysee[i] + 'a'));
	      }
	  }
	  printf(")\n");
      }
      else if(strcmp(commande,"frequence") == 0) /* analyse de coincidence */
      {
	  scanf("%d",&j);
	  Taille_bloc=longueur_texte/periode;
	  sous_texte = malloc((Taille_bloc+1) * sizeof(int));
	      
	  //pour bien retrouver la longueur du sous-texte
	  if(Taille_bloc * periode + j > longueur_texte)
          {
            longueur_sous_texte = Taille_bloc;
          }
          else
          {
            longueur_sous_texte = Taille_bloc+1;
          }
            
	  if( (0 <= j) && (j < periode))
	    {
	      //On extrait d'abord le sous-texte correspondant, avant de procéder à l'analyse
	      extraction_sous_texte(texte_chiffre, longueur_texte, sous_texte, longueur_sous_texte, periode, j);

	      printf("\n");
	      for(i=0;i<26;i++)
	      {
		printf("La frequence de %c vaut %f \n",(char)(((int) 'a') + i), ((double) compter_lettre(sous_texte, longueur_sous_texte,i))/((double) longueur_sous_texte));
	      }
	      printf("\nNumerisation\n");
	      for(i=0;i<26;i++) 
		{
		  printf(" %c ",'a'+(char)i);
		}
	      printf("\n");
	      for(i=0;i<10;i++)
		{
		  printf(" %d ",i);
		}
	      for(i=10;i<26;i++)
		{
		  printf(" %d",i);
		}
	      printf("\n\n");
	      printf("Chercher puis saisir le décalage qui a été utilisé lors du chiffrement de ce sous texte\n");
	      printf("Décalage pour ce sous texte  = ");
	      scanf("%d",&entree); 
	      if(j != periode)
		{
		  clef_analysee[j] = entree;
		}
	      else
		{
		  clef_analysee[0] = entree;

		}
	    }
	  else
	    {
	      printf("\nAttention : indice de sous texte invalide");
	    }
	    free(sous_texte);
      }
      else /* cas de commande non valide */
	{
	  printf("\n Erreur : Nom de commande invalide");
	}
    }
}
