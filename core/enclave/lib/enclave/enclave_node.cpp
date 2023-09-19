
#include "enclave_node.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "enclave_sys.h"
#include "enclave_db.h"
#include "Enclave_t.h"

void break_enclave() {

}

#ifdef __cplusplus
extern "C" {
#endif

Enclave_t* api_new_enclave(Enclave_t *e) {
    e->db = init_db();
    return e;
}

void api_del_enclave(Enclave_t *e) {
    // do nothing
}

Gaps_t api_db_add(Enclave_t *e, Txn_t *txn) {
    return ((QueryDB*)e->db)->add(txn);
}

Txn_t* api_db_get(Enclave_t *e, int urgent, uint64_t nonce, int *ok) {
    auto got_txn = ((QueryDB*)e->db)->get(urgent, nonce, *ok);
    return got_txn;
}

#ifdef ENCLAVE

int fprintf(int p, const char *fmt, ...) {
    char buf[WRAPBUFSIZ] = {'\0'};
    int result;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, WRAPBUFSIZ, fmt, ap);
            va_end(ap);
    ocall_print_string(&result, buf);
    // printf("Return to wrapper \n ");
    return result;
}

void perror(const char* msg) {
    fprintf(stderr, msg);
}

int rand_get() {
    unsigned char c;
    sgx_read_rand(&c, 1);
    return (int)c;
}

#endif

#ifdef __cplusplus
}
#endif