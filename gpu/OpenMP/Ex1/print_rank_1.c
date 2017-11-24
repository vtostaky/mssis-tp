#include <omp.h>

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char * argv[])
{
	int my_rank = -1;
	int nb_threads = -1;

    #pragma omp parallel private(my_rank)
    {
	    my_rank = omp_get_thread_num();
	    nb_threads = omp_get_num_threads();
        #pragma omp barrier
	    printf("I am thread %d (for a total of %d threads)\n", my_rank, nb_threads);
    }
	return 0;
	
}
