//
// Created by jianyu on 5/19/18.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ocall_election.h"

int ocall_print_string(const char* s)
{
    fprintf(stdout, "%s", s);
    return 0;
}