//
// Created by jianyu on 5/20/18.
//

#ifndef NODE_ENCLAVE_WRAPPER_H
#define NODE_ENCLAVE_WRAPPER_H

#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "include/metadata.h"
#include "enclave/enclave_node.h"

// A fake implementation of enclave
typedef int sgx_status_t;
typedef int sgx_enclave_id_t;
#define SGX_SUCCESS 1

#define init_enclave()

sgx_enclave_id_t enclave_id;

sgx_status_t enclave_api_new_enclave(Enclave_t **ret, Enclave_t *e) {
    api_new_enclave(e);
    return SGX_SUCCESS;
}
sgx_status_t enclave_del_enclave(Enclave_t *e) {
    api_del_enclave(e);
    return  SGX_SUCCESS;
}

#endif //NODE_ENCLAVE_WRAPPER_H
