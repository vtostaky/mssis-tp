//gcc breaker.c -o breaker -lcrypt && ./breaker


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>


#define MAX_SIZE 256

//Performance:
#define NB_BLOCKS 64
#define NB_THREADS_PER_BLOCK 128
#define MAX_ITER 200

__device__ void increment_brute_force(unsigned char* tab, int length, int loop_size, int index)
{
    int j;
    for(j=0; j<length; j++)
    {
        int add = index + tab[j];
        tab[j] = add % loop_size;
        index = add / loop_size;
    }
}

__device__ int get_keypair(unsigned char* tab, int length, int first_char, int last_char){

	int sum=0;
	int pow=1;

	int i=0;
	for(i=0; i<length; i++)
	{
		sum+=(tab[i]+first_char)*pow;
		pow*=(last_char - first_char);	
	}

	return sum;
}

__global__ void breaker(int numThreads, int length, int first_char, int last_char, int crypt, unsigned char* d_brute, unsigned char* d_password)
{
	//compute our index number
    uint idx = (blockIdx.x*blockDim.x + threadIdx.x);

	unsigned char tab[MAX_SIZE];
    int loop_size = last_char - first_char;
    int current_keypair;
	int i,j;

	for(j = 0; j < length; j++)
	{
		tab[j] = d_brute[j];
	}
	
	increment_brute_force(tab, length, loop_size, idx);
	for(i = 0; i < MAX_ITER; i++)
	{
        //get keypair
		current_keypair = get_keypair(tab, length, first_char, last_char);

		//compare keypairs
		if( crypt == current_keypair )
		{
			//save correct password
			for(j= 0; j < MAX_SIZE; j++)
			{
				d_password[j] = tab[j]+first_char;
			}
            d_password[length] = 0;
		}
		increment_brute_force(tab, length, loop_size, numThreads);
    }
}

__host__ bool increment_brute_force_host(unsigned char* tab, int length, int loop_size, int index)
{
    int j;
    for(j=0; j<length; j++)
    {
        int add = index + tab[j];
        tab[j] = add % loop_size;
        index = add / loop_size;
    }

    return index != 0; 
}

__host__ int get_keypair_host(unsigned char* tab, int length, int first_char, int last_char){

	int sum=0;
	int pow=1;

	int i=0;
	for(i=0; i<length; i++)
	{
		sum+=tab[i]*pow;
		pow*=(last_char - first_char);	
	}

	return sum;
}

int main( int argc, char** argv ) {
    char *password; 
	int first_char, last_char, length;
	float t1, t2; 

	int numThreads = NB_BLOCKS * NB_THREADS_PER_BLOCK;
	int k = 0;

	unsigned char h_brute[MAX_SIZE];
	unsigned char h_password[MAX_SIZE];
    unsigned char *d_brute;
    unsigned char *d_password;
	int crypted;
    bool end = false;
	
    if( argc == 1 ) {
		password = "A$4c";
		first_char = 32;
		last_char = 126;
        length = 4;
		/* ---ASCII values---
		 * special characters: 	32 to 47
		 * numbers: 		48 to 57
		 * special characters: 	58 to 64
		 * letters uppercase: 	65 to 90
		 * special characters: 	91 to 96
		 * letters lowercase: 	97 to 122
		 * special characters: 	123 to 126
		 * */
	} else if( argc == 4 ) {
        length = strlen(argv[1]);
        if(length > MAX_SIZE)
        {
            printf( "Too long password, shortened to MAX_SIZE %d.\n", MAX_SIZE);
            length = MAX_SIZE;
        }
		password = argv[1];
		first_char = atoi( argv[2] );
		last_char = atoi( argv[3] );
	} else {
		printf("usage: breaker <password> <first_ch> <last_ch>\n");
		printf("default: breaker A$4c 32 126\n");
		printf("exemple to break the binary password 1101000:\n");
		printf( "breaker 1101000 48 49\n" );
		exit( 0 );
	}

	crypted = get_keypair_host((unsigned char*)password, length, first_char, last_char);

    for(k = 0; k < MAX_SIZE; k++)
    {
        h_brute[k] = 0;
        h_password[k] = 0;
    }

	printf( "*running parameters*\n" );
	printf( " -password length:\t%d digits\n", length );
	printf( " -digits:\t\tfrom -%c- to -%c-\n", first_char, last_char );
	printf(	" -crypted to break:\t%d\n", crypted );

	t1 = clock();
    
    //d_password : the table used by the kernel to store retrieved password 
    cudaMalloc((void**)&d_password, MAX_SIZE*sizeof(unsigned char));
    cudaMalloc((void**)&d_brute, MAX_SIZE*sizeof(unsigned char));

    cudaMemcpy(d_password, h_password, MAX_SIZE, cudaMemcpyHostToDevice);

    //
	do{
		cudaMemcpy(d_brute, h_brute, MAX_SIZE, cudaMemcpyHostToDevice);
		
		dim3 dimGrid(NB_BLOCKS);
        dim3 dimBlock(NB_THREADS_PER_BLOCK);

        //Start breaker loop : each kernel will perform MAX_ITER password checks
		breaker<<<dimGrid, dimBlock>>>(numThreads, length, first_char, last_char, crypted, d_brute, d_password);

		//look for a matching password
		cudaMemcpy(&h_password, d_password, MAX_SIZE, cudaMemcpyDeviceToHost);

		if(h_password[0] != 0)
		{
			printf("\n\nFOUND password: ");
            k = 0;
			while(h_password[k] != 0)
			{
				printf("%c", h_password[k]);
#if EXHAUSTIVE_SEARCH                
                //The following can be performed in case we want to retrieve all matching passwords
                h_password[k] = 0;
#endif
				k++;
			}

			printf("\n");

#if EXHAUSTIVE_SEARCH
            //If exhaustive search, reinitialize password tables
            cudaMemcpy(d_password, h_password, MAX_SIZE, cudaMemcpyHostToDevice);
#else
            //If not exhaustive search, stop research at first match
            goto exit;
#endif
		}

        //Increment the table used for brute force, as numThreads*MAX_ITER have already been checked
		end = increment_brute_force_host(h_brute, length, last_char-first_char, numThreads * MAX_ITER);

	} while(!end);

exit:
    t2 = clock();

	float period = (t2-t1)/CLOCKS_PER_SEC;
	if( period < 60 )
		printf( "time: %.1fs \n", period );
	else
		printf( "time: %.1fmin \n", period/60 );

	return EXIT_SUCCESS;
}
