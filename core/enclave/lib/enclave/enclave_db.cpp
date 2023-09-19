
#include "enclave_db.h"
#include "enclave_sys.h"
#include <stdarg.h>
#include "Enclave_t.h"

#include <vector>

int printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    int res;
    ocall_print_string(&res, buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}

void* init_db() {
    auto db = new QueryDB();
    db->non_urgent_db.ready_nonce = 0;
    db->urgent_db.ready_nonce = 0;
    return db;
}

Gaps_t QueryDB::add(Txn_t *txn) {
    auto urgent = txn->is_urgent;
    auto nonce = txn->nonce;
    DB* db_ref = (urgent)? &urgent_db : &non_urgent_db;

    info_print("add request, urgent: %d\n", urgent);

    if (db_ref->exists(nonce)) {
        info_print("nonce %ld exists\n", nonce);
        return Gaps_t {
            .gaps = NULL,
            .size = 0
        };
    }
    
    db_ref->db[nonce] = txn;
    db_ref->incrementNonce();
    info_print("nonce %ld added\n", nonce);
    return db_ref->detectGaps(urgent, nonce);
}

Txn_t* QueryDB::get(int urgent, uint64_t nonce, int &ok) {
    DB* db_ref = (urgent)? &urgent_db : &non_urgent_db;
    ok = (db_ref->exists(nonce))? 1 : 0;
    debug_print("get request urgent:%d, nonce:%u %d \n", urgent, nonce, ok);
    return db_ref->db[nonce];
}

void DB::incrementNonce() {
    auto i = ready_nonce + 1;
    while (1) {
        if (exists(i)) {
            i++;
        } else {
            break;
        }
    }
    ready_nonce = i - 1;
}

Gaps_t DB::detectGaps(int urgent, uint64_t nonce) {
    auto gaps = std::vector<uint64_t>();
    for (int i = ready_nonce + 1;i < nonce; i++) {
        if (!exists(i)) {
            info_print("gap %u\n", i);
            gaps.push_back(i);
        }
    }
    Gaps_t gap;
    gap.size = gaps.size();
    gap.gaps = new uint64_t[gap.size];
    for (int i = 0;i < gap.size;i++) {
        gap.gaps[i] = gaps[i];
    }
    return gap;
}

int DB::exists(uint64_t nonce) {
    return db.count(nonce) > 0;
}