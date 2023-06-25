//  Created by Sima Vlad Grigore on 08/07/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.

#include <stdlib.h>
#include "task.h"

Task *init_task(Task *next, void *function_arg, void (*execute)(void *function_arg, PoolNotifier notifier, void (*completion)(PoolNotifier notifier)))
{
    Task *task = (Task *) malloc(sizeof(Task));
    task->next_task = next;
    task->function_arg = function_arg;
    task->execute = execute;
    return task;
}

void releease_task(Task *task)
{
    free(task);
}
