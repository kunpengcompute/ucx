/**
 * Copyright (C) Mellanox Technologies Ltd. 2001-2018.  ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */

#ifndef UCS_TYPE_INIT_ONCE_H_
#define UCS_TYPE_INIT_ONCE_H_

#include <pthread.h>


/*
 * Synchronization object for one-time initialization.
 */
typedef struct ucs_init_once {
    pthread_mutex_t lock;        /* Protects the initialization */
    int             initialized; /* Whether the initialization took place */
} ucs_init_once_t;


/* Static initializer for @ref ucs_init_once_t */
#define UCS_INIT_ONCE_INIITIALIZER \
    { PTHREAD_MUTEX_INITIALIZER, 0 }


/*
 * Start a code block to perform an arbitrary initialization step only once
 * during the lifetime of the provided synchronization object.
 *
 * @param [in] _once Pointer to @ref ucs_init_once_t synchronization object.
 *
 * Usage:
 *     UCS_INIT_ONCE(&once) {
 *         ... code ...
 *     }
 *
 * @note It's safe to use a "break" statement in order to exit the code block,
 * but "return" and "continue" statements may lead to unexpected behavior.
 *
 * How does it work? First, lock the mutex. Then check if already initialized,
 * if yes unlock the mutex and exit the loop (pthread_mutex_unlock is expected
 * to return 0). Otherwise, perform the "body" of the for loop, and then set
 * "initialized" to 1. On the next condition check, unlock the mutex and exit.
 */
#define UCS_INIT_ONCE(_once) \
    /* coverity[double_unlock] */ \
    for (pthread_mutex_lock(&(_once)->lock); \
         !(_once)->initialized || pthread_mutex_unlock(&(_once)->lock); \
         (_once)->initialized = 1)

#endif