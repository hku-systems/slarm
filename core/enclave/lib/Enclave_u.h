#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */

#include "include/metadata.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (char* s));
#endif

sgx_status_t api_new_enclave(sgx_enclave_id_t eid, Enclave_t** retval, Enclave_t* e);
sgx_status_t api_del_enclave(sgx_enclave_id_t eid, Enclave_t* e);
sgx_status_t api_db_add(sgx_enclave_id_t eid, Gaps_t* retval, Enclave_t* e, Txn_t* txn);
sgx_status_t api_db_get(sgx_enclave_id_t eid, Txn_t** retval, Enclave_t* e, int urgent, uint64_t nonce, int* ok);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
