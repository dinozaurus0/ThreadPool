//  Created by Sima Vlad Grigore on 11/07/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.

#include <pthread.h>

#ifndef POOL_NOTIFIER_H
#define POOL_NOTIFIER_H

typedef enum {
    RUNNING,
    BATCH_FINISHED,
    STOPPED
} PoolState;

// shallow copy of the pool struct
typedef struct PoolNotifier {
    unsigned int *number_of_tasks_finished;
    unsigned int total_number_of_tasks;
    pthread_mutex_t *mutex;
    pthread_cond_t *wait_threads_condition;
    PoolState *state;
} PoolNotifier;

#endif
