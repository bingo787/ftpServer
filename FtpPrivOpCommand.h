#ifndef _PRIV_COMMAND_H_
#define _PRIV_COMMAND_H_

#include "FtpSession.h"

void privop_pasv_get_data_sock(ConnectionSession_t *sess);
void privop_pasv_active(ConnectionSession_t *sess);
void privop_pasv_listen(ConnectionSession_t *sess);
void privop_pasv_accept(ConnectionSession_t *sess);

#endif  /*_PRIV_COMMAND_H_*/
