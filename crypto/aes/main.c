#include "LIBRAIRIES.h" 

#define Nb 4 //4 octets formant une colonne de mot de 32 bits

int Nr=0; //Nombre de tours, au départ initialisé à 0
int Nk=0; //Nombre de mots de 32 bits dans la clé, au début initialisé à 0
unsigned char in[16]; //texte clair
unsigned char out[16]; //texte chiffré
unsigned char state[4][4]; //matrice de manipulation du texte pendant le processus de chiffrement
unsigned char Key[32]; //clé "parisestmagique!" 
unsigned char RoundKey[240]; //les différentes sous-clés

#include "AES_MODIFIE.h"

int main()
{
	int i, L, choix;
        L=0; 
	char nom_fic_in[50];
	char nom_fic_out[50];
	FILE * FIC_IN;
	FILE * FIC_OUT;
	
	/*Obtention de la longeur de la clé*/
	while(L!=128 && L!=192 && L!=256) //Pour ce travail il faut saisir 128
	{
	  printf("Donner la longeur de la clé (128, 192 ou 256 seulement) : ");
	  scanf("%d",&L);
	} //Pour ce travail la clé utilisée est de taille 128
	
	/*Calcul de Nk et Nr à partir de la longueur de la clé*/
	Nk = L / 32;
	Nr = Nk + 6;
	
        printf("Faites votre choix :\n\n");
	printf("Chiffrement d'1 bloc de 128 bits depuis un fichier ==========> 1\n");
	printf("Déchiffrement d'1 bloc de 128 bits depuis un fichier chiffré=> 2\n");
	printf("Chiffrement d'un fichier en utilisant le Mode_1 =============> 3\n");
	printf("Déchiffrement d'un fichier en utilisant le Mode_1 ===========> 4\n");
	printf("Chiffrement d'un fichier en utilisant le Mode_2 =============> 5\n");
	printf("Déchiffrement d'un fichier en utilisant le Mode_2 ===========> 6\n");
	printf("Choix = ");
	scanf("%d", &choix);

	switch (choix)
	{
	  case 1:
	  {
	    printf("Donner le nom du fichier contenant le texte clair : ");
	    scanf("%50s", nom_fic_in);
	    printf("Donner le nom du fichier qui va contenir le texte chiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "r");
	    FIC_OUT = fopen(nom_fic_out, "wb");
	    
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La cle saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    Charger_texte_par_bloc_de_16carac(in, FIC_IN);
	    printf("Le texte à chiffrer est :\n");
	    printf("------------------------------------------------------------------\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", in[i]);
	    }
	    printf("\n------------------------------------------------------------------\n");
	
	    KeyExpansion();//KeyExpansion doit être effectué avant le processus de chiffrement
	    Cipher();
	    
	    for(i=0;i<Nb*4;i++)
	    {
	      fputc(out[i], FIC_OUT);
	    }
	    fclose(FIC_IN);
	    fclose(FIC_OUT);
	    break;
	  }
	  case 2:
	  {
	    printf("Donner le nom du fichier contenant le texte chiffré : ");
	    scanf("%50s", nom_fic_in);
	    
	     printf("Donner le nom du fichier qui va contenir le texte dechiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "rb");
	    FIC_OUT = fopen(nom_fic_out, "w");
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La clé saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    Charger_texte_par_bloc_de_16carac(in, FIC_IN);
	
	    KeyExpansion();/*KeyExpansion doit être effectué avant le processus de chiffrement*/
	    InvCipher();   
	    
	    printf("Le texte dechiffré est :\n");
	    printf("------------------------------------------------------------------\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", out[i]);
	    }
	    printf("\n------------------------------------------------------------------\n");
	  
	  fclose(FIC_IN);
	  fclose(FIC_OUT);
	  break;
	}
	case 3:
	  {
	    printf("Donner le nom du fichier contenant le texte clair : ");
	    scanf("%50s", nom_fic_in);
	    printf("Donner le nom du fichier qui va contenir le texte chiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "r");
	    FIC_OUT = fopen(nom_fic_out, "wb");
	    
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La cle saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    KeyExpansion();//KeyExpansion doit être effectué avant le processus de chiffrement
	    Cipher_Mode_1(FIC_IN, FIC_OUT);
	    
	    fclose(FIC_IN);
	    fclose(FIC_OUT);
	    break;
	  }
	  case 4:
	  {
	    printf("Donner le nom du fichier contenant le texte chiffré : ");
	    scanf("%50s", nom_fic_in);
	    
	     printf("Donner le nom du fichier qui va contenir le texte dechiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "rb");
	    FIC_OUT = fopen(nom_fic_out, "w");
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La clé saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    KeyExpansion();/*KeyExpansion doit être effectué avant le processus de chiffrement*/
	    InvCipher_Mode_1(FIC_IN, FIC_OUT); 
	    
	    
	  fclose(FIC_IN);
	  fclose(FIC_OUT);
	  break;
	}
	case 5:
	  {
	    printf("Donner le nom du fichier contenant le texte clair : ");
	    scanf("%50s", nom_fic_in);
	    printf("Donner le nom du fichier qui va contenir le texte chiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "r");
	    FIC_OUT = fopen(nom_fic_out, "wb");
	    
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La cle saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    KeyExpansion();//KeyExpansion doit être effectué avant le processus de chiffrement
	    Cipher_Mode_2(FIC_IN, FIC_OUT);
	    
	    fclose(FIC_IN);
	    fclose(FIC_OUT);
	    break;
	  }
	  case 6:
	  {
	    printf("Donner le nom du fichier contenant le texte chiffré : ");
	    scanf("%50s", nom_fic_in);
	    
	     printf("Donner le nom du fichier qui va contenir le texte dechiffré : ");
	    scanf("%50s", nom_fic_out);
	    
	    FIC_IN = fopen(nom_fic_in, "rb");
	    FIC_OUT = fopen(nom_fic_out, "w");
	    printf("Entrer la clé qui est une chaîne de caractères (16 caractères pour une clé de 128 bits) : \n");
	    scanf("\n%[^\n]", Key);
	
	    printf("La clé saisie est :\n");
	    for (i=0; i<16; i++)
	    {
	      printf("%c", Key[i]);
	    }
	    printf("\n");
	
	    KeyExpansion();/*KeyExpansion doit être effectué avant le processus de chiffrement*/
	    
	    InvCipher_Mode_2(FIC_IN, FIC_OUT); 
	    
	  fclose(FIC_IN);
	  fclose(FIC_OUT);
	  break;
	}
	}
	return 0;
}
