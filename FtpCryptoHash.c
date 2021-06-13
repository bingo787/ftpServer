
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "FtpCommonSysHeader.h"
#include "FtpCommonHeader.h"
#include "FtpSession.h"
#include "FtpGlobalConfiguration.h"
#include "FtpParseConfig.h"
#include "FtpCommandMap.h"
#include "FtpCryptoHash.h"

static Hash_node_t ** hash_get_bucket(Hash_t *hash, void *key);
static Hash_node_t *hash_get_node_by_key(Hash_t *hash, void *key, unsigned int key_size);
static void hash_clear_bucket(Hash_t *hash, unsigned int index);

//建立一个hash表，然后找回指针返回
Hash_t *hash_alloc(unsigned int buckets, hashfunc_t hash_func)
{
    Hash_t *hash = (Hash_t *)malloc(sizeof(Hash_t));
    hash->buckets = buckets; //桶的个数
    hash->hash_func = hash_func; //hash函数
    int len = sizeof(Hash_node_t *) * buckets; //数组占用字节数
    hash->nodes = (Hash_node_t **)malloc(len);
    memset(hash->nodes, 0, len);
    return hash;
}

//根据key查找value
void *hash_lookup_value_by_key(Hash_t *hash, 
        void *key, 
        unsigned int key_size)
{
    //先查找结点
    Hash_node_t *node = hash_get_node_by_key(hash, key, key_size);
    if(node == NULL)
    {
        return NULL;
    }
    else
    {
        return node->value;
    }
}

//向hash中添加结点
void hash_add_entry(Hash_t *hash, 
        void *key, 
        unsigned int key_size, 
        void *value,
        unsigned int value_size)
{
    //1. 查找bucket
    Hash_node_t **buck = hash_get_bucket(hash, key);
    assert(buck != NULL);

    //2.生成新的结点
    Hash_node_t *node = (Hash_node_t *)malloc(sizeof(Hash_node_t));
    memset(node, 0, sizeof(Hash_node_t));
    node->key = malloc(key_size);
    node->value = malloc(value_size);
    memcpy(node->key, key, key_size);
    memcpy(node->value, value, value_size); 

    //3.插入相应的链表， 头插法
    if(*buck != NULL)
    {
        Hash_node_t *head = *buck; //head是链表第一个结点

        node->next = head;
        head->pre = node;
        *buck = node; // 这里要用指针操作
    }
    else
    {
        *buck = node;
    }
}

//hash中释放结点
void hash_free_entry(Hash_t *hash, 
        void *key, 
        unsigned int key_size)
{
    //查找结点
    Hash_node_t *node = hash_get_node_by_key(hash, key, key_size);
    assert(node != NULL);

    free(node->key);
    free(node->value);

    //删除结点
    if(node->pre != NULL) //不是第一个结点
    {
        node->pre->next = node->next;
    }
    else
    {
        Hash_node_t **buck = hash_get_bucket(hash, key);
        *buck = node->next;
    }
    if(node->next != NULL) //还有下一个结点
        node->next->pre = node->pre;
    free(node);
}

//清空hash表
void hash_clear_entry(Hash_t *hash)
{
    unsigned int i;
    for(i = 0; i < hash->buckets; ++i)
    {
        hash_clear_bucket(hash, i);
        hash->nodes[i] = NULL;
    }
}

//销毁hash表
void hash_destroy(Hash_t *hash)
{
    hash_clear_entry(hash);
    free(hash->nodes);
    free(hash);
}

//根据key获取bucket
static Hash_node_t **hash_get_bucket(Hash_t *hash, void *key)
{
    unsigned int pos = hash->hash_func(hash->buckets, key); //根据key获取key所在buckets的位置
    assert(pos < hash->buckets);

    return &(hash->nodes[pos]); //返回key所在链表地址
}

//根据key获取node结点
static Hash_node_t *hash_get_node_by_key(Hash_t *hash, 
        void *key, 
        unsigned int key_size)
{
    //获取bucket
    Hash_node_t **buck = hash_get_bucket(hash, key);
    assert(buck != NULL);

    //查找元素
    Hash_node_t *node = *buck; //指向(key所在链表的）第一个元素
    while(node != NULL && memcmp(node->key, key, key_size) != 0)
    {
        node = node->next;
    }

    return node; //包含三种情况 NULL、???
}

//清空bucket
static void hash_clear_bucket(Hash_t *hash, unsigned int index)
{
    assert(index < hash->buckets); //防止越界
    Hash_node_t *node = hash->nodes[index]; //获得key所在桶第一个元素
    Hash_node_t *tmp = node;
    while(node)
    {
        tmp = node->next;
        free(node->key);
        free(node->value);
        free(node);
        node = tmp;
    }
    hash->nodes[index] = NULL;
}


