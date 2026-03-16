#include "flk_dia.h"
#include "smpp_sock.h"

DIA_INTF_PDU *dia_intf_create(u_int type, u_int seq_no)
{
    DIA_INTF_PDU *pdu;

    pdu = (DIA_INTF_PDU *)gw_malloc(sizeof(*pdu));
    pdu->type = type;

    switch (type) {
    #define INTEGER(name, octets) p->name = 0;
    #define NULTERMINATED(name, max_octets) p->name = NULL;
    #define OCTETS(name, field_giving_octetst) p->name = NULL;
    #define DIA_PDU(name, id, fields) \
        case id: { \
            struct name *p = &pdu->u.name; \
            pdu->type_name = #name; \
            fields \
            p->command_id = type; \
            p->sequence_number = seq_no; \
        } break;
    #include "dia_intf.def"
    default:
        error(0, "Unknown DIA_INTF_PDU type, internal error.");
        gw_free(pdu);
        return NULL;
    }

    return pdu;
}
DIA_INTF_PDU *dia_intf_resp_pdu_create(DIA_INTF_PDU *pdu)
{
	DIA_INTF_PDU *resp;
	if ( pdu == NULL ) {
		error(0, "[%s : %d] pdu is null", __func__, __LINE__);
		return NULL;
	}

	if ( (pdu->type & 0x80000000) == 0x80000000 ) {
		warning(0, "[%s : %d] command id invalid [%08x]", __func__, __LINE__, pdu->type);
		return NULL;
	}

	resp = dia_intf_create((pdu->type | 0x80000000), 0);
	if ( resp == NULL ) {
		error(0, "[%s : %d] dia_intf_create pdu->type(0x%08x) sequence_number(%u) failed", 
				__func__, __LINE__, pdu->type, 0);
		return NULL;
	}

	if ( pdu->type == dia_forward_sm ) {
		resp->u.dia_forward_sm_resp.source_host = octstr_duplicate(pdu->u.dia_forward_sm.dest_host);
		resp->u.dia_forward_sm_resp.dest_host = octstr_duplicate(pdu->u.dia_forward_sm.source_host);
	}

	return resp;

}

void dia_intf_destroy(DIA_INTF_PDU *pdu)
{
    if (pdu == NULL)
        return;

    switch (pdu->type) {
    #define INTEGER(name, octets) p->name = 0; /* Make sure "p" is used */
    #define NULTERMINATED(name, max_octets) octstr_destroy(p->name);
    #define OCTETS(name, field_giving_octets) octstr_destroy(p->name);
    #define DIA_PDU(name, id, fields) \
        case id: { struct name *p = &pdu->u.name; fields } break;
    #include "dia_intf.def"
    default:
        error(0, "Unknown DIA_INTF_PDU type, internal error while destroying.");
    }
    gw_free(pdu);
}


