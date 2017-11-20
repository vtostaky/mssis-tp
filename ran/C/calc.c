#include <stdio.h>
#include <stdlib.h>

#define INFINITE 0xFFFFFFFFFFFFFFFF

double ADD(double a, double b);
double SOU(double a, double b);
double MUL(double a, double b);
double DIV(double a, double b);
double PUI(double a, double b);
double MOD(double a, double b);

int main(int argc, char** argv)
{

    double a, b;
    char operateur;
    double res;
    char entry[255];
//    printf("\033[%d;%dH",15,10);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("Enter numbers and operator +,-,*,/,%,^\n");
    printf("Press exit to quit, or clear to reset\n");

    while(1)
    {
        //printf("Enter numbers and operator +,-,*,/,%,^\n");

        fgets(entry, 255, stdin);
        if(!strncmp(entry, "exit", 5))
        {
            printf("bye bye");
            break;
        }
        else if(!strncmp(entry, "clear", 5))
        {
            printf("\033[2J");
            printf("\033[1;1H");
            printf("Enter numbers and operator +,-,*,/,%,^\n");
            printf("Press exit to quit, or clear to reset\n");
            continue;
        }
        sscanf(entry, "%lf %c %lf", &a, &operateur, &b);

        //printf("%lf %c %lf\n",a,operateur,b);

        switch(operateur)
        {
            case '+':
                printf("%lf\n",ADD(a,b));
                break;
            case '-':
                printf("%lf\n",SOU(a,b));
                break;
            case '*':
                printf("%lf\n",MUL(a,b));
                break;
            case '/':
                res = DIV(a,b);
                if(res != INFINITE)
                    printf("%lf\n",res);
                break;
            case '%':
                printf("%lf\n",MOD(a,b));
                break;
            case '^':
                printf("%lf\n",PUI(a,b));
                break;
            default:
                a = 0;
                b = 0;
                operateur = 0;
                printf("Enter valid operators +,-,/,*,%,^\n");
                break;
        }
    }   
}

double ADD(double a, double b)
{
    return (a + b);
}

double SOU(double a, double b)
{
    return (a - b);
}

double DIV(double a, double b)
{
    if(b == 0)
    {
        printf("You cannot divide by zero!\n");
        return INFINITE;
    }
    return (a/b);
}

double MUL(double a, double b)
{
    return (a*b);
}

double PUI(double a, double b)
{
    double res = 1;
    int i;
    for(i = 0; i < b; i++)
    {
        res *= a;
    }
    return (res);
}

double MOD(double a, double b)
{
    return ((int)a%(int)b);
}
