#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "gwlib.h"
#include "watch_dog.h"

extern int errno;

static Connection *watch_conn;
static Counter *watch_counter;
static Octstr *watch_office;
static int watch_pid;
static int watch_ppid;
static time_t watch_time;
static FDSet *watch_fdset;

WATCH_DOG_ST *watch_dog_decode(Octstr *packet)
{
	int pos = 0;

	WATCH_DOG_ST *data = gw_malloc(sizeof(WATCH_DOG_ST));

	data->command = decode_integer(packet, pos, sizeof(data->command));
	pos += sizeof(data->command);

	data->sequence = (u_int)decode_integer(packet, pos, sizeof(data->sequence));
	pos += sizeof(data->sequence);

	data->pid = decode_integer(packet, pos, sizeof(data->pid));
	pos += sizeof(data->pid);

	data->ppid = decode_integer(packet, pos, sizeof(data->ppid));
	pos += sizeof(data->ppid);

	data->status_time = (u_int)decode_integer(packet, pos, sizeof(data->status_time));
	pos += sizeof(data->status_time);

	octstr_get_many_chars(data->office, packet, pos, sizeof(data->office));

	/*
	dbg("[%s] command[%d] seq[%u] pid[%d] ppid[%d] status_time[%u] office[%s]",
			__func__,
			data->command, data->sequence, data->pid, data->ppid, data->status_time, data->office);

			*/
	return data;
}

Octstr *watch_dog_encode(int cmd, u_int seq, int pid, int ppid, time_t state, char *office)
{

	char *buf = gw_malloc(WATCH_DOG_OFFICE_SIZE+1);
	Octstr *packet = octstr_create("");

	memset(buf, 0x00, WATCH_DOG_OFFICE_SIZE);
	strncpy(buf, office, WATCH_DOG_OFFICE_SIZE);

	append_encoded_integer(packet, (u_int)cmd, sizeof(int));
	append_encoded_integer(packet, seq, sizeof(int));
	append_encoded_integer(packet, (u_int)pid, sizeof(int));
	append_encoded_integer(packet, (u_int)ppid, sizeof(int));
	append_encoded_integer(packet, (u_int)state, sizeof(int));
	octstr_append_data(packet, buf, WATCH_DOG_OFFICE_SIZE);

	/*
	dbg("[%s] command[%d] seq[%u] pid[%d] ppid[%d] status_time[%u] office[%s]",
			__func__, cmd, seq, pid, ppid, state, office);
			*/
			
	gw_free(buf);
	return packet;
}

void watch_dog_callback(Connection *conn, void *arg)
{
	bool ret = true;
	Octstr *resp;
	WATCH_DOG_ST *packet;

	//info(0, "[%s] start", __func__);
	resp = conn_read_fixed(watch_conn, sizeof(WATCH_DOG_ST));
	if ( resp == NULL ) {
		if ( conn_eof(conn) || conn_error(conn) ) {
			error(errno, "[%s] conn_read_fixed failed : conn_eof(%d) conn_error(%d)", __func__, conn_eof(conn), conn_error(conn));
			ret = false;
		}
	}else {
		packet = watch_dog_decode(resp);
		if (packet == NULL ) {
			error(0, "watch_dog_decode failed");
		}


		/*
		dbg("[%s] command[%d] seq[%u] pid[%d] ppid[%d] status_time[%u] office[%s]", __func__,
				packet->command, packet->sequence, packet->pid, packet->ppid, packet->status_time, packet->office);
				*/

		switch ( packet->command ) {
			case WATCH_DOG_CMD_DEREGISTER :
				warning(0, "[WATCH_DOG] MASTER de-register incomming : i'm shutdown");
				ret = false;
				break;
			case WATCH_DOG_CMD_PING :
				resp = watch_dog_encode( WATCH_DOG_CMD_PONG, (u_int)packet->sequence, watch_pid, watch_ppid, watch_time, octstr_get_cstr(watch_office));
				//dbg("send WATCH_DOG_CMD_PEER_PONG SEQ[%u] PID[%u] Time[%lu]", packet->sequence, watch_pid, watch_time);
				if ( conn_write(conn, resp) == -1 ) {
					error(errno, "[WATCH_DOG] Ping-Ping PACKET SEND FAILED");
					ret = false;
				}else {
					if ( conn_eof(conn) || conn_error(conn) ) {
						error(errno, "[%s] conn_eof(%d) conn_error(%d)", __func__, conn_eof(conn), conn_error(conn));
						ret = false;
					}else {
						conn_flush(conn);
					}
				}

				break;
			case WATCH_DOG_CMD_PONG :
				break;
			case WATCH_DOG_CMD_PEER_REGISETER :
			case WATCH_DOG_CMD_PEER_DEREGISETER :
			case WATCH_DOG_CMD_PEER_PING :
			case WATCH_DOG_CMD_PEER_PONG :
			case WATCH_DOG_CMD_ROK :
			case WATCH_DOG_CMD_REGISTER : 
			default :
				break;
		}
	}


	if  ( resp != NULL ) {
		octstr_destroy(resp);
		resp = NULL;
	}


	gw_free(packet);


	if ( ret == false ) {
		watch_dog_stop();
	}

}

