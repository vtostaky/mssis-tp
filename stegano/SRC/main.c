#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "error.h"
#include "jpeg_manip.h"

#ifdef WIN32
void pause(void)
{
	system("pause");
}
#endif

int compute_num_coeff(JPEGimg *img)
{
    int i, num = 0;
    for (i = 0; i<img->cinfo.num_components; i++)
	{
		num += img->cinfo.comp_info[i].height_in_blocks * img->cinfo.comp_info[i].width_in_blocks * 64;
	}
    return num;
}

int change_image_coeff(JPEGimg *img, float rate, char* imgpath, int num)
{
    int i, value = 1;
    int return_value;
	DCTpos position = { 0 };

    for (i = 0; i < num * rate; i++)
    {
	    getDCTpos(img->dctCoeffs, &(img->cinfo), i, &position);
        value = rand() % 2;
        if(value)
            img->dctCoeffs[position.comp][position.lin][position.col][position.coeff] ^= 1;
    }
	
    // write it in a new file
	return_value = jpeg_write_from_coeffs(imgpath, img);
	//if (return_value == EXIT_SUCCESS)
		//printf("\nImage written in %s\n", imgpath);

    return return_value;
}

int compute_histo(JPEGimg *img, int num, char * csvpath, int *tab)
{
    int ret = -1;
    int i, value;
	DCTpos position = { 0 };
    FILE *fp = fopen(csvpath, "w");

    if(fp == NULL)
        goto exit;

    for (i = 0; i <= 200; i++)
    {
        tab[i] = 0;
    }

    for (i = 0; i < num; i++)
    {
	    getDCTpos(img->dctCoeffs, &(img->cinfo), i, &position);
        value = img->dctCoeffs[position.comp][position.lin][position.col][position.coeff];

        if(value <= 100 && value >= -100)
            tab[value+100]++;
    }

    for (i = 0; i <= 200; i++)
    {
        fprintf(fp, "%d,%d\n",i-100,tab[i]);
    }

    fclose(fp);
    ret = 0;
exit:
    return ret;
}

/***************************** main() ***********************************
Main function (see help for more details)
************************************************************************/
int main(int argc, char ** argv)
{
	int i; // Loop variable
	int return_value;
	JPEGimg *img = NULL;
	DCTpos position = { 0 };
    int num = 0;
    char histo_name[200];
    int tab_cover[201];
    int tab_stega[201];
    unsigned int diff = 0;
    unsigned int main_diff = 0;
    unsigned int average_diff = 0;

	// To pause the system at exit if run under windows
#ifdef WIN32
	atexit(pause);
#endif

	// Check number of arguments of the command line
	if (argc < 3)
	{
		printf("%s: Reads a jpeg image and write it in a new file\n", argv[0]);
		printf("Not enough arguments for %s\n", argv[0]);
		printf("Usage: %s <cover.jpg> <copy.jpg>\n", argv[0]);
		return EXIT_FAILURE;
	}

	srand((int)time(NULL));
	// read image
	img = jpeg_read(argv[1]);
	if (!img)
		return EXIT_FAILURE;

	// print some informations
	//printf("Number of components: %d\n", img->cinfo.num_components);
	//for (i = 0; i<img->cinfo.num_components; i++)
	//{
		//printf("Component %d:\n   %d lines, %d columns\n\n", i, img->cinfo.comp_info[i].height_in_blocks, img->cinfo.comp_info[i].width_in_blocks);
	//}

	getDCTpos(img->dctCoeffs, &(img->cinfo), 0, &position);
	getDCTcoeffValue(img->dctCoeffs, &(img->cinfo), &position, &return_value);
    img->dctCoeffs[0][0][0][1] += 1;
	//printf("First coefficient (comp 0, lin 0, col 0, coeff 0): %d\n", return_value);

    num = compute_num_coeff(img);
    sprintf(histo_name,"histo_cover_%s.csv",argv[1]);
    return_value = compute_histo(img, num, histo_name, tab_cover);
    return_value = change_image_coeff(img, atof(argv[3]), argv[2], num);
    sprintf(histo_name,"histo_stega_%s.csv",argv[1]);
    return_value = compute_histo(img, num, histo_name, tab_stega);

    for(i = 0; i < 200; i+=2)
    {
        if(tab_stega[i] > tab_stega[i+1])
            diff = tab_stega[i] - tab_stega[i+1];
        else
            diff = tab_stega[i+1] - tab_stega[i];
        average_diff += diff;
        if(main_diff < diff)
            main_diff = diff;
    }
    printf("sdiff : %d",average_diff/200);
    main_diff = 0;
    average_diff = 0;

    for(i = 0; i < 200; i+=2)
    {
        if(tab_cover[i] > tab_cover[i+1])
            diff = tab_cover[i] - tab_cover[i+1];
        else
            diff = tab_cover[i+1] - tab_cover[i];
        average_diff += diff;
        if(main_diff < diff)
            main_diff = diff;
    }
    printf(" cdiff : %d\n",average_diff/200);

	free_jpeg_img(img);
	return EXIT_SUCCESS;
}