Octstr *dia_intf_pack(DIA_INTF_PDU *pdu)
{
    Octstr *os;
    Octstr *temp;

    os = octstr_create("");

    gw_assert(pdu != NULL);

    /*
     * Fix lengths of octet string fields.
     */
    switch (pdu->type) {
    #define INTEGER(name, octets) p = *(&p);
    #define NULTERMINATED(name, max_octets) p = *(&p);
    #define OCTETS(name, field_giving_octets) \
    	p->field_giving_octets = octstr_len(p->name);
    #define DIA_PDU(name, id, fields) \
    	case id: { struct name *p = &pdu->u.name; fields } break;
    #include "dia_intf.def"
    default:
    	error(0, "Unknown DIA_INTF_PDU type, internal error while packing.");
    }

    switch (pdu->type) {
    #define TL(name, octets) \
        append_encoded_integer(os, DIA_INTF_##name, 2); \
        append_encoded_integer(os, octets, 2);
    #define INTEGER(name, octets) \
        append_encoded_integer(os, p->name, octets);
    #define NULTERMINATED(name, max_octets) \
        if (p->name != NULL) { \
            if (octstr_len(p->name) >= max_octets) { \
                /*warning(0, "DIA_INTF: DIA_PDU element <%s> too long " \
                        "(length is %ld, should be %d)", \
                        #name, octstr_len(p->name), max_octets-1);*/ \
                temp = octstr_copy(p->name, 0, max_octets-1); \
            } else \
                temp = octstr_duplicate(p->name); \
            octstr_append(os, temp); \
            octstr_destroy(temp); \
        } \
        octstr_append_char(os, '\0');
    #define OCTETS(name, field_giving_octets) \
        if (p->name) octstr_append(os, p->name);
    #define DIA_PDU(name, id, fields) \
        case id: { struct name *p = &pdu->u.name; fields } break;
    #include "dia_intf.def"
    default:
        error(0, "Unknown DIA_INTF_PDU type, internal error while packing.");
    }

    temp = octstr_create("");
    append_encoded_integer(temp, octstr_len(os) + 4, 4);
    octstr_insert(os, temp, 0);
    octstr_destroy(temp);

    return os;
}


DIA_INTF_PDU *dia_intf_unpack(Octstr *data_without_len)
{
    DIA_INTF_PDU *pdu;
    u_int type;
    int len, pos;

    len = octstr_len(data_without_len);

    if (len < 4) {
        error(0, "DIA_INTF: DIA_PDU was too short (%ld bytes).",
              octstr_len(data_without_len));
        return NULL;
    }

    /* get the DIA_PDU type */
    if ((type = decode_integer(data_without_len, 0, 4)) == -1)
        return NULL;

    /* create a coresponding representation structure */
    pdu = dia_intf_create(type, 0);
    if (pdu == NULL)
        return NULL;

    pos = 0;

    switch (type) {
    #define INTEGER(name, octets) \
        if ((p->name = decode_integer(data_without_len, pos, octets)) == -1) \
            goto err; \
        pos += octets;
    #define NULTERMINATED(name, max_octets) \
        /* just warn about errors but not fail */ \
        copy_until_nul(#name, data_without_len, &pos, max_octets, &p->name);
    #define OCTETS(name, field_giving_octets) \
    	p->name = octstr_copy(data_without_len, pos, \
	    	    	      p->field_giving_octets); \
        if (p->field_giving_octets != (u_int) octstr_len(p->name)) { \
            error(0, "dia_intf: error while unpacking '" #name "', " \
                     "len is %ld but should have been %u, dropping.", \
                     octstr_len(p->name), p->field_giving_octets); \
            goto err; \
        } else { \
            pos += p->field_giving_octets; \
        }
    #define DIA_PDU(name, id, fields) \
    	case id: { struct name *p = &pdu->u.name; fields } break;
    #include "dia_intf.def"
    default:
    	error(0, "Unknown DIA_INTF_PDU type, internal error while unpacking.");
    }

    return pdu;
    
err:
    dia_intf_destroy(pdu);
    octstr_dump(data_without_len, 0);
    return NULL;
}


void dia_intf_dump(DIA_INTF_PDU *pdu)
{
    debug("dia_intf", 0, "DIA_INTF DIA_PDU %p dump:", (void *) pdu);
    debug("dia_intf", 0, "  type_name: %s", pdu->type_name);
    switch (pdu->type) {
    #define INTEGER(name, octets) \
        debug("dia_intf", 0, "  %s: %u = 0x%08x", #name, p->name, p->name);
    #define NULTERMINATED(name, max_octets) \
        octstr_dump_long(p->name, 2, #name);
    #define OCTETS(name, field_giving_octets) \
       octstr_dump_long(p->name, 2, #name);
    #define DIA_PDU(name, id, fields) \
        case id: { struct name *p = &pdu->u.name; fields } break;
    #include "dia_intf.def"
    default:
        error(0, "Unknown DIA_INTF_PDU type, internal error.");
        break;
    }
    debug("dia_intf", 0, "DIA_INTF DIA_PDU dump ends.");
}



/*
 * Return error string for given error code
 * NOTE: If you add new error strings here please use
 *       error strings from DIA_INTF spec. and please keep
 *       error codes in switch statement sorted by error
 *       code ID.
 */
const char *dia_intf_error_to_string(enum DIA_INTF_ERROR_MESSAGES error)
{
    switch (error) {
        case DIA_INTF_ROK:
            return "OK";
        case DIA_INTF_RINVMSGLEN:
            return "Message Length is invalid";
        case DIA_INTF_RINVCMDLEN:
            return "Command Length is invalid";
        case DIA_INTF_RINVCMDID:
            return "Invalid Command ID";
        case DIA_INTF_RINVBNDSTS:
            return "Incorrect BIND Status for given command";
        case DIA_INTF_RALYBND:
            return "ESME Already in Bound State";
        case DIA_INTF_RINVPRTFLG:
            return "Invalid Priority Flag";
        case DIA_INTF_RINVREGDLVFLG:
            return "Invalid Registered Delivery Flag";
        case DIA_INTF_RSYSERR:
            return "System Error";
        case DIA_INTF_RINVSRCADR:
            return "Invalid Source Address";
        case DIA_INTF_RINVDSTADR:
            return "Invalid Destination Address";
        case DIA_INTF_RBINDFAIL:
            return "Bind Failed";
        case DIA_INTF_RINVPASWD:
            return "Invalid Password";
        case DIA_INTF_RINVSYSID:
            return "Invalid System ID";
        case DIA_INTF_RCANCELFAIL:
            return "Cancel SM Failed";
        case DIA_INTF_RREPLACEFAIL:
            return "Replace SM Failed";
        case DIA_INTF_RMSGQFUL:
            return "Message Queue Full";
        case DIA_INTF_RINVSERTYP:
            return "Invalid Service Type";
        case DIA_INTF_RINVNUMDESTS:
            return "Invalid number of destinations";
        case DIA_INTF_RINVDLNAME:
            return "Invalid Distribution List Name";
        case DIA_INTF_RINVDESTFLAG:
            return "Destination flag is invalid";
        case DIA_INTF_RINVSUBREP:
            return "Submit w/replace not supported/allowed";
        case DIA_INTF_RINVESMCLASS:
            return "Invalid esm_class field data";
        case DIA_INTF_RCNTSUBDL:
            return "Cannot Submit to Distribution List";
        case DIA_INTF_RSUBMITFAIL:
            return "Submit failed";
        case DIA_INTF_RINVSRCTON:
            return "Invalid Source address TON"; 
        case DIA_INTF_RINVSRCNPI:
            return "Invalid Source address NPI";
        case DIA_INTF_RINVDSTTON:
            return "Invalid Destination address TON";
        case DIA_INTF_RINVDSTNPI:
            return "Invalid Destination address NPI";
        case DIA_INTF_RINVSYSTYP:
            return "Invalid system_type field";
        case DIA_INTF_RINVREPFLAG:
            return "Invalid replace_if_present flag";
        case DIA_INTF_RINVNUMMSGS:
            return "Invalid number of messages";
        case DIA_INTF_RTHROTTLED:
            return "Throttling error";
        case DIA_INTF_RINVSCHED:
            return "Invalid Scheduled Delivery Time";
        case DIA_INTF_RINVEXPIRY:
            return "Invalid message validity period";
        case DIA_INTF_RINVDFTMSGID:
            return "Predefined Message ID is Invalid or specific predefined message was not found";
        case DIA_INTF_RX_T_APPN:
            return "ESME Receiver Temporary App Error Code";
        case DIA_INTF_RX_P_APPN:
            return "ESME Receiver Permanent App Error Code";
        case DIA_INTF_RX_R_APPN:
            return "ESME Receiver Reject Message Error Code";
        case DIA_INTF_RQUERYFAIL:
            return "query_sm request failed";
        case DIA_INTF_RINVTLVSTREAM:
            return "Error in optional part of the PDU Body";
        case DIA_INTF_RTLVNOTALLWD:
            return "TLV not allowed";
        case DIA_INTF_RINVTLVLEN:
            return "Invalid Parameter Length";
        case DIA_INTF_RMISSINGTLV:
            return "Expected TLV missing";
        case DIA_INTF_RINVTLVVAL:
            return "Invalid TLV value";
        case DIA_INTF_RDELIVERYFAILURE:
            return "Transaction Delivery Failure";
        case DIA_INTF_RUNKNOWNERR:
            return "Unknown Error";
        case DIA_INTF_RSERTYPUNAUTH:
            return "ESME Not authorized to use specified service_type";
        case DIA_INTF_RPROHIBITED:
            return "ESME Prohibited from using specified operation";
        case DIA_INTF_RSERTYPUNAVAIL:
            return "Specified service_type is unavailable";
        case DIA_INTF_RSERTYPDENIED:
            return "Specified service_type is denied";
        case DIA_INTF_RINVDCS:
            return "Invalid Data Coding Scheme";
        case DIA_INTF_RINVSRCADDRSUBUNIT:
            return "Source Address Sub unit is invalid";
        case DIA_INTF_RINVDSTADDRSUBUNIT:
            return "Destination Address Sub unit is invalid";
        case DIA_INTF_RINVBCASTFREQINT:
            return "Broadcast Frequency Interval is invalid";
        case DIA_INTF_RINVBCASTALIAS_NAME:
            return "Broadcast Alias Name is invalid";
        case DIA_INTF_RINVBCASTAREAFMT:
            return "Broadcast Area Format is invalid";
        case DIA_INTF_RINVNUMBCAST_AREAS:
            return "Number of Broadcast Areas is invalid";
        case DIA_INTF_RINVBCASTCNTTYPE:
            return "Broadcast Content Type is invalid";
        case DIA_INTF_RINVBCASTMSGCLASS:
            return "Broadcast Message Class is invalid";
        case DIA_INTF_RBCASTFAIL:
            return "broadcast_sm operation failed";
        case DIA_INTF_RBCASTQUERYFAIL:
            return "broadcast_query_sm operation failed";
        case DIA_INTF_RBCASTCANCELFAIL:
            return "broadcast_cancel_sm operation failed";
        case DIA_INTF_RINVBCAST_REP:
            return "Number of Repeated Broadcasts is invalid";
        case DIA_INTF_RINVBCASTSRVGRP:
            return "Broadcast Service Group is invalid";
        case DIA_INTF_RINVBCASTCHANIND:
            return "Broadcast Channel Indicator is invalid";

        default:
            /* tell the user that we have a vendor-specific beast here */
            if (error >= 0x0400 && error <= 0x04FF)
                return "Vendor-specific error, please refer to your DIA_INTF provider";
            else
                return "Unknown/Reserved";
    }
}

int dia_intf_read_len(Connection *conn)
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
	if (len < MIN_DIA_INTF_PDU_LEN) {
		error(0, "SMPP: PDU length was too small (%d, minimum is %d).", len, MIN_DIA_INTF_PDU_LEN);
		return -1;
	}
	if (len > MAX_DIA_INTF_PDU_LEN) {
		error(0, "SMPP: PDU length was too large (%d, maximum is %d).", len, MAX_DIA_INTF_PDU_LEN);
		return -1;
	}
	return len;
}


Octstr *dia_intf_read_data(Connection *conn, int len)
{
	Octstr *os;

	os = conn_read_fixed(conn, ((len - 4) > 0 ? (len - 4) : 0));    /* `len' includes itself. */

	return os;
}

int send_dia_intf(Connection *conn, DIA_INTF_PDU *pdu)
{
	int ret;

	Octstr *os = dia_intf_pack(pdu);

	debug(__func__, __LINE__, "DIA_INTF SEND RAW");
	octstr_dump(os, GW_DEBUG);

	ret = conn_write(conn, os);   /* Caller checks for write errors later */
	if ( ret == -1 ) {
		error(errno, "conn_write failed");
		ret = RET_CONN_FAIL;
	}else {
		debug(__func__, __LINE__, "SEND PACKET");
		dia_intf_dump(pdu);
		ret = RET_CONN_SUCCESS;
	}

	octstr_destroy(os);

	return ret;
}

int read_dia_intf(Connection *conn, int *len, DIA_INTF_PDU **pdu)
{ 
	Octstr *os; 
	int rc;

	*len = dia_intf_read_len(conn); 
	if (*len == -1) { 
		error(0, "[%s : %d]: PDU dia_intf_read_len failed.", __func__, __LINE__);
		return RET_CONN_FAIL; 
	} else if (*len == 0) { 
		if (conn_eof(conn) || conn_error(conn)) {
			return RET_CONN_FAIL; 
		}
		return RET_CONN_TIME_OUT; 
	} 

	os = dia_intf_read_data(conn, *len); 
	if (os == NULL) { 
		if (conn_eof(conn) || conn_error(conn))  {
			error(0, "[%s : %d]: PDU dia_intf_read_data failed.", __func__, __LINE__);
		}else {
			error(0, "[%s : %d]: PDU dia_intf_read_data timeout failed.", __func__, __LINE__);
		}
		return RET_CONN_FAIL; 
	} 

	debug(__func__, __LINE__, "RECV PACKET");
	*pdu = dia_intf_unpack(os);
	if ( *pdu != NULL )  {
		dia_intf_dump(*pdu);
		rc = RET_CONN_SUCCESS;
	}
	else {
		rc = RET_CONN_FAIL;
	}
	octstr_destroy(os);

	return rc;
}
int read_dia_intf_timeout(Connection *conn, int *len, DIA_INTF_PDU **pdu, int tm_out)
{
	int ret = 0;

	if ( conn_inbuf_len(conn) == 0 ) {
		ret = wait_socket_event(conn, tm_out);
	
		if ( ret == RET_CONN_FAIL ) {
			return ret;
		}
	}

	ret =  read_dia_intf(conn, len, pdu);
	
	return ret;
}
