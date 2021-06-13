#ifndef _TRANS_CTRL_H_
#define _TRANS_CTRL_H_

/*
 *限速模块
 */
#include "FtpSession.h"

void limit_curr_rate(ConnectionSession_t *sess, int nbytes, int is_upload);

//控制连接
void setup_signal_alarm_ctrl_fd();
void start_signal_alarm_ctrl_fd();

//数据连接
void setup_signal_alarm_data_fd();
void start_signal_alarm_data_fd();

void cancel_signal_alarm();

void setup_signal_sigurg();

void do_site_chmod(ConnectionSession_t *sess, char *args);
void do_site_umask(ConnectionSession_t *sess, char *args);
void do_site_help(ConnectionSession_t *sess);

#endif  /*_TRANS_CTRL_H_*/