bool start_watch_dog(Octstr *host, int port, char *office)
{
	bool ret = true;
#if 0
	int rc;
	u_long seq;
	WATCH_DOG_ST *rdata = NULL;
	Octstr *resp = NULL;
	Octstr *req = NULL;

	watch_office = octstr_create(office);
	if ( watch_office == NULL ) {
		error(0, "[%s] office octstr_create failed office[%s]", __func__, office);
		return false;
	}
	
	if ( octstr_len(watch_office) > WATCH_DOG_OFFICE_SIZE ) {
		error(0, "[%s] office length max(%d) over [%s (len:%ld)]", 
				__func__, WATCH_DOG_OFFICE_SIZE, octstr_get_cstr(watch_office), octstr_len(watch_office));
		return false;
	}

	watch_conn = conn_open_tcp(host, port, NULL);
	if ( watch_conn == NULL ) {
		error(errno, "[%s] conn_open_tcp failed : ip[%s], port[%d]", __func__, octstr_get_cstr(host), port);
		return false;
	}

	watch_counter = counter_create();
	if ( watch_counter == NULL ) {
		error(0, "[%s] counter_create failed", __func__);
		goto result;
	}

	seq = counter_increase(watch_counter);
	watch_pid = getpid();
	watch_ppid = getppid();
	time(&watch_time);

	req = watch_dog_encode(WATCH_DOG_CMD_REGISTER, seq, watch_pid, watch_ppid, watch_time, octstr_get_cstr(watch_office));

	rc = conn_write(watch_conn, req);
	if ( rc == -1 ) {
		error(0, "[%s] send WATCH_DOG_CMD_REGISTER failed", __func__);
		ret = false;
		goto result;
	}

	rc = conn_wait(watch_conn, 10L);
	if ( rc != 0 ) {
		error(0, "[%s] WAIT PACKET EVENT FAILED", __func__);
		ret = false;
		goto result;
	}

	resp = conn_read_fixed(watch_conn, sizeof(WATCH_DOG_ST));
	if ( resp == NULL ) {
		error(0, "[%s] READ PACKET FAILED", __func__);
		ret = false;
		goto result;
	}

	rdata = watch_dog_decode(resp);

	if ( rdata->command == WATCH_DOG_CMD_ROK ) {
		watch_fdset = fdset_create();
		conn_register(watch_conn, watch_fdset, watch_dog_callback, NULL);
		ret = true;
	} else {
		error(0, "[%s] response cmd is not rok(%d)", __func__, rdata->command);
		ret = false;
	}

result :	

	if ( ret == false ) {
		if ( watch_conn != NULL ) {
			conn_destroy(watch_conn);
			watch_conn = NULL;
		}
	}
	if ( req != NULL )
		octstr_destroy(req);

	if ( resp != NULL )
		octstr_destroy(resp);

	if ( rdata != NULL ) 
		gw_free(rdata);

#endif
	return ret;
}

static void watch_dog_deregister()
{
	int rc;
	Octstr *req = NULL, *resp = NULL;
	WATCH_DOG_ST *data = NULL;

	if ( watch_conn == NULL )
		return;

	if ( watch_office == NULL ) {
		return;
	}
	
	if ( watch_counter == NULL ) {
		return;
	}

	u_long seq = counter_increase(watch_counter);

	req = watch_dog_encode(WATCH_DOG_CMD_DEREGISTER, (u_int)seq, (int)getpid(), (int)getppid(), time(NULL), octstr_get_cstr(watch_office));

	rc = conn_write(watch_conn, req);
	if ( rc == -1 ) {
		error(0, "[%s] send WATCH_DOG_CMD_REGISTER failed", __func__);
		goto result;
	}

	rc = conn_wait(watch_conn, 1.0);
	if ( rc != 0 ) {
		error(0, "[%s] WAIT PACKET EVENT FAILED", __func__);
		goto result;
	}

	resp = conn_read_fixed(watch_conn, sizeof(WATCH_DOG_ST));
	if ( resp == NULL ) {
		error(0, "[%s] READ PACKET FAILED", __func__);
		goto result;
	}

	data = watch_dog_decode(resp);

result :

	if ( req != NULL )
		octstr_destroy(req);
	if ( resp != NULL )
		octstr_destroy(resp);
	if ( data != NULL )
		gw_free(data);

}

void watch_dog_stop()
{
	return;
#if 0
	//info(0, "[WATCH_DOG] watch_dog_stop");

	watch_dog_deregister();

	if ( watch_office != NULL ) {
		octstr_destroy(watch_office);
		watch_office = NULL;
	}

	if ( watch_conn != NULL ) {
		if ( conn_get_id(watch_conn) > 0 )
			conn_destroy(watch_conn);
		watch_conn = NULL;
	}

	if ( watch_counter != NULL ) {
		counter_destroy(watch_counter);
		watch_counter = NULL;
	}

	/*
	if ( watch_fdset != NULL ) {
		fdset_destroy(watch_fdset);
		watch_fdset = NULL;
	}
	*/
#endif

}

void watch_dog_status_update()
{
#if 0
	time(&watch_time);
#endif
}
