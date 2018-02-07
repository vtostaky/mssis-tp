//gcc breaker.c -o breaker -lcrypt && ./breaker


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>


/*
struct keypair{
	int sum;
	int mul;
};
typedef struct keypair s_keypair;

*/

typedef int s_keypair;
s_keypair get_keypair(char* tab, int length, int first_char, int last_char){

	int sum=0;
	int mul=0;
	int pow=1;

	int i=0;
	for(i=0; i<length; i++)
	{
		sum+=tab[i]*pow;
		pow*=(last_char - first_char);	
	}

	return sum;
}


int check_keypairs(s_keypair crypt, s_keypair test)
{
	if(crypt == test)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



unsigned long search_all_1( s_keypair crypted, int length, int first_char, int last_char ){
	int loop_size = last_char - first_char;
	int max_iter = powl(loop_size, length);
	char *tab = (char*)malloc(sizeof(char)*(length+1));
	tab[length]='\0';
	int j;
	for(j=0; j<length; j++) tab[j] = first_char;

	int i;
	printf("max_iter = %lu \n", (unsigned long) max_iter);	

	s_keypair current_keypair;
	int pow=0;	

	for(i=0; i<max_iter; i++)
	{
	
              	pow=1;
                for(j=0; j<length; j++)
                {
                        tab[j] = ((i/pow) % loop_size) + first_char;
                        pow*=loop_size;
                }


		current_keypair = get_keypair(tab, length, first_char, last_char);
		if( check_keypairs(crypted, current_keypair) ) {
			printf( "password found: %s\n", tab );
			return (unsigned long)i;
		}	
				
	}	

}


int main( int argc, char** argv ) {
	char* password; 
	int first_char, last_char;
	float t1, t2; 
	unsigned long cmp;

	if( argc == 1 ) {
		password = "A$4c";
		first_char = 32;
		last_char = 126;
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
	s_keypair crypted = get_keypair(password, strlen(password), first_char, last_char);

	printf( "*running parameters*\n" );
	printf( " -password length:\t%d digits\n", strlen(password) );
	printf( " -digits:\t\tfrom -%c- to -%c-\n", first_char, last_char );
	printf(	" -crypted to break:\t%d\n", crypted );

	t1 = clock();
	cmp = search_all_1( crypted, strlen( password ), first_char, last_char );
	t2 = clock();

	float period = (t2-t1)/CLOCKS_PER_SEC;
	if( period < 60 )
		printf( "time: %.1fs \n", period );
	else
		printf( "time: %.1fmin \n", period/60 );
	printf( "#tries: %lu\n", cmp );
	printf( "=> efficiency: %.f tries/s\n", (float)cmp/period );

	return EXIT_SUCCESS;
}
