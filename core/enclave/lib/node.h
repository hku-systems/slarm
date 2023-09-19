//
// Created by Michael Xusheng Chen on 11/4/2018.
//

#ifndef LEADER_ELECTION_NODE_H
#define LEADER_ELECTION_NODE_H

// api for leader election

#include "include/metadata.h"
#ifdef __cplusplus
extern "C" {
#endif

void test();

char**makeCharArray(int size);

void setArrayString(char **a, char *s, int n);

void freeCharArray(char **a, int size);

Enclave_t* new_enclave();

int del_enclave(Enclave_t *e);

Gaps_t db_add(Enclave_t *e, Txn_t *txn);

Txn_t* db_get(Enclave_t *e, int urgent, uint64_t nonce, int *ok);

Txn_t* new_txn(char* payload, int psize, int is_urgent, uint64_t nonce);

void copy_array(char *go_arr, uint64_t *carr, int size);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //LEADER_ELECTION_NODE_H
