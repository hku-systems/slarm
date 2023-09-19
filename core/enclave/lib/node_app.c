//
// Created by Maxxie Jiang on 19/5/2018.
//

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include "node.h"
#include "include/metadata.h"
#include "include/sync_header.h"

#ifndef SGX_FAKE
#include "enclave_manager.h"
#include "Enclave_u.h"

#define CALL_API(func, ...) func(enclave_id, __VA_ARGS__)

#else
#define CALL_API(func, ...) enclave_##func(__VA_ARGS__)
#include "enclave_wrapper.h"
#endif


static void *RecvFunc(void *opaque);

char**makeCharArray(int size) {
    return (char**)calloc(sizeof(char*), size);
}

void test() {
    printf("jyp!!!");
}

void freeCharArray(char **a, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(a[i]);
    free(a);
}

void setArrayString(char **a, char *s, int n) {
    a[n] = s;
}

Enclave_t* new_enclave() {
    init_enclave();
    Enclave_t *e = NULL;
    e = (Enclave_t*)malloc(sizeof(Enclave_t));
    sgx_status_t status = CALL_API(api_new_enclave, &e, e);
    if (status != SGX_SUCCESS) {
        return NULL;
    }

    return e;
}

int del_enclave(Enclave_t *e) {
    sgx_status_t status = CALL_API(api_del_enclave, e);
    if (status != SGX_SUCCESS) {
        return NULL;
    }

    return 0;
}

Txn_t* new_txn(char* payload, int psize, int is_urgent, uint64_t nonce) {
    Txn_t *new_txn = (Txn_t *)malloc(sizeof(Txn_t));
    char *buf = (char*)malloc(psize);
    new_txn->is_urgent = is_urgent;
    new_txn->nonce = nonce;
    new_txn->payload = buf;
    new_txn->payload_size = psize;
    memcpy(new_txn->payload, payload, psize);
    return new_txn;
}

Gaps_t db_add(Enclave_t *e, Txn_t *txn) {
    Gaps_t ret;
    info_print("add addr %lx\n", txn);
    sgx_status_t status = CALL_API(api_db_add, &ret, e, txn);
    if (status != SGX_SUCCESS) {
        printf("Enclave: error in api_db_add, status %lx\n", status);
        return ret;
    }
    return ret;
}

Txn_t* db_get(Enclave_t *e, int urgent, uint64_t nonce, int *ok) {
    Txn_t *ret = NULL;
    sgx_status_t status = CALL_API(api_db_get, &ret, e, urgent, nonce, ok);
    if (status != SGX_SUCCESS) {
        printf("Enclave: error in api_db_get, status %lx\n", status);
        return NULL;
    }
    info_print("addr %lx\n", ret);
    return ret;
}

void copy_array(char *goarr, uint64_t *carr, int size) {
    uint64_t *go_arr = (uint64_t*)goarr;
    for (int i = 0;i < size;i++) {
        go_arr[i] = carr[i];
    }
}

