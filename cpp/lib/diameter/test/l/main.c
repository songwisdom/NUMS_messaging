#include "stdio.h"
#include "gwlib.h"


int main (int argc, char **argv)
{
	gwlib_init();
	float a, b, c, d;
	int tmp;

	a = 1.99;
	Octstr *os;

	tmp = htonl(*(int *)&a);
	*(int *)&c = ntohl(tmp);

	printf("%f vs %f sizeof(%ld)\n", c, a, sizeof(a));

	/*

	os = octstr_create("");
	append_encoded_integer(os, a, 4);


	b = (float)decode_integer(os, 0, 4);

	printf("%f\n", b);
	*/

	gwlib_shutdown();
}
