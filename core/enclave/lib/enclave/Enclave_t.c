#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_api_new_enclave(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_api_new_enclave_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_api_new_enclave_t* ms = SGX_CAST(ms_api_new_enclave_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	Enclave_t* _tmp_e = ms->ms_e;



	ms->ms_retval = api_new_enclave(_tmp_e);


	return status;
}

static sgx_status_t SGX_CDECL sgx_api_del_enclave(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_api_del_enclave_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_api_del_enclave_t* ms = SGX_CAST(ms_api_del_enclave_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	Enclave_t* _tmp_e = ms->ms_e;



	api_del_enclave(_tmp_e);


	return status;
}

static sgx_status_t SGX_CDECL sgx_api_db_add(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_api_db_add_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_api_db_add_t* ms = SGX_CAST(ms_api_db_add_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	Enclave_t* _tmp_e = ms->ms_e;
	Txn_t* _tmp_txn = ms->ms_txn;



	ms->ms_retval = api_db_add(_tmp_e, _tmp_txn);


	return status;
}

static sgx_status_t SGX_CDECL sgx_api_db_get(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_api_db_get_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_api_db_get_t* ms = SGX_CAST(ms_api_db_get_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	Enclave_t* _tmp_e = ms->ms_e;
	int* _tmp_ok = ms->ms_ok;



	ms->ms_retval = api_db_get(_tmp_e, ms->ms_urgent, ms->ms_nonce, _tmp_ok);


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[4];
} g_ecall_table = {
	4,
	{
		{(void*)(uintptr_t)sgx_api_new_enclave, 0, 0},
		{(void*)(uintptr_t)sgx_api_del_enclave, 0, 0},
		{(void*)(uintptr_t)sgx_api_db_add, 0, 0},
		{(void*)(uintptr_t)sgx_api_db_get, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][4];
} g_dyn_entry_table = {
	1,
	{
		{0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print_string(int* retval, char* s)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_s = s ? strlen(s) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(s, _len_s);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (s != NULL) ? _len_s : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (s != NULL) {
		ms->ms_s = (char*)__tmp;
		if (_len_s % sizeof(*s) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, s, _len_s)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_s);
		ocalloc_size -= _len_s;
	} else {
		ms->ms_s = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

