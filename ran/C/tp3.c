#include <stdio.h>
#include <stdlib.h>

void ENTIER_ADR(int* A);
void AFFICHE_TAB_N(int n, int tab[n]);
int* ALLOC_TAB_DYN(int N);
void INIT_ALEA_TAB(int N, int* TAB);
void AFFICHE_TAB_DYN_N(int n, int *tab);
int** ALLOC_MAT_DYN(int nb_l, int nb_c);
void INIT_ALEA_MAT(int nb_l, int nb_c, int** mat);
void LIB_MAT(int nb_l, int **mat);
void AFFICHE_MAT(int nb_l, int nb_c, int **mat);

void AFFICHE_MAT_DANS_FIC(int nb_l, int nb_c, int **mat, FILE *fic);
void INIT_MAT_VIA_FIC(int nb_l, int nb_c, int **mat, FILE *fic);

int RECHERCHE_MOTIF(FILE* fic, char* motif);
void INVERSER_CHAINE_1();
void INVERSER_CHAINE_2(char* str);
void INVERSER_CHAINE_3(char* str, int start, int end);

long int PGCD(long int a, long int b);
long int PPCM(long int a, long int b, long int c);

int main(int argc, char** argv)
{
    int my_integer = 0;
    int tab[] = {11,12,13,14,15};
    int *tab_dyn;
    int **mat_dyn;
    int **mat_dyn_from_file;
    int i;
    FILE *fic;
    FILE *fichier_lorem;

    printf("%d\n", my_integer);
    ENTIER_ADR(&my_integer);
    printf("%d\n", my_integer);

    AFFICHE_TAB_N(sizeof(tab)/sizeof(int), tab);
    AFFICHE_TAB_N(sizeof(tab)/sizeof(int)-3, tab+3);

    tab_dyn = ALLOC_TAB_DYN(15);
    INIT_ALEA_TAB(15, tab_dyn);
    AFFICHE_TAB_DYN_N(15,tab_dyn);

    mat_dyn = ALLOC_MAT_DYN(3,4);
    INIT_ALEA_MAT(3, 4, mat_dyn);

    free(tab_dyn);

    AFFICHE_MAT(3, 4, mat_dyn);
    fic = fopen("test_mat.txt","w");
    AFFICHE_MAT_DANS_FIC(3, 4, mat_dyn, fic);
    fclose(fic);

    LIB_MAT(3, mat_dyn);
    fic = fopen("test_mat.txt","r");
    mat_dyn_from_file = ALLOC_MAT_DYN(3, 4);
    INIT_MAT_VIA_FIC(3, 4, mat_dyn_from_file, fic);

    AFFICHE_MAT(3, 4, mat_dyn_from_file);
    LIB_MAT(3, mat_dyn_from_file);

    fclose(fic);

    fichier_lorem = fopen("lorem.txt","r");
    RECHERCHE_MOTIF(fichier_lorem, "um");
    fclose(fichier_lorem);

    INVERSER_CHAINE_1();
    printf("\n");
    INVERSER_CHAINE_2("azertyuiop");
    printf("\n");
    INVERSER_CHAINE_3("azertyuiop",3,6);
    printf("\n");

    printf("PGCD %d, %d is %ld\n",152,42, PGCD(152, 42));
    printf("PPCM %d, %d is %ld\n",15,6, PPCM(15, 6, 15));

    return 0;
}

void ENTIER_ADR(int* A)
{
    *A = (*A) + 2;
    printf("%d\n", *A);
}

void AFFICHE_TAB_N(int n, int tab[n])
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%d|", tab[i]);
    }
    printf("\n");
}

int* ALLOC_TAB_DYN(int N)
{
    int* tab = malloc(N * sizeof(int));
    return tab;
}

void INIT_ALEA_TAB(int N, int* TAB)
{
    int i;

    for(i = 0; i < N; i++)
    {
        TAB[i] = (rand()%90)+5;
    }
}

