//
// Created by jianyu on 5/18/18.
//

#ifndef NODE_METADATA_H
#define NODE_METADATA_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "sync_header.h"

#define DEBUG 1
#define debug_print(fmt, ...) \
            do { if (DEBUG) printf(fmt, __VA_ARGS__, __FILE__, __LINE__); } while (0)

#define INFO 0
#define info_print(fmt, ...) \
            do { if (INFO) printf(fmt, __VA_ARGS__); } while (0)


#define __SIZEOF_PTHREAD_CONT_T 48
#define __SIZEOF_PTHREAD_MUTEX_T 40

typedef struct Txn_t {
    int        is_urgent;
    // uint64_t    urgent_nonce;
    // uint64_t    non_urgent_nonce;
    uint64_t    nonce;
    char*       payload;
    int         payload_size;
} Txn_t;

typedef struct Enclave_t {
    void* db;
} Enclave_t;

typedef struct {
    uint64_t *gaps;
    int size;
} Gaps_t;

#endif //NODE_METADATA_H
