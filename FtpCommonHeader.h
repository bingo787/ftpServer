#ifndef _SYS_UTIL_H_
#define _SYS_UTIL_H_

#include "FtpCommonSysHeader.h"
//去除右边的\r\n
void str_trim_crlf(char *str);
//分割字符串，根据c分割为两部分
void str_split(const char *str , char *left, char *right, char c);
//判断字符串是否全部为空格
int str_all_space(const char *str);
//把字符串转化为大写
void str_upper(char *str);
//把八进制字符串转化为无符号整数
unsigned int str_octal_to_uint(const char *str);

void activate_oobinline(int sockfd);
void activate_signal_sigurg(int sockfd);

int get_curr_time_sec();
int get_curr_time_usec();
int nano_sleep(double t);

int lock_file_read(int fd);
int lock_file_write(int fd);
int unlock_file(int fd);

//创建客户fd
int tcp_client(unsigned int port);
//创建监听fd
int tcp_server(const char *host, unsigned short port);

int get_local_ip(char *ip);

//设置fd为阻塞或者非阻塞
void activate_nonblock(int fd);
void deactivate_nonblock(int fd);

//超时IO函数
int read_timeout(int fd, unsigned int wait_seconds);
int write_timeout(int fd, unsigned int wait_seconds);
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);
int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds);

//处理TCP粘包问题
ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, const void *buf, size_t n);
ssize_t readline(int sockfd, void *buf, size_t maxline);

//发送与接收文件描述符
void send_fd(int sock_fd, int fd);
int recv_fd(const int sock_fd);

#endif  /*_SYS_UTIL_H_*/
