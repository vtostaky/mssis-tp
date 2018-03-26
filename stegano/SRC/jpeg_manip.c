#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "error.h"
#include "jpeg_manip.h"

/*********************** init_jpeg_img() ********************************
Memory allocation and initialisation of a JPEGimg structr
Returns a pointer to the struct if correctly allocated,
        NULL in case of error

ARGS: (void)
*************************************************************************/
JPEGimg *
init_jpeg_img ( void )
{
	JPEGimg * img = NULL;
	if ( (img = (JPEGimg*) malloc (sizeof(JPEGimg)) ) == NULL)
	{
		print_err ("init_jpeg_img", "img", ERR_MEM);
		return NULL;
	}
	img->dctCoeffs = NULL;
	img->virtCoeffs = NULL;
	
	return img;
}

/************************* free_jpeg_img() *****************************
Free memory
Returns EXIT_SUCCESS
        a negative value in case of failure

ARGS: JPEGimg *img: structure to free
************************************************************************/
int free_jpeg_img ( JPEGimg *img )
{
	// Checkargs
	if (!img)
		return ERR_ARG;
	
	// Free memory
	if (img->dctCoeffs)
	{
		free (img->dctCoeffs);
		img->dctCoeffs = NULL;
	}
	
	jpeg_destroy_decompress(&(img->cinfo));
	free(img);
	img = NULL;
	
	return EXIT_SUCCESS;
}


/************************* jpeg_read() **********************************
Reads a jpeg image, store information and DCT coefficients
Returns a pointer to the structure created,
        NULL in case of failure
Retourne un code d'erreur

ARGS: char *path:       Path of the image
*********************************************************************/
JPEGimg *
jpeg_read (char *path)
{
	int comp;
	struct jpeg_error_mgr jerr; // Handle errors
	FILE *infile = NULL;
	JPEGimg *img = NULL;
	
	// Check args
	if (!path)
	{
		print_err ("jpeg_read()", "path", ERR_ARG);
		return NULL;
	}
	
	// Open path
	if ((infile = fopen(path, "rb") ) == NULL)
	{
		print_err ("jpeg_read()", path, ERR_FOPEN);
		return NULL;
	}
	
	// Memory allocation for img
	if ((img = init_jpeg_img()) == NULL)
	{
		fclose (infile);
		return NULL;
	}
	
	/* Initialize the JPEG decompression object with default error handling. */
	img->cinfo.err = jpeg_std_error (&jerr);
	jpeg_create_decompress (&(img->cinfo));
  
	/* Specify data source for decompression */
	jpeg_stdio_src (&(img->cinfo), infile);
  
	// Read header
	(void) jpeg_read_header (&(img->cinfo), TRUE);
  
	/* Get DCT coefficients
	 * dct_coeffs is a virtual array of the components Y, Cb, Cr
	 * access to the physical array with the function
	 * (cinfo->mem -> access_virt_barray)*/
	img->virtCoeffs = jpeg_read_coefficients (&(img->cinfo));
	
	// Structure allocation
	img->dctCoeffs = (JBLOCKARRAY*) malloc (sizeof(JBLOCKARRAY) * img->cinfo.num_components );
	if (img->dctCoeffs == NULL)
	{
		print_err ("jpeg_read()", "img->dctCoeffs", ERR_MEM);
		fclose (infile);
		return NULL;
	}
  
	// Loop on the components of the virtual array to get DCT coefficients
	for(comp = 0; comp < img->cinfo.num_components; comp++)
	{
  	img->dctCoeffs[comp] = (img->cinfo.mem -> access_virt_barray)((j_common_ptr) &(img->cinfo),
       img->virtCoeffs[comp], 0, 1, TRUE);
	}
  
	// free and close
	fclose (infile);
                        
	return img;
}

/******************** jpeg_write_from_coeffs() **************************
Write a jpeg image from its DCT coeffs
Returns EXIT_SUCCESS, 
        a negative value in case of error

ARGS: char *outfile: Path of the new image
      JPEGimg *img:  Structure containing DCT coeffs and infos of the image
************************************************************************/
int 
jpeg_write_from_coeffs (char *outfile, JPEGimg *img)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *output = NULL;

	// Open file
	if ((output = fopen (outfile, "wb")) == NULL)
	{
		print_err( "jpeg_write_from_coeffs()", outfile, ERR_FOPEN);
		return ERR_FOPEN;
	}
	
	/* Initialize the JPEG compression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	/*telling, where to put jpeg data*/
	jpeg_stdio_dest(&cinfo, output);

	/* Applying parameters from source jpeg */
	jpeg_copy_critical_parameters(&(img->cinfo), &cinfo);

	/* copying DCT */
	jpeg_write_coefficients(&cinfo, img->virtCoeffs);

	/*clean-up*/
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose (output);
	
	/*Done!*/
	return EXIT_SUCCESS;
}

