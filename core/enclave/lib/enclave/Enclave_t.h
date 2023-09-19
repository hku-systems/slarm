#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "../include/metadata.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

Enclave_t* api_new_enclave(Enclave_t* e);
void api_del_enclave(Enclave_t* e);
Gaps_t api_db_add(Enclave_t* e, Txn_t* txn);
Txn_t* api_db_get(Enclave_t* e, int urgent, uint64_t nonce, int* ok);

sgx_status_t SGX_CDECL ocall_print_string(int* retval, char* s);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
