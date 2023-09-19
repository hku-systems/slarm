
#pragma once

#include "../include/metadata.h"
#include <map>

void* init_db();

class QueryDB;

class DB {
public:
    std::map<uint64_t, Txn_t*> db;
    uint64_t ready_nonce;
    void incrementNonce();
    Gaps_t detectGaps(int urgent, uint64_t nonce);
    int exists(uint64_t);
};

class QueryDB {
public:
    DB urgent_db;
    DB non_urgent_db;
    Gaps_t add(Txn_t *txn);
    Txn_t* get(int urgent, uint64_t nonce, int &ok);
};