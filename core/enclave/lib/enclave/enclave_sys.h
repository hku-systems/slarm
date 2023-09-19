//
// Created by jianyu on 5/18/18.
//

#ifndef NODE_ENCLAVE_SYS_H
#define NODE_ENCLAVE_SYS_H

#ifdef ENCLAVE
#include <sgx_trts.h>
#include "election_t.h"
#define WRAPBUFSIZ 15000

#define stderr 2

#ifdef __cplusplus
extern "C" {
#endif

int fprintf(int p, const char *fmt, ...);

void perror(const char* msg);

#define rand_init()

int rand_get();

#ifdef __cplusplus
}
#endif

#define gettime(time) gettimeofday(time, NULL)

typedef unsigned int socklen_t;

typedef unsigned long ssize_t;

struct in_addr {
    uint32_t s_addr;          // load with inet_pton()
};

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct timeval   {
    long int tv_sec;
    long int tv_usec;
};

struct timespec
{
    unsigned long tv_sec;		/* Seconds.  */
    unsigned long tv_nsec;	/* Nanoseconds.  */
};

#else
#include <unistd.h>
#include <stdio.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#define rand_init() { \
    time_t t; \
    srand((unsigned) time(&t) + offset * 10000); } \

#define rand_get() rand()
#endif
//void gettimeofday(struct timeval t) {
//
//}

#endif //NODE_ENCLAVE_SYS_H
