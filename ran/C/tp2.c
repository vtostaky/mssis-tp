#include <stdio.h>
#include <stdlib.h>

int LENGTH(char *str);
int DIGITS(char *str);
int CARACTERES(char *str);
void REVERSE(char *str);
int FIND(char *str, char c);
void BITS(char* str);
void FLIP(char* str);
void PERMUTE_BIT_FORT_FAIBLE(char* str);
char* AFFICHAGE_TEXTE();

int main(int argc, char **argv)
{
    char* string_output;
    if(argc > 1)
    {
        printf("%d\n",LENGTH(argv[1]));
        printf("%d\n",DIGITS(argv[1]));
        printf("%d\n",CARACTERES(argv[1]));
        printf("%d\n",FIND(argv[1], 'c'));
        REVERSE(argv[1]);
        printf("%s\n",argv[1]);
        printf("%d\n",FIND(argv[1], 'c'));
        BITS(argv[1]);
        FLIP(argv[1]);
        printf("%s\n",argv[1]);
        BITS(argv[1]);
        PERMUTE_BIT_FORT_FAIBLE(argv[1]);
        BITS(argv[1]);
        string_output = AFFICHAGE_TEXTE();
        printf("%s\n", string_output);
        BITS(string_output);
    }
    if(string_output != NULL)
        free(string_output);
    return 0;
}

int LENGTH(char *str)
{
    int num = 0;
    while(str[num] != '\0')
        num++;
    return num;
}

int DIGITS(char *str)
{
    int i = 0;
    int nb_num=0;

    while(str[i] != '\0')
    {
        if(str[i] >= '0' && str[i] <= '9')
            nb_num++;
        i++;
    }
    return nb_num;
}

int CARACTERES(char *str)
{
    int i = 0;
    int nb_char = 0;

    while(str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            nb_char++;
        i++;
    }
    return nb_char;
}

void REVERSE(char *str)
{
    int i=0;
    int length = LENGTH(str);
    char reverse[length];
    for (i = 0; i < length; i++)
    {
        reverse[i] = '\0';
    }
    i = 0;
    for ( i = 0; i < length; i++)
    {
        reverse[length-i-1] = str[i];
    }
    for(i = 0; i < length; i++)
    {
        str[i] = reverse[i];
    }
}

int FIND(char *str, char c)
{
    int i, index = -1;
    int length = LENGTH(str);

    for(i = 0; i < length; i++)
    {
        if(str[i] == c)
        {
            index = i;
            break;
        }
    }
    return index;
}

void BITS(char *str)
{
    int i,j;
    int length = LENGTH(str);

    for(i = 0; i < length; i++)
    {
        printf("%c, %d, ",str[i],(int)str[i]);
        for(j = 7; j >= 0; j--)
        {
            if(((str[i] >> j) & 1) == 1)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }
}

void FLIP(char *str)
{
    int i,j;
    int length = LENGTH(str);

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < 7; j++)       
            str[i] = str[i]^(1<<j);
    }
}

void PERMUTE_BIT_FORT_FAIBLE(char *str)
{
    int i,j;
    int length = LENGTH(str);

    for(i = 0; i < length; i++)
    {
        if(((str[i] >> 6) & 1) != (str[i] & 1))
        {
            str[i] = str[i]^(1<<6);
            str[i] = str[i]^1;
        }
    }
}

char* AFFICHAGE_TEXTE()
{
    int i,j;
    int length;
    int nb_char;

    char bin_string[32];
    char *char_string;

retry:
    for(i = 0; i < 32; i++)
        bin_string[i] = '\0';

    printf("Enter binary\n");
    fgets(bin_string, 32, stdin);

    length = LENGTH(bin_string);
    nb_char = (length%8 == 0)?length/8:length/8+1;

    for(i = 0; i < length; i++)
        if(bin_string[i] != '0' && bin_string[i] != '1')
        {
            printf("Invalid binary!\n");
            goto retry;
        }

    char_string = calloc(nb_char+1, sizeof(char));
    for(i = 0; i < nb_char; i++)
    {
        for(j = 0; j < 8 && i*8+j < length; j++)
        {
            if(bin_string[i*8+j] == '0')
                char_string[i] = char_string[i] + (0 << (7-j));
            else
                char_string[i] = char_string[i] +
                    (1 << (7-j));
        }
    }
    return char_string;
}
