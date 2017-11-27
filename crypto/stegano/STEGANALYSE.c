/*
 On suppose que vous avez le package SDL déjà installé sur votre machine, sinon il faudra l'installer. Pour compiler il suffit de taper la commande :
 gcc -o EXEC STEGANALYSE.c `sdl-config --cflags --libs` -Wall
 
 Le code a été largement commenté pour vous permettre de le comprendre plus facilement et plus rapidement.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>


/*Fonction qui sert à charger une image*/
SDL_Surface * load_image( char * filename ) 
{
	SDL_Surface * Image_chargee = NULL; //Surface tampon qui nous servira pour charger l'image
	SDL_Surface * Image_optimisee = NULL; //L'image optimisée qu'on va utiliser

	Image_chargee = SDL_LoadBMP(filename); //Chargement de l'image BMP

	if( Image_chargee != NULL ) //Si le chargement se passe bien
	{ 	 
	    Image_optimisee = SDL_DisplayFormat(Image_chargee); /*Pour convertir l'image chargée vers le format 
	                                                       d'affichage adapté (celui de l'écran actuel)*/
	    SDL_FreeSurface(Image_chargee); //Libération de l'ancienne image chargée
	}
	return Image_optimisee; //On retourne l'image optimisée 
}



/* Pour applliquer une surface sur une autre à partir de la position en haut à gauche (x,y)*/
void appliquer_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination) 
{ 
	SDL_Rect POS; /*Pour définir une zone rectangulaire commençant en haut à gauche*/
	POS.x = x; /*position x de depart en haut à gauche*/
	POS.y = y; /*position y de depart en haut à gauche*/
	SDL_BlitSurface(source, NULL, destination, &POS ); //On applique toute la surface source => destination
}



/* Fonction qui permet de recuperer la valeur d'un pixel d'une image, à une position (x,y) donnée. 
   Ce qui permettrait ensuite de recupérer les valeurs des composant RGB du pixel via la fonction SDL_GetRGB.*/
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel; /*Pour obtenir l'encodage des pixels de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;
 
    case 2:
        return *(Uint16 *)p;
 
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2]; //L'octet le plus fort est mis à l'adresse mémoire la plus petite
        else
            return p[0] | p[1] << 8 | p[2] << 16; //L'octet le plus faible est mis à l'adresse mémoire la plus petite
    case 4:
        return *(Uint32 *)p;
 
    default:
        return 0;
    }
}



/*Fonction qui recherche une image cachée dans une autre image*/
void recherche_image_cachee(SDL_Surface * image_mere, SDL_Surface * image_cachee)
{
  /*vous utiliserez la fonction getpixel pour récupérer les pixels de l'image mère*/
  /*vous utiliserez la fonction SDL_GetRGB pour récupérer les composants RGB du pixel de l'image mère*/
    int i,j;
    Uint32 pixel;
    Uint8 r,g,b;
    Uint8 r_res,g_res,b_res;
    Uint8 *p;
    int bpp = image_mere->format->BytesPerPixel; /*Pour obtenir l'encodage des pixels de l'image*/

    for(i = 0; i < image_mere->w/2; i++)
    {
        for(j = 0; j < image_mere->h; j++)
        {
            pixel = getpixel(image_mere, 2*i, j);
            SDL_GetRGB(pixel,image_mere->format,&r,&g,&b);
            r_res = (r & 0xf) << 4;
            g_res = (g & 0xf) << 4;
            b_res = (b & 0xf) << 4;
            pixel = getpixel(image_mere, 2*i+1, j);
            SDL_GetRGB(pixel,image_mere->format,&r,&g,&b);
            r_res |= r & (0xf);
            g_res |= g & (0xf);
            b_res |= b & (0xf);
            pixel = SDL_MapRGB(image_mere->format,r_res,g_res,b_res);
            p = (Uint8 *)image_cachee->pixels + j * image_cachee->pitch + i * bpp;
            *(Uint32*)p = pixel;
        }
    }
}