/**************************** getDCTpos() *********************************
Get the position of a DCT coefficient regarding the value of pos
Fill the the structure position given as arguement {component, line, column, coeff_index}
Return EXIT_SUCCESS or an error

ARGS:     [IN] JBLOCKARRAY *img: DCT coeffs
          [IN] sjdec *cinfo:     image informations
          [IN] int pos:          integer position
	  [IN,OUT] DCTpos *position: return value (must be already allocated)
***************************************************************************/
int getDCTpos (JBLOCKARRAY *img, sjdec *cinfo, int pos, DCTpos * const position)
{
	int nbDCTblocks = 0; // Number of DCT coefficients into a component

	// Check arguments
	if (!img || !cinfo || !position)
	{
		print_err("getDCTpos()", "img, cinfo or position", ERR_ARG);
		return ERR_ARG;
	}
	position->comp = 0;
	
	// Find position in DCT block position
	position->coeff = pos & 63; // equivalent to pos % 64
	pos >>= 6;                  // equivalent to pos / 64
	
	// Find component
	while (position->comp < cinfo->num_components)
	{
		nbDCTblocks = cinfo->comp_info[position->comp].height_in_blocks * cinfo->comp_info[position->comp].width_in_blocks;
		if (pos < nbDCTblocks)
			break;
		position->comp++;
		pos -= nbDCTblocks;
	}
	
	// Find line and column
	position->col = pos % cinfo->comp_info[position->comp].width_in_blocks;
	position->lin = pos / cinfo->comp_info[position->comp].width_in_blocks;
	
	//	printf ("Pos = %d\t%d\t%d\t%d\n", position->comp, position->lin, position->col, position->coeff);
		
	return EXIT_SUCCESS; 
}


/**************************** getDCTcoeffValue() *********************************
Given an image and a position, returns the DCT value of the coefficient in the argument value
Return EXIT_SUCCESS or an error code

ARGS:     [IN] JBLOCKARRAY *img:    DCT coeffs
          [IN] sjdec *cinfo:        image informations
          [IN] DCTpos *pos:         structure position
          [IN,OUT] int *coeffValue: return value (must be already allocated)
***************************************************************************/
int getDCTcoeffValue(JBLOCKARRAY *img, sjdec *cinfo, DCTpos *pos, int *coeffValue)
{
	// Check arguments
	if (!img || !cinfo || !pos || !coeffValue)
	{
		print_err("getDCTpos()", "img, cinfo or position", ERR_ARG);
		return EXIT_FAILURE;
	}

	if (pos->comp < 0 || pos->comp >= cinfo->num_components
		|| pos->lin < 0 || pos->lin >= cinfo->comp_info[pos->comp].height_in_blocks
		|| pos->col < 0 || pos->col >= cinfo->comp_info[pos->comp].width_in_blocks
		|| pos->coeff < 0 || pos->coeff >= 64 )
		return ERR_TREAT;
	
	*coeffValue = img[pos->comp][pos->lin][pos->col][pos->coeff];
	return EXIT_SUCCESS;
}

/*--------------------------readFile()-----------------------------
Read a file and store it in an array, update the size of the file
Return the array or NULL in case of error

ARGS:	[IN] path: filepath to read
   [IN, OUT] size: pointer to the size (must be already allocated)
---------------------------------------------------------------------------*/
char * lecture_fichier(char *path, int *size)
{
	// initialisation des variables 
	int i;
	char *tab = NULL;
	FILE *fichier = NULL;

	// Analyse des arguments 
	if (!path || !size)
	{
		print_err("readFile()", "args", ERR_ARG);
		return NULL;
	}

	// Ouverture du fichier 
	fichier = fopen(path, "rb");
	if (fichier == NULL)
	{
		print_err("readFile()", "fichier", ERR_MEM);
		return NULL;
	}

	// recuperation de la taille du fichier
	fseek(fichier, 0, SEEK_END);
	*size = ftell(fichier);

	// allocation memoire du tableau de retour
	if ((tab = (char*)calloc(sizeof(char), *size)) == NULL)
	{
		print_err("readFile()", "tab", ERR_MEM);
		return NULL;
	}

	// parcours du fichier et remplissage du tableau 
	fseek(fichier, 0, SEEK_SET);
	fread(tab, 1, *size, fichier);

	// fermeture du fichier
	fclose(fichier);

	return (tab);
}
