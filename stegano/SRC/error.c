#include <stdlib.h>
#include <stdio.h>

#include "error.h"

/**************************** print_err() ******************************
Affiche un message d'erreur en fonction de l'erreur passee
Ne renvoie rien
**********************************************************************/
void print_err (char *function, char *var, int ERR)
{
	switch (ERR)
	{
		case ERR_MEM: // Erreur memoire sur un *alloc()
			fprintf (stdout, "In function %s, memory allocation error with '%s'\n\n",
			         function, var);
		break;
		
		case ERR_FOPEN: // Erreur ouverture de fichier
			fprintf (stdout, "In function %s, cannot find file '%s'\n\n",
			         function, var);
		break;
		
		case ERR_FREAD: // Erreur lecture de fichier
			fprintf (stdout, "In function %s, cannot read more in file '%s'\n\n",
			         function, var);
		break;
		
		case ERR_ARG: // Erreur argument
			fprintf (stdout, "In function %s, error with argument '%s'\n\n",
			         function, var);
		break;
		
		case ERR_TREAT: // Erreur d'execution dans la fonction
			fprintf (stdout, "In function %s, treatment error with function %s\n\n",
			         function, var);
		break;
		
		default: // Erreur inconnue
			fprintf (stdout, "In function %s, unknown error with '%s'. Error code = %d\n\n",
			         function, var, ERR);
		break;
	}
}
