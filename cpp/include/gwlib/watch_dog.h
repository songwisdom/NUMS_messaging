#ifndef _H_FLK_WATCH_DOG_
#define _H_FLK_WATCH_DOG_

#include "gwlib.h"
#define WATCH_DOG_OFFICE_SIZE 8

typedef enum {
	// RESPONSE
	WATCH_DOG_CMD_ROK,
	WATCH_DOG_CMD_NACK,

	// REQUEST
	WATCH_DOG_CMD_REGISTER,
	WATCH_DOG_CMD_DEREGISTER,

	WATCH_DOG_CMD_PING,
	WATCH_DOG_CMD_PONG,

	WATCH_DOG_CMD_PEER_REGISETER,
	WATCH_DOG_CMD_PEER_DEREGISETER,
	WATCH_DOG_CMD_PEER_PING,
	WATCH_DOG_CMD_PEER_PONG,

}WATCH_DOG_PACKET_CMD_ENUM;

typedef struct {
	int command;
	int sequence;
	int pid;
	int ppid;
	int status_time;
	char office[WATCH_DOG_OFFICE_SIZE];
}WATCH_DOG_ST;

WATCH_DOG_ST *watch_dog_decode(Octstr *packet);
Octstr *watch_dog_encode(int cmd, int seq, int pid, int ppid, time_t state, char *office);
bool start_watch_dog(Octstr *host, int port, char *office);
void watch_dog_stop();
void watch_dog_status_update();
#endif