unsigned int num_of_clients = 0; //表示连接数目

Hash_t *ip_to_clients;
Hash_t *pid_to_ip;

static void handle_sigchld(int sig);
static unsigned int hash_func(unsigned int buckets, void *key);
//获取当前ip的数目，并且+1
static unsigned int add_ip_to_hash(uint32_t ip);


void check_permission()
{
  //root 的uid为0
  if(getuid())
  {
    fprintf(stderr, "FtpServer must be started by root\n");
    exit(EXIT_FAILURE);
  }
}

void setup_signal_chld()
{
  //signal第二个参数是handler，则signal阻塞，执行handler函数
  if(signal(SIGCHLD, handle_sigchld) == SIG_ERR)
    ERR_EXIT("signal");
}

void print_conf()
{
  printf("tunable_pasv_enable=%d\n", tunable_pasv_enable);
  printf("tunable_port_enable=%d\n", tunable_port_enable);

  printf("tunable_listen_port=%u\n", tunable_listen_port);
  printf("tunable_max_clients=%u\n", tunable_max_clients);
  printf("tunable_max_per_ip=%u\n", tunable_max_per_ip);
  printf("tunable_accept_timeout=%u\n", tunable_accept_timeout);
  printf("tunable_connect_timeout=%u\n", tunable_connect_timeout);
  printf("tunable_idle_session_timeout=%u\n", tunable_idle_session_timeout);
  printf("tunable_data_connection_timeout=%u\n", tunable_data_connection_timeout);
  printf("tunable_local_umask=0%o\n", tunable_local_umask);
  printf("tunable_upload_max_rate=%u\n", tunable_upload_max_rate);
  printf("tunable_download_max_rate=%u\n", tunable_download_max_rate);

  if (tunable_listen_address == NULL)
    printf("tunable_listen_address=NULL\n");
  else
    printf("tunable_listen_address=%s\n", tunable_listen_address);
}

void limit_num_clients(Session_t *sess)
{
  if(tunable_max_clients > 0 && sess->curr_clients > tunable_max_clients)
  {
    //421 There are too many connected users, please try later.
    ftp_reply(sess, FTP_TOO_MANY_USERS, "There are too many connected users, please try later.");
    exit(EXIT_FAILURE);
  }

  if(tunable_max_per_ip > 0 && sess->curr_ip_clients > tunable_max_per_ip)
  {
    //421 There are too many connections from your internet address.
    ftp_reply(sess, FTP_IP_LIMIT, "There are too many connections from your internet address.");
    exit(EXIT_FAILURE);
  }
}

static void handle_sigchld(int sig)
{
  pid_t pid;
  //waitpid挂起当前进程的执行，直到孩子状态发生改变
  //孩子状态发生变化，伴随着孩子进程相关资源的释放，这可以解决僵尸进程
  while((pid = waitpid(-1, NULL, WNOHANG)) > 0)
  {
    --num_of_clients;
    //pid -> ip
    uint32_t *p_ip = hash_lookup_value_by_key(pid_to_ip, &pid, sizeof(pid));
    assert(p_ip != NULL); //ip必须能找到
    uint32_t ip = *p_ip;
    //ip -> clients
    unsigned int *p_value = (unsigned int *)hash_lookup_value_by_key(ip_to_clients, &ip, sizeof(ip));
    assert(p_value != NULL && *p_value > 0);
    --*p_value;

    //释放hash表的结点
    hash_free_entry(pid_to_ip, &pid, sizeof(pid));
  }
}

//hash函数
static unsigned int hash_func(unsigned int buckets, void *key)
{
  unsigned int *number = (unsigned int*)key;

  return (*number) % buckets;
}


//在对应的ip记录中+1,返回当前ip的客户数量
static unsigned int add_ip_to_hash(uint32_t ip)
{
  unsigned int *p_value = (unsigned int *)hash_lookup_value_by_key(ip_to_clients, &ip, sizeof(ip));
  if(p_value == NULL)
  {
    unsigned int value = 1;
    hash_add_entry(ip_to_clients, &ip, sizeof(ip), &value, sizeof(value));
    return 1;
  }
  else
  {
    unsigned int value = *p_value;
    ++value;
    *p_value = value;

    return value;
  }
}


void init_hash()
{
  ip_to_clients = hash_alloc(256, hash_func);
  pid_to_ip = hash_alloc(256, hash_func);
}

void add_clients_to_hash(Session_t *sess, uint32_t ip)
{
  ++num_of_clients; //连接数目+1
  sess->curr_clients = num_of_clients;
  sess->curr_ip_clients = add_ip_to_hash(ip);
}

void add_pid_ip_to_hash(pid_t pid, uint32_t ip)
{
  hash_add_entry(pid_to_ip, &pid, sizeof(pid), &ip, sizeof(ip));
}
