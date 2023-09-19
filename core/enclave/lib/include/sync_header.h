//
// Created by Maxxie Jiang on 19/5/2018.
//

#ifndef NODE_SYNC_HEADER_H
#define NODE_SYNC_HEADER_H

#ifdef ENCLAVE
#include <sgx_thread.h>

typedef sgx_thread_mutex_t pmutex_t;

typedef sgx_thread_cond_t pcond_t;

typedef sgx_thread_t pthread_id;

#define pthread_padding uint64[2];

//#define plock(mutex) sgx_thread_mutex_lock(mutex.lock)
//#define punlock(mutex) sgx_thread_mutex_unlock(mutex.lock)
#define plock(mutex)
#define punlock(mutex)
#define plock_init(mutex) sgx_thread_mutex_init(mutex.lock, NULL)

#define pcond_init(con) sgx_thread_cond_init(con.cond, NULL)
//#define pcond_broadcast(con) sgx_thread_cond_broadcast(con.cond)
#define pcond_broadcast(con) ret_broadcast=offset+1;
#define pcond_time_wait(con, mutex, t) sgx_thread_cond_wait(con.cond, mutex.lock)

#else

#include <pthread.h>
typedef pthread_mutex_t pmutex_t;

typedef pthread_cond_t pcond_t;

typedef pthread_t pthread_id;

#define pthread_padding uint64[2];

#define plock(mutex) pthread_mutex_lock(mutex)
#define punlock(mutex) pthread_mutex_unlock(mutex)
#define plock_init(mutex) pthread_mutex_init(mutex, NULL)

#define pcond_init(cond) pthread_cond_init(cond, NULL)
#define pcond_broadcast(cond) pthread_cond_broadcast(cond)
#define pcond_time_wait(cond, lock, t) pthread_cond_timedwait(cond, lock, t)
#endif

#endif //NODE_SYNC_HEADER_H
