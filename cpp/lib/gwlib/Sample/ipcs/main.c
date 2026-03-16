#include "gwlib.h"
#include "flkipc.h"
#include "errno.h"

extern int errno;

int main (int argc, char **argv)
{
	gwlib_init();

	int id;
	bool ret;

	id = queue_get_msgqid((key_t)0xaa0000bb);

	char buff[16]= {0x00, };
	long mtype = 1;

	strcpy(buff, "hello");

	ret = queue_send(id, mtype, buff, sizeof(buff));
	if ( ret == false ) {
		error(0, "queue_send failed");
		goto result;
	}

	info(0, "SEND [%ld] [%s]", mtype, buff);

	memset(buff, 0x00, sizeof(buff));
	mtype = 1;

	ret = queue_read(id, &mtype, buff, sizeof(buff));
	if ( ret == false ) {
		error(0, "queue_read failed");
		goto result;
	}

	info(0, "RECV [%ld] [%s]", mtype, buff);

result :
	gwlib_shutdown();
	return 0;
}
