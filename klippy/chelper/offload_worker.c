#include <pthread.h> // pthread_create
#include <stdint.h> // uint64_t
#include <stdlib.h> // malloc
#include <string.h> // memset
#include "stepcompress.h" // stepcompress_flush()
#include "itersolve.h" // struct stepper_kinematics *sk
#include "compiler.h" // __visible
#include "pyhelper.h" // set_thread_name

struct offload_worker {
    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_mutex_t done_mutex;
    pthread_cond_t done_cond;
    pthread_t thread;
    char name[16];

    struct stepper_kinematics *sk;
    double flush_time;
    uint64_t move_clock;
    int ret;

    uint8_t has_work;
    uint8_t shutdown;
    uint8_t work_done;
};

static void*
worker_thread(void* arg) {
    struct offload_worker *worker = (struct offload_worker *)arg;
    set_thread_name(worker->name);
    while (1) {
        pthread_mutex_lock(&worker->work_mutex);
        while (!worker->has_work && !worker->shutdown) {
            pthread_cond_wait(&worker->work_cond, &worker->work_mutex);
        }

        if (worker->shutdown) {
            pthread_mutex_unlock(&worker->work_mutex);
            break;
        }

        int ret = itersolve_generate_steps(worker->sk, worker->flush_time);
        struct stepcompress *sc = worker->sk->sc;
        if (!ret && worker->move_clock > 0)
            ret = stepcompress_flush(sc, worker->move_clock);

        worker->has_work = 0;
        pthread_mutex_unlock(&worker->work_mutex);
        worker->ret = ret;
        // Signal completion
        pthread_mutex_lock(&worker->done_mutex);
        worker->work_done = 1;
        pthread_cond_signal(&worker->done_cond);
        pthread_mutex_unlock(&worker->done_mutex);
    }

    return NULL;
}

struct offload_worker * __visible
offload_worker_alloc(char name[16]) {
    struct offload_worker *worker;
    worker = malloc(sizeof(struct offload_worker));
    memset(worker, 0, sizeof(*worker));
    strncpy(worker->name, name, sizeof(worker->name));
    worker->name[sizeof(worker->name)-1] = '\0';

    pthread_mutex_init(&worker->work_mutex, NULL);
    pthread_cond_init(&worker->work_cond, NULL);
    pthread_mutex_init(&worker->done_mutex, NULL);
    pthread_cond_init(&worker->done_cond, NULL);

    pthread_create(&worker->thread, NULL, worker_thread, worker);
    return worker;
}

void __visible
async_itersolve_generate_steps(struct offload_worker *worker,
                               struct stepper_kinematics *sk,
                               double flush_time,
                               uint64_t move_clock) {
    pthread_mutex_lock(&worker->work_mutex);
    worker->sk = sk;
    worker->flush_time = flush_time;
    worker->move_clock = move_clock;
    worker->has_work = 1;
    worker->work_done = 0;
    pthread_cond_signal(&worker->work_cond);
    pthread_mutex_unlock(&worker->work_mutex);
}

int __visible
result_offload_worker(struct offload_worker *worker) {
    int ret;
    pthread_mutex_lock(&worker->done_mutex);
    while (!worker->work_done)
        pthread_cond_wait(&worker->done_cond, &worker->done_mutex);
    worker->work_done = 0;
    ret = worker->ret;
    pthread_mutex_unlock(&worker->done_mutex);
    return ret;
}

void __visible
offload_worker_free(struct offload_worker *worker) {
    pthread_mutex_lock(&worker->work_mutex);
    worker->shutdown = 1;
    pthread_cond_signal(&worker->work_cond);
    pthread_mutex_unlock(&worker->work_mutex);

    pthread_join(worker->thread, NULL);

    pthread_mutex_destroy(&worker->work_mutex);
    pthread_cond_destroy(&worker->work_cond);
    pthread_mutex_destroy(&worker->done_mutex);
    pthread_cond_destroy(&worker->done_cond);

    free(worker);
}
