#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <stdbool.h>
#include <threads.h>
#include <stdatomic.h>

#include <signal.h>

typedef struct PosigsHandler {
    thrd_t thread_id;
    sigset_t * sig_set;
    void (*sig_action)(int signum, void * arg);
    void * sig_action_arg;
    struct timespec timeout;
    atomic_bool is_running;
} PosigsHandler;

bool posigs_create_handler(PosigsHandler * handler);
bool posigs_destroy_handler(PosigsHandler * handler);
static inline bool posigs_handler_is_running(PosigsHandler * handler) {return handler->is_running;}

#endif
