#include <stdlib.h>
#include <errno.h>

#include "posigs.h"

static int posigs_handler_main(void * arg)
{
    /* the signal handler is passed as a pointer cast to void* */
    PosigsHandler * const handler = (PosigsHandler*) arg;

    /* while running: wait for signal with timeout and handle signal if one is available */
    while (handler->is_running) {
        const int signum = sigtimedwait(handler->sig_set, nullptr, &handler->timeout);
        if (signum > 0) {
            handler->sig_action(signum, handler->sig_action_arg);
        }
        else {
            /* actual error, not just timeout */
            if (errno != EAGAIN) {
                handler->is_running = false;
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

bool posigs_create_handler(PosigsHandler * handler)
{
    /* block any signals in sigs */
    if (pthread_sigmask(SIG_BLOCK, handler->sig_set, nullptr) != 0) {
        return false;
    }
    
    /* initialize atomic bool 'is_running' */
    atomic_init(&handler->is_running, true);

    /* launch signal handling thread */
    if (thrd_create(&handler->thread_id, posigs_handler_main, (void*) handler) != thrd_success) {
        handler->is_running = false;
        return false;
    }

    return true;
}

bool posigs_destroy_handler(PosigsHandler * handler)
{
    handler->is_running = false;

    int result;
    return (thrd_join(handler->thread_id, &result) == thrd_success && result == EXIT_SUCCESS);
}
