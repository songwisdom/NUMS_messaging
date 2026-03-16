#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "gwlib.h"

int main (int argc, char **argv)
{
	gwlib_init();
	FLK_INI_ST *ini;
	ini = flkini_open("/home/lmsgw/config/lmsgw.ini");
	if ( ini == NULL ) {
		error(0, "flkini_open failed");
	}

	int tmp;
	bool ret;

	ret = flkini_get_hex_integer(ini, "RECTDB", "IPC_KEY_BASE", &tmp);
	if ( ret == false  ) {
		error(0, "flkini_get_integer failed");
	}else {
		info(0, "IPC_KEY_BASE [%d] [%08x]", tmp, tmp);
	}

	gwlib_shutdown();
	exit(0);
}
