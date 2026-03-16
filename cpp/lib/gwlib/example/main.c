#include "flk_push.h"

void set_msg(PUSH_MSG_PDU *msg)
{
	strcpy(msg->papType, "00");
	strcpy(msg->senderId, "SYSTEM");
	msg->osTp[0] = '1';
	strcpy(msg->appId, "0123456789");
	strcpy(msg->regId, "DeviceToken");
	strcpy(msg->content, "good morning");
	strcpy(msg->data, "i'm data");
}

int main (int argc, char **argv)
{
	push_init("./push_client.log", 0, 1);

	PUSH_MSG_PDU msg;
	PUSH_MSG_RESP_PDU *resp;

	PUSH_CANCEL_MSG_PDU cancel;
	PUSH_CANCEL_MSG_RESP_PDU *cancel_resp;

	memset(&msg, 0x00, sizeof(PUSH_MSG_PDU));

	set_msg(&msg);

	resp = send_push_msg(&msg, "112.172.129.68", 8405);

	info(0, "result Code[%s] Desc[%s] pushId[%s]", resp->code, resp->desc, resp->pushId);

	memset(&cancel, 0x00, sizeof(PUSH_CANCEL_MSG_PDU));
	strcpy(cancel.papType, "01");
	strcpy(cancel.pushId, resp->pushId);

	cancel_resp = send_push_cancel(&cancel, "112.172.129.68", 8405);
	info(0, "cancel result Code[%s] Desc[%s]", cancel_resp->code, cancel_resp->desc);

	free(resp);
	free(cancel_resp);

	push_shutdown();
}
