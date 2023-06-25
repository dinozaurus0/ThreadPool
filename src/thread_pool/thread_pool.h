//  Created by Sima Vlad Grigore on 24/06/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.
//

#include "task.h"

#ifndef POOL_H
#define POOL_H

typedef struct ThreadPool ThreadPool;

#endif

ThreadPool *init_thread_pool(const unsigned int number_of_threads, Task *task, const unsigned int number_of_tasks);
void awake_threads(ThreadPool *pool);
void update_thread_pool_tasks(ThreadPool *pool, Task *task, const unsigned int number_of_tasks);
void notify_thread_pool_to_exit(ThreadPool *pool);
void wait_all_threads(ThreadPool *pool);
void release_thread_pool(ThreadPool *pool);
