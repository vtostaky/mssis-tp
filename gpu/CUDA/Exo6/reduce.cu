#include <stdlib.h>
#include <stdio.h>
#include <math.h>


__global__ void reduce_kernel(float *in, float *out, int ntot)
{
    int i = blockIdx.x;
    int j = i * blockDim.x + threadIdx.x;
 
    if(j < ntot)
        out[i] += in[j];
}

__host__ void init_vec(float *h_in, int ntot)
{
    for(int i = 0 ; i < ntot ; i++)
    {
			h_in[i] = sinf(float(i));
    }
}

__host__ void verif(float sum, float *h_in, int ntot)
{
    float sum_res = 0.;
    for(int i = 0 ; i < ntot ; i++)
    {
			sum_res += h_in[i];
    }
    float err = fabsf((sum - sum_res)/sum);
    printf("GPU sum : %.4e\n", sum);
    printf("CPU sum : %.4e\n", sum_res);
    if (err < 1.e-4)
    {
			printf("TEST PASSED (err %.4e < 1.e-4).\n", err);
    }
 	  else
    {
			printf("TEST FAILED (err %.4e > 1.e-4).\n", err);
    }
}

int main(int argc, char **argv)
{
    float sum;
    int nthreads, nblocks, ntot;
    int i, j;

    nthreads = 128;
    ntot = atoi(argv[1]);
    nblocks = (ntot + nthreads - 1) / nthreads;

    printf("Ntot     : %d\n", ntot);
    printf("nthreads : %d\n", nthreads);
    printf("nblocks  : %d\n", nblocks);

    float *d_sum, *d_bl, *d_in, *h_in, *h_bl;

    h_in = (float*)malloc(ntot*sizeof(float));
    h_bl = (float*)malloc(nblocks*sizeof(float));

    cudaMalloc((void**)&d_sum, sizeof(float));
    cudaMalloc((void**)&d_bl, nblocks*sizeof(float));
    cudaMalloc((void**)&d_in, ntot*sizeof(float));

    init_vec(h_in, ntot);
    for(j = 0; j < ntot; j++)
        printf("|%f",h_in[j]);
    cudaMemcpy(d_in, h_in, ntot*sizeof(float), cudaMemcpyHostToDevice);

    // TODO : la réduction de d_in a lieu ici, le resultat est obtenu dans *d_sum
    dim3 dimGrid(nblocks,1,1);
    dim3 dimBlock(nthreads,1,1);
    reduce_kernel<<<dimGrid, dimBlock>>>(d_in, d_bl, ntot);

    cudaMemcpy(h_bl, d_bl, sizeof(float), cudaMemcpyDeviceToHost);

    for(i = 0; i < nblocks; i++)
        printf("Block %d : ",i);
        for(j = 0; j < ntot / nblocks; j++)
            printf("|%f",h_in[i*nblocks+j]);
        printf(" - result %f\n",h_bl[i]);
    
    //cudaMemcpy(&sum, d_sum, sizeof(float), cudaMemcpyDeviceToHost);
    
    //verif(sum, h_in, ntot);

    cudaFree(d_sum);
    cudaFree(d_bl);
    cudaFree(d_in);
    free(h_in);

    return 0;
}

