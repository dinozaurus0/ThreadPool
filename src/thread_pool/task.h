//  Created by Sima Vlad Grigore on 28/06/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.

#include "pool_notifier.h"

#ifndef TASK_H
#define TASK_H

typedef struct Task {
    struct Task *next_task;
    void *function_arg;
    void (*execute)(void *function_arg, PoolNotifier notifier, void (*completion)(PoolNotifier notifier));
} Task;

#endif

Task *init_task(Task *next, void *function_arg, void (*execute)(void *function_arg, PoolNotifier notifier, void (*completion)(PoolNotifier notifier)));
void releease_task(Task *task);
