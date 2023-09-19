#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_api_new_enclave_t {
	Enclave_t* ms_retval;
	Enclave_t* ms_e;
} ms_api_new_enclave_t;

typedef struct ms_api_del_enclave_t {
	Enclave_t* ms_e;
} ms_api_del_enclave_t;

typedef struct ms_api_db_add_t {
	Gaps_t ms_retval;
	Enclave_t* ms_e;
	Txn_t* ms_txn;
} ms_api_db_add_t;

typedef struct ms_api_db_get_t {
	Txn_t* ms_retval;
	Enclave_t* ms_e;
	int ms_urgent;
	uint64_t ms_nonce;
	int* ms_ok;
} ms_api_db_get_t;

typedef struct ms_ocall_print_string_t {
	int ms_retval;
	char* ms_s;
} ms_ocall_print_string_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ms->ms_retval = ocall_print_string(ms->ms_s);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[1];
} ocall_table_Enclave = {
	1,
	{
		(void*)Enclave_ocall_print_string,
	}
};
sgx_status_t api_new_enclave(sgx_enclave_id_t eid, Enclave_t** retval, Enclave_t* e)
{
	sgx_status_t status;
	ms_api_new_enclave_t ms;
	ms.ms_e = e;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t api_del_enclave(sgx_enclave_id_t eid, Enclave_t* e)
{
	sgx_status_t status;
	ms_api_del_enclave_t ms;
	ms.ms_e = e;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t api_db_add(sgx_enclave_id_t eid, Gaps_t* retval, Enclave_t* e, Txn_t* txn)
{
	sgx_status_t status;
	ms_api_db_add_t ms;
	ms.ms_e = e;
	ms.ms_txn = txn;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t api_db_get(sgx_enclave_id_t eid, Txn_t** retval, Enclave_t* e, int urgent, uint64_t nonce, int* ok)
{
	sgx_status_t status;
	ms_api_db_get_t ms;
	ms.ms_e = e;
	ms.ms_urgent = urgent;
	ms.ms_nonce = nonce;
	ms.ms_ok = ok;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

