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
	


	#pragma omp parallel firstprivate(tmp_sum) shared(array, sum)
	{

		int j;

        #pragma omp for reduction(+:sum)
		for(j=0; j<size; j++)
		{
			tmp_sum += array[j];
			sum += array[j];
		}
#if PRAGMA_FOR
        #pragma omp for
		for(j=0; j<size; j++)
		{
			tmp_sum += array[j];
		}
        #pragma omp atomic
		sum += tmp_sum;
#endif
#if RANG_CHECK
        int rang = omp_get_thread_num();
		for(j = rang * 16; j < (rang + 1)*16; j++)
		{
			tmp_sum += array[j];
		}
        #pragma omp atomic
		sum += tmp_sum;
#endif


		for(j=0; j<nb_threads; j++)
		{		
			if(omp_get_thread_num() == j)
			{	
				printf("tmp_sum = %d \n", tmp_sum);
			}
			#pragma omp barrier
		}
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
