//
// Created by jianyu on 5/19/18.
//

#include "enclave_manager.h"
#include <dlfcn.h>

void *handle_urts = NULL;
int init = 0;
int created = 0;
uint64_t global_id = 0;

int psw_installed_check() {
    // if (init) {
    //     return 1;
    // }
    // void *handle_urts = NULL;
    // void  *handle_uae_service = NULL;
    // handle_uae_service = dlopen(SGX_PSW_UAE_SO , RTLD_NOW);
    // if (!handle_uae_service) {
    //     printf("dlopen: uae_service not available %d\n", errno);
    //     return 0;
    // }

    // handle_urts = dlopen(SGX_PSW_URT_SO, RTLD_NOW);
    // if (!handle_urts) {
    //     printf("dlopen: sgx_urts.so not avaialble\n");
    //     dlclose(handle_uae_service);
    //     handle_uae_service = NULL;
    //     return 0;
    // }
    // init = 1;
    return 1;
}

void init_enclave() {
    if (created) {
        return;
    }
    sgx_status_t ret = SGX_SUCCESS;
    sgx_launch_token_t token = {0};
    int updated = 0;
    if (!(psw_installed_check())) {
        printf(" PSW Not installed\n");
        return;
    }
    sgx_enclave_id_t current = global_id;
    ret = sgx_create_enclave(FILENAME, SGX_DEBUG_FLAG, &token, &updated, &current, NULL);
    if (ret != SGX_SUCCESS) {
        printf("\nApp: error %#x, failed to create enclave.\n", ret);
    }
    enclave_id = current;
    created = 1;
}