/*Fonction qui recherche le texte caché dans l'image fille*/
void recherche_texte_cache_dans_image(SDL_Surface * image, char * texte)
{
    int i,j;
    Uint32 pixel;
    Uint8 r,g,b;

    for(i = 0; i < image->w; i++)
    {
        for(j = 0; j < image->h; j++)
        {
            pixel = getpixel(image, i, j);
            SDL_GetRGB(pixel,image->format,&r,&g,&b);
            texte[j*(image->w)+i] =  (r & 0x3) << 5;
            texte[j*(image->w)+i] |= (g & 0x3) << 3;
            texte[j*(image->w)+i] |= (b & 0x7);
        }
    }
}


/*Fonction qui affiche un texte sur la sortie standard*/
void affichage_texte(char * TEXTE)
{
 printf("-----------------------------------------------------------------------------------------------\n");
 printf("%s\n", TEXTE);
 printf("-----------------------------------------------------------------------------------------------\n");
}




int main (int argc, char * argv[])
{
  int LARGEUR_ECRAN, HAUTEUR_ECRAN, CODAGE, PAUSE;
  LARGEUR_ECRAN=1300; HAUTEUR_ECRAN=650;
  CODAGE = 24; //8x3=24 bits / pixel couleur
  char * TEXTE;
  SDL_Surface * ecran = NULL;        //déclaration d'une surface d'écran de base (sur laquelle on va travailler)
  SDL_Surface * IMAGE = NULL;        //déclaration d'une surface (pour l'image qui cache)
  SDL_Surface * image_cachee = NULL; //Déclaration d'une autre surface (pour l'image cachée)
  

  SDL_Event event; //Pour gérer les evénements type clic de souris
  
 
  ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, CODAGE, SDL_SWSURFACE); //Initialisation du mode video
  SDL_WM_SetCaption( "Steganalyse Image BMP", NULL ); //Mise en place de la barre caption nommée "Steganalyse Image BMP"
  
  IMAGE = load_image( "tiger_stegano.bmp" ); //Chargement de l'image mère contenant les informations cachées
  
  appliquer_surface(0, 0, IMAGE, ecran); //On applique l'image mère sur l'écran de base au coin haut gauche (0,0)
  
  SDL_Flip( ecran ); //pour mettre à jour l'écran et faire apparaitre la nouvelle surface appliquée juste avant
  
  
  PAUSE=1;
  while (PAUSE) 
  {
     SDL_WaitEvent(&event); //attend qu'un événement se produise
     switch(event.type) 
     {
      case SDL_MOUSEBUTTONUP: // en cas de Clic de la souris
         if(event.button.button==SDL_BUTTON_LEFT) 
	 {PAUSE=0;/*Pour ensuite sortir de la boucle et conituer le programme*/}
     } 
  }
  
  image_cachee = SDL_CreateRGBSurface(0, 400, 555, 24, 0, 0, 0, 0); //Création d'une surface RGB 150x250 (24 bits/pixel)
  
  recherche_image_cachee(IMAGE, image_cachee); //on cherche l'image cachée dans l'image mère
  
  printf("L'image cachée est la suivante :\n");
  appliquer_surface(850, 0, image_cachee, ecran); //On applique l'image sur l'écran à la position (y=0 et x=350)
  SDL_Flip(ecran); //pour mettre à jour l'écran et faire apparaitre la nouvelle surface appliquée précédemment
  
  PAUSE=1;
  while (PAUSE) /*Pour faire une pause sur l'écran et observer */
  {
     SDL_WaitEvent(&event); //attend qu'un événement se produise
     switch(event.type) 
     {
      case SDL_MOUSEBUTTONUP: // en cas de Clic de la souris
         if(event.button.button==SDL_BUTTON_LEFT) //Si on clique sur le bouton gauche de la souris 
	 {PAUSE=0;}
     } 
  }
  
  /*Recherche puis affichage du texte caché dans une image*/
  TEXTE = malloc(sizeof(char)*222000);
  recherche_texte_cache_dans_image(image_cachee, TEXTE);
  printf("Le texte caché est le suivant :\n");
  affichage_texte(TEXTE);


  SDL_FreeSurface(IMAGE);
  SDL_FreeSurface(image_cachee);
  SDL_FreeSurface(ecran);
  free(TEXTE);
  SDL_Quit();
  
  return EXIT_SUCCESS;
}











































