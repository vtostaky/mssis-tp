
int comp_int(int *,int *);

/* fonction d'affichage */
void affiche_numerique(int *,int );
void affichage_alphabetique_texte(int *,int );
void affiche_clair_vigenere(int *, int ,  int , int *, int );


/* fonctions de chiffrement */
void chiffrer_substitution( int *,  int *, int *,int );
void chiffrer_vigenere( int *, int ,  int *, int *,int );

/* Fonction de calcul de frequence */
int compter_lettre(int *,int ,int );
double indice_coincidence( int *, int );


void extraction_sous_texte( int *, int , int *, int , int , int );

void analyse_coincidence(int *texte_chiffre, int longueur_texte);
void analyse_decalage(int *texte_chiffre, int longueur_texte,int periode,int * clef_analysee);
 