#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

void print_cwd(void)
{
    /* You know, it's good to get into the habit of abstracting things. */
#if DEBUG
    char cwd[100];
    (void) memset(cwd, 0, sizeof(cwd));
    (void) printf("Current Directory: %s\n", getcwd(cwd, sizeof(cwd)));
#endif
}

int main(int argc, char **argv)
{
    char cmd[100];
    print_cwd();

    char tampon[UCHAR_MAX];
    FILE *sortie;

    if ((sortie = popen ("ls -d */ 2> /dev/null", "r")) == NULL) {
        fprintf (stderr, "erreur");
        return EXIT_FAILURE;
    }

    while (fgets (tampon, sizeof tampon, sortie) != NULL) {
        tampon[strlen(tampon)-1] = '\0';
        sprintf(cmd, "cp %s %s",argv[0], tampon);
        system(cmd);

        if(!chdir(tampon)) /* chdir returns 0 on success */
        {
            print_cwd();
            sprintf(cmd, "./%s",argv[0]);
            system(cmd);
            chdir("..");
        }
        else
        {
            perror("Uh, something that shouldn't happen happened!");
            return -1;
        }
    }

    fclose (sortie);
    return EXIT_SUCCESS;
}
