#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	int nb_threads=-1;
	#pragma omp parallel
	{
		nb_threads = omp_get_num_threads();	
	}


	int size = nb_threads*16;
	int * array = (int *)malloc(sizeof(int)*size);
	int i;
	for(i=0; i<size; i++) array[i] = i+1;

	int sum=0;
	int verif_sum=0;
	int tmp_sum = 0;

	for(i=0; i<size; i++) 
	{
		verif_sum += array[i];
	}
	


	#pragma omp parallel for reduction(+:sum)
	for(i=0; i<size; i++)
	{
		sum += array[i];
	}

	if(sum == verif_sum)
	{
		printf("OK! sum = verif_sum!\n");
	}
	else
	{
		printf("Error! sum != verif_sum! (sum = %d ; verif_sum = %d)\n", sum, verif_sum); 
	}
		

	free(array);



	return 0;
}
