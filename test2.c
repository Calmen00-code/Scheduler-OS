#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

void get_input( void *arg )
{
    char *str = NULL;
    str = (char*)arg; 
    printf("INPUT: ");
    scanf("%s", str);
}

int main()
{
    char buffer1[1][100];
    get_input( buffer1[0] );
    printf("%s\n", buffer1[0]);
    return 0;
}
