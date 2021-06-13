#ifndef _TRANS_DATA_H_
#define _TRANS_DATA_H_

#include "FtpSession.h"

void download_file(ConnectionSession_t *sess);
void upload_file(ConnectionSession_t *sess, int is_appe);
void trans_list(ConnectionSession_t *sess, int list);

#endif  /*_TRANS_DATA_H_*/
