# Thread Pool

This project is a representation of a thread pool.

## Installation

In the Makefile folder, open a terminal and run make all(previous to this you need to have a C compiler installed). After that the Makefile will generate a main executable that you can run after with ./main. 

```bash
make
./main
```

## Usage

The project behaves like a async queue, that supports multiple tasks being scheduled at the same time. This is a task base represantation of a thread pool, meaning all the thread's state is dictated by the state of the task it executes and this behaviour is independant from the thread that executes it. When there are no more tasks in the pool to execute, all spawn threads halt and are pulled out from ready queue of the processor, to avoid spinning. When new tasks are added to the processor, the calling thread sends a broadcast message to all sleeping threads to awake and start executing the new batch of tasks.

In what regard the functionality, you can do the following:

1. Schedules task for execution.
2. Wait for all tasks.
3. Queue up for tasks  execution after a specific batch has finished.
4. Awake the thread pool(created threads) if another batch of tasks is scheduled.
5. Exit the thread pool to clean the memory afterwards.
