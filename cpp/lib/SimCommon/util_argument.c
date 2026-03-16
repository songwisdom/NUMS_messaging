#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "flk_type.h"

static char * Argument;

int set_argument ( char * buffer )
{
	if ( buffer == NULL )
		return FLK_FAIL;

	Argument = buffer;
	return FLK_SUCCESS;
}


