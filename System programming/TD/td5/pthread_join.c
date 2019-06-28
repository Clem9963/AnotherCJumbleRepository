#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void * f ( void * arg )
{
	int * x = malloc( sizeof( int ) );

	if ( x == NULL )
	{
		perror( "malloc" );
		exit( -1 );
	} 	

	*x = 12;

	return (void *) x;
}

int main( void )
{
	pthread_t id;
	int * ret;

	pthread_create( &id, NULL, f, NULL );

	pthread_join( id, (void **)&ret );

	printf( "*ret = %d", *ret );

	printf( "%ld %ld\n", sizeof(int), sizeof(int *));

	return 0;
}