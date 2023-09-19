//
// Created by Michael Xusheng Chen on 11/4/2018.
//

#ifndef ENCLAVE_NODE_H
#define ENCLAVE_NODE_H

// api for leader election

#include "../include/metadata.h"

#ifdef __cplusplus
extern "C" {
#endif

Enclave_t* api_new_enclave(Enclave_t*);

void api_del_enclave(Enclave_t*);

Gaps_t api_db_add(Enclave_t *e, Txn_t *txn);

Txn_t* api_db_get(Enclave_t *e, int urgent, uint64_t nonce, int *ok);

#ifdef __cplusplus
}
#endif

#endif //LEADER_ELECTION_NODE_H
