#ifndef _COMMAND_MAP_H_
#define _COMMAND_MAP_H_

#include "FtpSession.h"

//命令映射相关函数

void do_command_map(ConnectionSession_t *);
void ftp_reply(ConnectionSession_t *, int, const char *);
void ftp_lreply(ConnectionSession_t *, int, const char *);

void do_user(ConnectionSession_t *);
void do_pass(ConnectionSession_t *);
void do_cwd(ConnectionSession_t *);
void do_cdup(ConnectionSession_t *);
void do_quit(ConnectionSession_t *);
void do_port(ConnectionSession_t *);
void do_pasv(ConnectionSession_t *);
void do_type(ConnectionSession_t *);
void do_stru(ConnectionSession_t *);
void do_mode(ConnectionSession_t *);
void do_retr(ConnectionSession_t *);
void do_stor(ConnectionSession_t *);
void do_appe(ConnectionSession_t *);
void do_list(ConnectionSession_t *);
void do_nlst(ConnectionSession_t *);
void do_rest(ConnectionSession_t *);
void do_abor(ConnectionSession_t *);
void do_pwd(ConnectionSession_t *);
void do_mkd(ConnectionSession_t *);
void do_rmd(ConnectionSession_t *);
void do_dele(ConnectionSession_t *);
void do_rnfr(ConnectionSession_t *);
void do_rnto(ConnectionSession_t *);
void do_site(ConnectionSession_t *);
void do_syst(ConnectionSession_t *);
void do_feat(ConnectionSession_t *);
void do_size(ConnectionSession_t *);
void do_stat(ConnectionSession_t *);
void do_noop(ConnectionSession_t *);
void do_help(ConnectionSession_t *);

#endif  /*_COMMAND_MAP_H_*/
