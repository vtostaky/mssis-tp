#include <stdlib.h>
#include <stdio.h>
#include <math.h>


__global__ void reduce_kernel(float *in, float *out, int ntot)
{
    // TODO : coder ici
     int nthreads = 1;
     int totthreads = blockDim.x;
     int test = 2;
     int index = blockIdx.x * blockDim.x + threadIdx.x;
     while(nthreads!=totthreads)
     {
	if((index % test == 0) && (index + nthreads < ntot))
        {
          in[index] = in[index] + in[index + nthreads];  
        }
        __syncthreads();
        nthreads = nthreads * 2;
        test = test * 2;
     }
  
   out[blockIdx.x] = in[blockIdx.x * blockDim.x];
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

    nthreads = 128;
    ntot = atoi(argv[1]);
    nblocks = (ntot + nthreads - 1) / nthreads;

    printf("Ntot     : %d\n", ntot);
    printf("nthreads : %d\n", nthreads);
    printf("nblocks  : %d\n", nblocks);

    float *d_sum, *d_bl, *d_in, *h_in;

    h_in = (float*)malloc(ntot*sizeof(float));

    cudaMalloc((void**)&d_sum, sizeof(float));
    cudaMalloc((void**)&d_bl, nblocks*sizeof(float));
    cudaMalloc((void**)&d_in, ntot*sizeof(float));

    init_vec(h_in, ntot);
    cudaMemcpy(d_in, h_in, ntot*sizeof(float), cudaMemcpyHostToDevice);

    // TODO : la réduction de d_in a lieu ici, le resultat est obtenu dans *d_sum
    dim3 block(nblocks,1,1);
    dim3 thread(nthreads,1,1);
    dim3 unique(1,1,1);

    reduce_kernel<<<block, thread>>>(d_in, d_bl, ntot);
    
    reduce_kernel<<<unique,block>>>(d_bl, d_sum, ntot);

    cudaMemcpy(&sum, d_sum, sizeof(float), cudaMemcpyDeviceToHost);
    
    verif(sum, h_in, ntot);

    cudaFree(d_sum);
    cudaFree(d_bl);
    cudaFree(d_in);
    free(h_in);

    return 0;
}

