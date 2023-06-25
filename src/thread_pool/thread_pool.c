//  Created by Sima Vlad Grigore on 24/06/2020.
//  Copyright © 2020 Sima Vlad Grigore. All rights reserved.
//

#include <pthread.h>
#include <stdlib.h>
#include "thread_pool.h"
#include "pool_notifier.h"

typedef struct ThreadPool {
    unsigned int number_of_tasks_finished;
    unsigned int total_number_of_tasks;
    pthread_mutex_t mutex;
    pthread_cond_t wait_threads_condition;
    pthread_cond_t awake_threads_condition;
    Task *task;
    PoolState state;
} ThreadPool;

static void create_threads(const int numberOfThreads, ThreadPool *pool);
static void *routine(void *thread_arg);
static void setup_thread_pool_locking(ThreadPool *pool);
static void task_finished_callback(PoolNotifier notifier);
static inline PoolNotifier create_pool_notifier(ThreadPool *pool);

ThreadPool *init_thread_pool(const unsigned int number_of_threads, Task *task, const unsigned int number_of_tasks)
{
    ThreadPool *pool = (ThreadPool *) malloc(sizeof(ThreadPool));
    pool->total_number_of_tasks = number_of_tasks;
    pool->number_of_tasks_finished = 0;
    pool->state = RUNNING;
    pool->task = task;
    setup_thread_pool_locking(pool);
    create_threads(number_of_threads, pool);
    
    return pool;
}

void awake_threads(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutex);
    pthread_cond_broadcast(&pool->awake_threads_condition);
    pthread_mutex_unlock(&pool->mutex);
}

void update_thread_pool_tasks(ThreadPool *pool, Task *task, const unsigned int number_of_tasks)
{
    pool->state = RUNNING;
    pool->number_of_tasks_finished = 0;
    pool->total_number_of_tasks = number_of_tasks;
    pool->task = task;
}

void notify_thread_pool_to_exit(ThreadPool *pool)
{
    pool->state = STOPPED;
    pthread_mutex_lock(&pool->mutex);
    pthread_cond_signal(&pool->awake_threads_condition);
    pthread_mutex_unlock(&pool->mutex);
}

void wait_all_threads(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutex);
    while (pool->state == RUNNING)
    {
        pthread_cond_wait(&pool->wait_threads_condition, &pool->mutex);
    }
    pthread_mutex_unlock(&pool->mutex);
}

void release_thread_pool(ThreadPool *pool)
{
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->wait_threads_condition);
    pthread_cond_destroy(&pool->awake_threads_condition);
    free(pool);
}

static void setup_thread_pool_locking(ThreadPool *pool)
{
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->wait_threads_condition, NULL);
    pthread_cond_init(&pool->awake_threads_condition, NULL);
}

static void create_threads(const int numberOfThreads, ThreadPool *pool)
{
    pthread_t thread;
    for (int i=0; i<numberOfThreads; i++)
    {
        pthread_create(&thread, NULL, routine, pool);
    }
}

static void* routine(void * thread_arg)
{
    ThreadPool *pool = (ThreadPool*) thread_arg;
    
    while (1)
    {
        if (pool->state == STOPPED) { break; }
        
        pthread_mutex_lock(&pool->mutex);
        
        Task *task = pool->task;
        
        if (task == NULL)
        {
            pthread_cond_wait(&pool->awake_threads_condition, &pool->mutex);
        } else
        {
            pool->task = task->next_task;
        }
        
        pthread_mutex_unlock(&pool->mutex);
        
        if (task != NULL)
        {
            PoolNotifier notifier = create_pool_notifier(pool);
            task->execute(task->function_arg, notifier, task_finished_callback);
        }
    }
    
    return NULL;
}

static void task_finished_callback(PoolNotifier notifier)
{
    pthread_mutex_lock(notifier.mutex);
    *notifier.number_of_tasks_finished = *notifier.number_of_tasks_finished + 1;

    if (*notifier.number_of_tasks_finished == notifier.total_number_of_tasks)
    {
        *notifier.state = BATCH_FINISHED;
        pthread_cond_signal(notifier.wait_threads_condition);
    }

    pthread_mutex_unlock(notifier.mutex);
}

static inline PoolNotifier create_pool_notifier(ThreadPool *pool)
{
    PoolNotifier notifier;
    notifier.mutex = &pool->mutex;
    notifier.number_of_tasks_finished = &pool->number_of_tasks_finished;
    notifier.total_number_of_tasks = pool->total_number_of_tasks;
    notifier.wait_threads_condition = &pool->wait_threads_condition;
    notifier.state = &pool->state;
    
    return notifier;
}