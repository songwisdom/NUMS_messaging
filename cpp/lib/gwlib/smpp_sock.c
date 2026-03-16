#include <errno.h>

#include "gwlib.h"
#include "smpp_sock.h"
#include "smpp_pdu.h"

int smpp_pdu_read_len(Connection *conn)
{
	Octstr *os;
	unsigned char buf[4];    /* The length is 4 octets. */
	int len;

	os = conn_read_fixed(conn, sizeof(buf));
	if (os == NULL)
		return 0;

	octstr_get_many_chars((char*) buf, os, 0, sizeof(buf));
	octstr_destroy(os);
	len = decode_network_long(buf);
	if (len < MIN_SMPP_PDU_LEN) {
		error(0, "SMPP: PDU length was too small (%d, minimum is %d).", len, MIN_SMPP_PDU_LEN);
		return -1;
	}
	if (len > MAX_SMPP_PDU_LEN) {
		error(0, "SMPP: PDU length was too large (%d, maximum is %d).", len, MAX_SMPP_PDU_LEN);
		return -1;
	}
	return len;
}


Octstr *smpp_pdu_read_data(Connection *conn, int len)
{
	Octstr *os;

	os = conn_read_fixed(conn, ((len - 4) > 0 ? (len - 4) : 0));    /* `len' includes itself. */

	return os;
}

int send_smpp_pdu(Connection *conn, SMPP_PDU *pdu)
{
	int ret;

	Octstr *os = smpp_pdu_pack(pdu);

	ret = conn_write(conn, os);   /* Caller checks for write errors later */

	if ( ret == -1 ) {
		error(errno, "conn_write failed");
		ret = RET_CONN_FAIL;
	}else {
		ret = RET_CONN_SUCCESS;
	}

	octstr_destroy(os);

	return ret;
}

int read_smpp_pdu(Connection *conn, int *len, SMPP_PDU **pdu)
{ 
	Octstr *os; 

	*len = smpp_pdu_read_len(conn); 
	if (*len == -1) { 
		error(0, "[%s : %d]: PDU smpp_pdu_read_len failed.", __func__, __LINE__);
		return RET_CONN_FAIL; 
	} else if (*len == 0) { 
		if (conn_eof(conn) || conn_error(conn)) {
			return RET_CONN_FAIL; 
		}
		return RET_CONN_TIME_OUT; 
	} 

	os = smpp_pdu_read_data(conn, *len); 
	if (os == NULL) { 
		if (conn_eof(conn) || conn_error(conn))  {
			error(0, "[%s : %d]: PDU smpp_pdu_read_data failed.", __func__, __LINE__);
		}else {
			error(0, "[%s : %d]: PDU smpp_pdu_read_data timeout failed.", __func__, __LINE__);
		}
		return RET_CONN_FAIL; 
	} 

	*pdu = smpp_pdu_unpack(os);
	octstr_destroy(os);

	return RET_CONN_SUCCESS;
}

int wait_socket_event(Connection *conn, int tm_out)
{
	int ret;
	
	ret = conn_wait(conn, (double) tm_out);
	
	switch (ret) {
		case 0 :
			if ( conn_eof(conn) || conn_error(conn) ) {
				error(errno, "[%s : %d] conn_wait failed : sesession error ", __func__, __LINE__);
				return RET_CONN_FAIL;
			}else {
				return RET_CONN_SUCCESS;
			}
		case 1 : return RET_CONN_TIME_OUT;
		default : 
			 error(errno, "[%s : %d] conn_wait failed reurn (%d)", __func__, __LINE__, ret);
			 return RET_CONN_FAIL;
	}
}

int read_smpp_pdu_timeout(Connection *conn, int *len, SMPP_PDU **pdu, int tm_out)
{
	int ret = 0;

	if ( conn_inbuf_len(conn) == 0 ) {
		ret = wait_socket_event(conn, tm_out);
	
		if ( ret == RET_CONN_FAIL ) {
			return ret;
		}
	}

	ret =  read_smpp_pdu(conn, len, pdu);
	
	return ret;
}