void AFFICHE_TAB_DYN_N(int n, int *tab)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("%d|", *(tab+i));
    }
    printf("\n");
}

int** ALLOC_MAT_DYN(int nb_l, int nb_c)
{
    int** my_mat;
    int i;
    my_mat = malloc(nb_l*sizeof(int));

    for(i = 0; i < nb_l; i++)
    {
        *(my_mat+i) = malloc(nb_c*sizeof(int));
    }
    return my_mat;
}

void INIT_ALEA_MAT(int nb_l, int nb_c, int** mat)
{
    int i,j;

    for(i = 0; i < nb_l; i++)
    {
        for(j = 0; j < nb_c; j++)
        {
            *(*(mat+i)+j) = rand()%9;
        }
    }
}

void LIB_MAT(int nb_l, int **mat)
{
    int i;
    for(i = 0; i < nb_l; i++)
    {
        free(*(mat+i));
    }
    free(mat);
}

void AFFICHE_MAT(int nb_l, int nb_c, int **mat)
{
    int i;
    for(i = 0; i < nb_l; i++)
    {
        AFFICHE_TAB_DYN_N(nb_c,*(mat+i));
    }
}

void AFFICHE_MAT_DANS_FIC(int nb_l, int nb_c, int **mat, FILE *fic)
{
    int i,j;
    for(i = 0; i < nb_l; i++)
    {
        for(j = 0; j < nb_c; j++)
        {
            fprintf(fic,"%d ", *(*(mat+i)+j));
        }
        fprintf(fic,"\n");
    }
}

void INIT_MAT_VIA_FIC(int nb_l, int nb_c, int **mat, FILE *fic)
{
    int c;
    int i,j;
    fseek(fic,0,SEEK_SET);
    for(i = 0; i < nb_l; i++)
    {
        for(j = 0; j < nb_c; j++)
        {
            fscanf(fic,"%d", &c);
            mat[i][j] = c;
        }
        fscanf(fic,"\n");
    }
}

int RECHERCHE_MOTIF(FILE* fic, char* motif)
{
    int count = 0;
    int i = 0;
    int nb_line = 0;
    char current_char;

    do
    {
        i = 0;
        current_char=fgetc(fic);
        if(current_char == '\n')
            nb_line++;
        while(motif[i] == current_char && motif[i] != '\0')
        {
            current_char=fgetc(fic);
            i++;
            if(motif[i] == '\0')
            {
                count++;
                printf("found %s at %d\n", motif, nb_line);
            }
        }
    }while(current_char != EOF);
}

void INVERSER_CHAINE_1()
{
    char current_char = fgetc(stdin);
    if(current_char != '.')
        INVERSER_CHAINE_1();
    printf("%c",current_char);
}

void INVERSER_CHAINE_2(char* str)
{
    char current_char = str[0];
    if(current_char != '\0')
        INVERSER_CHAINE_2(str+1);
    printf("%c",current_char);
}

void INVERSER_CHAINE_3(char* str, int start, int end)
{
    char current_char = str[start];
    if(current_char != '\0' && start != end)
        INVERSER_CHAINE_3(str,start+1,end);
    printf("%c",current_char);
}

long int PGCD(long int a, long int b)
{
    int reste;
    if(a > b)
    {
        reste = a%b;
        if(reste == 0)
            return b;
        else
            PGCD(b,reste);
    }
    else if(a < b)
    {
        reste = b%a;
        if(reste == 0)
            return a;
        else
            PGCD(a,reste);
    }
    else
        return a;
}

long int PPCM(long int a, long int b, long int c)
{
    if(a > b)
    {
        if(a%b == 0)
            return a;
        else
            PPCM(a+c, b, c);
    }
    else if(a < b)
    {
        if(b%a == 0)
            return b;
        else
            PPCM(a,b+c,c);
    }
    else
        return a;
}
