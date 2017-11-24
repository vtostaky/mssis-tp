//gcc breaker.c -o breaker -lcrypt && ./breaker


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <time.h>


unsigned long cmp = 0;


int carry( char* tab, int i, int first_char, int last_char ) {
	if( i==0 )
		return 0;
	else
		if( tab[i-1] < last_char ) {
			tab[i-1] = tab[i-1]+1;
			return 1;
		} else {
			tab[i-1] = first_char;
			return carry( tab, i-1, first_char, last_char );
		}
}


int search_one( char* crypted, char* tab ) {
	cmp++;
	if( !strcmp( crypted, crypt( tab, "salt" ) ) ) {
		printf( "password found: %s\n", tab );
		return 1;
	} else
		return 0;
}


void search_all( char* crypted, int length, int first_char, int last_char ){
	int i, loop=1;
	char* tab = (char*) malloc( (length+1)*sizeof(char) );
	for(i=0; i<length; i++) tab[i] = first_char;

	tab[length] = '\0';
	printf( "let's break it...\n" );
	loop = !search_one( crypted, tab );
	i=length-1;
	while( loop ) {
		if( tab[i] < last_char ) {
			tab[i] = tab[i]+1;
			loop = !search_one(crypted, tab );
		} else {
			tab[i] = first_char;
			if( loop = carry( tab, i, first_char, last_char ) )
				loop = !search_one( crypted, tab );
		}
	}
}


int main( int argc, char** argv ) {
	char* password; 
	int first_char, last_char;
	float t1, t2; 

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
	char* crypted0 = crypt( password, "salt" );
	char* crypted = (char*) malloc( (strlen(crypted0)+1)*sizeof(char) );
	strcpy( crypted, crypted0 );

	printf( "*running parameters*\n" );
	printf( " -password length:\t%d digits\n", strlen(password) );
	printf( " -digits:\t\tfrom -%c- to -%c-\n", first_char, last_char );
	printf(	" -crypted to break:\t%s\n", crypted );

	t1 = clock();
	search_all( crypted, strlen( password ), first_char, last_char );
	t2 = clock();

	float period = (t2-t1)/CLOCKS_PER_SEC;
	if( period < 60 )
		printf( "time: %.1fs \n", period );
	else
		printf( "time: %.1fmin \n", period/60 );
	printf( "#tries: %d\n", cmp );
	printf( "=> efficiency: %.f tries/s\n", (float)cmp/period );

	return EXIT_SUCCESS;
}
