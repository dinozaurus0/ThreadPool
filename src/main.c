//  Created by Sima Vlad Grigore on 12/07/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.

#include <stdio.h>
#include "thread_pool/thread_pool.h"

static void execute_task(void *argument, PoolNotifier notifier, void(*completion)(PoolNotifier));

int main()
{
    Task *tasks[13];
    int tasks_ids[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    tasks[6] = NULL;
    for (int i=5; i>=0; i--)
    {
        tasks[i] = init_task(tasks[i+1], &tasks_ids[i], execute_task); 
    }
    
    ThreadPool *pool = init_thread_pool(3, tasks[0], 6);
    wait_all_threads(pool);
    
    printf("Finished first batch \n");

    tasks[12] = NULL;
    for (int i=11; i>=6; i--)
    {
        tasks[i] = init_task(tasks[i+1], &tasks_ids[i], execute_task); 
    }
    
    update_thread_pool_tasks(pool, tasks[6], 6);
    awake_threads(pool);
    wait_all_threads(pool);
    release_thread_pool(pool);
    
    printf("Finished second batch \n");

    for(int i=0; i<12; i++)
    {
        releease_task(tasks[i]);
    }
    
    return 0;
}

static void execute_task(void *argument, PoolNotifier notifier, void(*completion)(PoolNotifier))
{
    printf("Executed task with argument id %d \n", *((int*)argument));
    completion(notifier);
}
