//
// Created by jianyu on 5/18/18.
//

#ifndef NODE_NODE_APP_H
#define NODE_NODE_APP_H

#include <sgx_eid.h>
#include <sgx_urts.h>
#include <stddef.h>
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>

#ifdef SGX_HW
#define SGX_PSW_UAE_SO "libsgx_uae_service.so"
#define SGX_PSW_URT_SO "libsgx_urts.so"
#define FILENAME "/usr/lib/libenclave_trusted_signed_hw.so"
#else
#define SGX_PSW_UAE_SO "libsgx_uae_service_sim.so"
#define SGX_PSW_URT_SO "libsgx_urts_sim.so"
// #define FILENAME "/usr/lib/libenclave_trusted_signed_sim.so"
#define FILENAME "enclave.signed.so"
#endif



sgx_enclave_id_t enclave_id;

int psw_installed_check();

void init_enclave();



// this file contains interfaces to enclave api


#endif //NODE_NODE_APP_H
