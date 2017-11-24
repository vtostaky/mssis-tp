#include <omp.h>
#include <stdio.h>


int main(){

    float a = 92000.;
    int n = 12;

#pragma omp parallel default(none) \
    firstprivate(a)
    {
        int n = 5;
        a = a + 290.;
        printf("Values %d, %f from thread %d\n",n,a,omp_get_thread_num());
    }

    printf("End of prog, outside parallel region, value %d,%f\n",n,a);
    return 0;
}
